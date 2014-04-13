/*
 * test_rlimit.cpp
 *
 *  Created on: 2014年4月12日
 *      Author: bill
 */

#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>

int main() {
    struct rlimit64 rlp;
    int ret = getrlimit64(RLIMIT_STACK, &rlp);
    if (ret == 0) {
        sleep(1);
    }
    return 0;
}

