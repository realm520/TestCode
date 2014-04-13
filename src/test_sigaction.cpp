/*
 * test_sigaction.cpp
 *
 *  Created on: 2014年4月10日
 *      Author: bill
 */
#include <signal.h>
#include <iostream>

int i = 0;

void signalHandler(int sig, siginfo_t *si, void *d) {
    std::cout << si->si_pid << std::endl;
    i++;
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signalHandler;
    sigaction(2, &sa, NULL);
    while (i < 10) {
        std::cout << i << std::endl;
        sleep(1);
    }
    return 0;
}




