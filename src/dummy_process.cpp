#include <fstream>
#include <iostream>
#include <windows.h>

int main()
{
    std::ifstream ifs("d:\\test_file.log");

    char line[1024];
    while (true)
    {
        if (ifs.eof())
        {
            ifs.clear();
            Sleep(1000);
        }
        else
        {
            ifs.getline(line, 1000);
            if (strlen(line) > 0)
            {
                std::cout << line << std::endl;
            }
            else
            {
                std::cout << "end of file.\n";
            }
            
        }
    }

    return 0;
}