#include <locale>
#include <iostream>
#include <string>
#include <vector>
//#include <algorithm>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

struct employee {
    int id;
    std::string name;
};

int main() {
        std::locale loc;
        std::cout << "Locale name: " << loc.name() << std::endl;

    {
        employee a = { 10, "test" };
        employee b = a;
    }

        std::string str = ";;Hello|wor ld||-foo--bar;yo w;baz|"; 
        std::cout << str << std::endl;
        std::vector<std::string> vStr;
        boost::algorithm::split(vStr, str, boost::algorithm::is_any_of("|"));
        for (int i=0; i<vStr.size(); i++) {
            std::cout << vStr[i] << std::endl;
        }
        std::cout << boost::algorithm::join(vStr, "|") << std::endl;
        /*
        typedef boost::tokenizer<boost::char_separator<char> >     tokenizer; 
        boost::char_separator<char> sep("-;", "|", boost::keep_empty_tokens );
        tokenizer tokens(str, sep);
        for (tokenizer::iterator tok_iter = tokens.begin();
                        tok_iter != tokens.end();
                        ++tok_iter) {
                std::cout << "<" << *tok_iter << "> ";
        }

        std::cout << "\n";


        std::string dir = "/opt/abv/ddd/dae";
        std::string dir_a = "/opt";
        size_t pos = dir.find('/', dir_a.size()+1);
        std::cout << "pos: " << pos << std::endl;
        */

        return 0;
}
