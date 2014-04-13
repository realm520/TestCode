/*
 * test_FTPClientSession.cpp
 *
 *  Created on: 2014/04/09
 *      Author: Zhentao
 */
#include <iostream>
#include <string>
#include "Bmco/Net/FTPClientSession.h"
#include "Bmco/Net/NetException.h"

int main() {
    Bmco::Net::FTPClientSession session("192.168.0.69", 21);
    try {
        session.login("ctctest", "ctctest");
        //session.setWorkingDirectory("/opt/ctctest/opthb/src/cpp/hla/LogStat");
        std::string oldName("/opt/ctctest/opthb/src/cpp/hla/LogStat/file_old.log");
        std::string newName("/opt/ctctest/opthb/src/cpp/hla/LogStat/file_new.log");
        int i = 0;
        while (i < 10000) {
            session.rename(oldName, newName);
            session.rename(newName, oldName);
            i++;
            std::cout << "Times: " << i << std::endl;
        }
    } catch (Bmco::Net::FTPException &e) {
        std::cout << "FTP exception: " << e.displayText() << ", " << e.message() << std::endl;
    } catch (...) {
        std::cout << "Unknown exception.";
    }
    session.close();
}

