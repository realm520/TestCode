#include <unistd.h>
#include <iostream>
#include <boost/thread.hpp>

boost::condition_variable cond;
boost::mutex mut;

void func1() {
    {
        //boost::lock_guard<boost::mutex> lock(mut);
        std::cout << "func1" << std::endl;
    }
    cond.notify_one();
    sleep(1);
}

void func2() {
    boost::unique_lock<boost::mutex> lock(mut);
    {
        cond.wait(lock);
    }
    std::cout << "func2" << std::endl;
}

int main() {
    boost::thread t1(func1);
    boost::thread t2(func2);
    t1.join();
    t2.join();

    return 0;
}

