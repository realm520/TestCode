#include "test_file.h"
#include <fstream>
#include <iostream>

void test_file()
{
    std::ofstream ofs("d:\\test_file.log");
    std::ifstream ifs("test_file.log");

    int i = 0;
    //char line[1024];
    while (!ifs.eof())
    {
        ofs << "test_log: " << i++ << std::endl;
        std::cout << "write a new line\n";
        //ifs.getline(line, 1023);
        getchar();
    }
}