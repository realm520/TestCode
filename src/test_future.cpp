#include "test_future.h"
#include <future>
#include <thread>
#include <iostream>
#include <functional>

void print_int(std::future<int> &fut)
{
    int x = fut.get();
    std::cout << "value: " << x << std::endl;
}

void test_future()
{
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();
    std::thread t(print_int, std::ref(fut));
    prom.set_value(10);
    t.join();
}