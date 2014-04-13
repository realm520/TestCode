#include <iostream>
#include <sstream>
#include <string>
#include "Bmco/Util/IniFileConfiguration.h"

bool iniToString(Bmco::Util::IniFileConfiguration &conf, std::string &result) {
    Bmco::Util::AbstractConfiguration::Keys keys;
    conf.keys(keys);
    for (int i=0; i<keys.size(); i++) {
        std::cout << keys[i] << std::endl;
    }
    return true;
}

int main() {
    std::string path("/billing/ztzhang/test.ini");
    std::cout << path.substr(0, path.find('/', 1)) << ": " << path.find('/', 1) << std::endl;
    Bmco::AutoPtr<Bmco::Util::IniFileConfiguration> pConf =
            new Bmco::Util::IniFileConfiguration(path);
    //std::string result;
    //iniToString(*pConf, result);
    std::ostringstream oss;
    pConf->save(oss);
    std::cout << oss.str() << std::endl;

    return 0;
}

