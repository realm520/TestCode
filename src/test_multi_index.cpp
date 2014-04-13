#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include "Bmco/Util/IniFileConfiguration.h"
#include "Bmco/NumberFormatter.h"

using boost::multi_index_container;
using namespace boost::multi_index;


struct ChannelTable {
    std::string channelId;
    std::string channelName;
    std::string channelDesc;
    std::string sysType;
    std::string subSysType;
    std::string ruleType;
    std::string beginDate;
    std::string createDate;
    std::string endDate;
    std::string outFilePath;
    std::string outHostName;
    std::string outUserName;
    std::string value1;
    std::string value2;
    std::string value3;
    std::string value4;
    std::string bolId;
    std::string status;

    ChannelTable() {
    }

    ChannelTable(const ChannelTable &c) : channelId(c.channelId), channelName(c.channelName),
        channelDesc(c.channelDesc), sysType(c.sysType), subSysType(c.subSysType),
        ruleType(c.ruleType), beginDate(c.beginDate), createDate(c.createDate),
        endDate(c.endDate), outFilePath(c.outFilePath), outHostName(c.outHostName),
        outUserName(c.outUserName), value1(c.value1), value2(c.value2), value3(c.value3),
        value4(c.value4), bolId(c.bolId), status(c.status) {
    }

    friend std::ostream& operator<<(std::ostream& os, const ChannelTable& e) {
        os << e.channelId << "|" << e.channelName << "|" << e.channelDesc << "|"
            << e.sysType << "|" << e.subSysType << "|" << e.ruleType << "|" 
            << e.beginDate << "|" << e.createDate << "|" << e.endDate << "|"
            << e.outFilePath << "|" << e.outHostName << "|" << e.outUserName << "|"
            << e.value1 << "|" << e.value2 << "|" << e.value3 << "|" << e.value4 << "|"
            << e.bolId << "|" << e.status;
        return os;
    }

    bool operator<(const ChannelTable &c) const {
        return channelId < c.channelId;
    }
};

struct cId{};
struct bId{};

typedef
boost::multi_index_container<
    ChannelTable,
    indexed_by<
        //ordered_unique<tag<cId>, identity<ChannelTable> >,
        ordered_unique<tag<cId>, member<ChannelTable, std::string, &ChannelTable::channelId> >,
        ordered_non_unique<tag<bId>, member<ChannelTable, std::string, &ChannelTable::bolId> >
    >
> ChannelTableContainer;

struct ChangeChannelFunctor {
    ChangeChannelFunctor(const std::string &outFilePath, const std::string &outHostName,
        const std::string &outUserName, const std::string &bolId) : newOutFilePath(outFilePath),
        newOutHostName(outHostName), newOutUserName(outUserName), newBolId(bolId) {
    }

    void operator()(ChannelTable& c) {
            c.outFilePath = newOutFilePath;
            c.outHostName = newOutHostName;
            c.outUserName = newOutUserName;
            c.bolId = newBolId;
    }

private:
    std::string newOutFilePath;
    std::string newOutHostName;
    std::string newOutUserName;
    std::string newBolId;
};

void reassignChannel(const std::string &bolIdBusy, const std::string &bolIdFree,
    Bmco::AutoPtr<Bmco::Util::IniFileConfiguration> &config, int flag = 1) {
    ChannelTableContainer ctc;
    int channelCount = config->getInt("Table.Table.8.recordnum");
    std::string recordPrefix("Table.Table.8.record.");
    for (int i=1; i<=channelCount; i++) {
        std::string record = config->getString(recordPrefix+Bmco::NumberFormatter::format(i));
        std::vector<std::string> fields;
        boost::algorithm::split(fields, record, boost::algorithm::is_any_of("|"));
        ChannelTable ct;
        ct.channelId = fields[0];
        ct.channelName = fields[1];
        ct.channelDesc = fields[2];
        ct.sysType = fields[3];
        ct.subSysType = fields[4];
        ct.ruleType = fields[5];
        ct.beginDate = fields[6];
        ct.createDate = fields[7];
        ct.endDate = fields[8];
        ct.outFilePath = fields[9];
        ct.outHostName = fields[10];
        ct.outUserName = fields[11];
        ct.value1 = fields[12];
        ct.value2 = fields[13];
        ct.value3 = fields[14];
        ct.value4 = fields[15];
        ct.bolId = fields[16];
        ct.status = fields[17];
        ctc.insert(ct);
    }

    ChannelTableContainer::nth_index<1>::type& ib= ctc.get<1>();

    int busyCount = ib.count(bolIdBusy);
    int freeCount = ib.count(bolIdFree);
    //std::cout << "Number of channel with busy bolId [" << bolIdBusy << "]: " << busyCount << std::endl;
    //std::cout << "Number of channel with free bolId [" << bolIdFree << "]: " << freeCount << std::endl;
    int newBusyCount = 0;
    if (flag == 1) { // reassign channel by mean
        newBusyCount = (busyCount + freeCount) / 2;
        ChannelTableContainer::index<bId>::type::iterator itFree = ib.find(bolIdFree);
        if (itFree == ib.end()) {
            std::cout << "error\n";
            return;
        }
        ChannelTable tmpChannel(*itFree);
        if (tmpChannel.status == "0") {
            tmpChannel.status = "1";
            ib.replace(itFree, tmpChannel);
        }
        while (busyCount > newBusyCount) {
            ChannelTableContainer::index<bId>::type::iterator itr = ib.find(bolIdBusy);
            if (!ib.modify(itr, ChangeChannelFunctor(tmpChannel.outFilePath, tmpChannel.outHostName,
                tmpChannel.outUserName, tmpChannel.bolId))) {
                std::cout << "Failed to update channel table." << std::endl;
            }
            busyCount--;
        }
    } else if (flag == 0) { // tack back channel from BOL
        newBusyCount = busyCount + freeCount - 1;
        ChannelTableContainer::index<bId>::type::iterator itBusy = ib.find(bolIdBusy);
        if (itBusy == ib.end()) {
            std::cout << "error\n";
            return;
        }
        ChannelTable tmpChannel(*itBusy);
        while (busyCount < newBusyCount) {
            ChannelTableContainer::index<bId>::type::iterator itr = ib.find(bolIdFree);
            if (!ib.modify(itr, ChangeChannelFunctor(tmpChannel.outFilePath, tmpChannel.outHostName,
                tmpChannel.outUserName, tmpChannel.bolId))) {
                std::cout << "Failed to update channel table." << std::endl;
            }
            busyCount++;
        }
        itBusy = ib.find(bolIdFree);
        ChannelTable disabledChannel(*itBusy);
        if (disabledChannel.status == "1") {
            disabledChannel.status = "0";
            ib.replace(itBusy, disabledChannel);
        }
    }
    ChannelTableContainer::iterator it = ctc.begin();
    int idx = 1;
    std::ostringstream oss;
    while (it != ctc.end()) {
        oss << (*it);
        config->setString(recordPrefix+Bmco::NumberFormatter::format(idx), oss.str());
        oss.str("");
        it++;
        idx++;
    }
    config->save(std::cout);
}


int main() {
    std::string busy("bol2");
    std::string free("bol3");
    std::string path("./test.ini");
    Bmco::AutoPtr<Bmco::Util::IniFileConfiguration> config = new Bmco::Util::IniFileConfiguration(path);
    reassignChannel(busy, free, config);
}
