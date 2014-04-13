#include "Bmco/Path.h"
#include "Bmco/DirectoryIterator.h"
#include <iostream>
#include <string>
#include <vector>

static void findFiles(const std::string &path, std::vector<Bmco::File> &files) {
    Bmco::DirectoryIterator it(path);
    Bmco::DirectoryIterator end;
    while (it != end) {
        if (files.size() >= 100) {
            break;
        }
        if (it->isFile()) {
            if (it->path().size() > 4 && it->path().substr(it->path().size()-4, 4) == ".log") {
                files.push_back(*it);
            }
        } else if (it->isDirectory()) {

            findFiles(it->path(), files);
        }
        ++it;
    }
}

int main() {
    std::vector<Bmco::File> files;
    std::string path(".");
    findFiles(path, files);
    for (int i=0; i<files.size(); i++) {
        std::cout << files[i].path() << std::endl;
    }

    return 0;
}

