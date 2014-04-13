/*
 * test_signal.cpp
 *
 *  Created on: 2014年4月10日
 *      Author: bill
 */
#include <signal.h>
#include <iostream>
#include <string>

int i = 0;

void signalHandler(int s) {
    i++;
}

int main() {
    signal(2, signalHandler);
    while (i < 10) {
        std::cout << i << std::endl;
        sleep(1);
    }
    return 0;
}

