#include <iostream>
#include <malloc.h>
#include <thread>
#include <vector>
#include <algorithm>
#include "RvalueReferences.h"

long bssVar;

class parent {
public:
    static void increase() {
        a++;
    }
    static int getA() {
        return a;
    }

private:
    static int a;
};

int parent::a = 10;

// std::make_heap - 基于一个范围构造一个堆
// std::pop_heap  - 将堆中第一个元素（最大值）放到last-1的位置，也就是vector的最后，剩余的值继续保持堆得顺序
// std::push_heap - 将vector中新插入的值插入到堆中

void printVector(std::vector<int> &vec)
{
    for (auto v : vec)
    {
        std::cout << v << ", ";
    }
    std::cout << std::endl;
}


void testMakeHeapFromVector()
{
    std::vector<int> ints = { 1, 20, 7, 30, 12, 50, 17, 24 };
    printVector(ints);
    std::make_heap(ints.begin(), ints.end());
    printVector(ints);
    std::push_heap(ints.begin(), ints.end());
    printVector(ints);
}

void testStaticArrayInitialization()
{
    typedef struct person
    {
        char name[20];
        int dead;
    } Per;

    static Per few[2] = {
        {"ztzhang", 1},
        {"lxh", 3}
    };

    std::cout << "testStaticArrayInitialization:\n";
    std::cout << "\tfew[0].name = " << few[0].name << std::endl;
    std::cout << "\tfew[0].dead = " << few[0].dead << std::endl;
    std::cout << "\tfew[1].name = " << few[1].name << std::endl;
    std::cout << "\tfew[1].dead = " << few[1].dead << std::endl;
}

void testTypedefIntAndPointer()
{
    typedef int a, *b, c;
    a x1 = 10;
    b x2 = &x1;
    c x3 = *x2;

    std::cout << "testTypedefIntAndPointer:\n";
    std::cout << "\ttype of x1: " << typeid(x1).name() << std::endl;
    std::cout << "\ttype of x2: " << typeid(x2).name() << std::endl;
    std::cout << "\ttype of x3: " << typeid(x3).name() << std::endl;
}

void testClassStaticMember()
{
    std::cout << "testTypedefIntAndPointer:\n";
    std::cout << "\tparent::getA():" << parent::getA() << std::endl;
    parent::increase();
    std::cout << "\tparent::getA() after increase:" << parent::getA() << std::endl;
}

void testLong2Unsigned()
{
    long l = -1;
    unsigned int u = l;

    std::cout << "testTypedefIntAndPointer:\n";
    std::cout << "\tunsigned int of -1l: " << u << std::endl;
}

void testRvalueReferences()
{
    std::string up("up");
    const std::string down("down");
    quark(up);                          // 形参T&&, 实参是A型左值, 参数被推导为A&, T&& + & => T&
    quark(down);                        // 形参T&&, 实参是const A型左值, 参数被推导为const A&, T&& + & => T&
    quark(strange());
    quark(charm());

    //quantum(up);
    //quantum(down);
    quantum(strange());
    quantum(charm());
}

void testThreadConcurrency()
{
    //输出CPU数量
    std::cout << "testThreadConcurrency:\n";
    std::cout << "\tThread concurrency: " << std::thread::hardware_concurrency() << std::endl;
}

void testSignUnsign()
{
    int i = -2000;
    unsigned u = 1000;
    unsigned r = u + i;
    std::cout << "unsigned 1000 plus signed -2000 is equal to: " << r << std::endl;
}

void testLambda()
{
    //嵌套的内部lambda表达式可以抓取最外部的变量
    int outer = 10;
    int m = [=](int x)
    {
        int inner = 20;
        return [=](int y)
        {
            return y * 2 + outer + inner;
        }(x) + 3 + outer;
    }(5);

    std::cout << "testLambda:\n";
    std::cout << "\tm = " << m << std::endl;
}

/*
TEST_F(TestCpp, testHeapBssGap) {
    std::cout << "Program break (init): " << (long)sbrk(0) << std::endl;
    void *p = malloc(32);
    std::cout << "[Malloc once]" << std::endl;
    std::cout << "Program break (after malloc 32 bytes): " << (long)sbrk(0) << std::endl;
    std::cout << "BSS end: " << (long)&bssVar -4 << std::endl;
    std::cout << "Heap start: " << (long)p << std::endl;
    std::cout << "Gap between BSS and Heap: "
        << (long)p - (long)&bssVar - 4 << std::endl;
    free(p);
    std::cout << "Program break (after free 32 bytes): " << (long)sbrk(0) << std::endl;

    sbrk(32);
    std::cout << "Program break (after increase brk 32 bytes): " << (long)sbrk(0) << std::endl;
    p = malloc(32);
    std::cout << "[Malloc twice]" << std::endl;
    std::cout << "BSS end: " << (long)&bssVar -4 << std::endl;
    std::cout << "Heap start: " << (long)p << std::endl;
    std::cout << "Gap between BSS and Heap: "
        << (long)p - (long)&bssVar - 4 << std::endl;
    free(p);
}

TEST_F(TestCpp, testMallocInfo) {
    struct mallinfo mInfo = mallinfo();
    std::cout << "(Init)arena: " << mInfo.arena << " Non-mmapped space allocated (bytes)" << std::endl;
    std::cout << "(Init)ordblks: " << mInfo.ordblks << " Number of free chunks" << std::endl;
    std::cout << "(Init)smblks: " << mInfo.smblks << " Number of free fastbin blocks" << std::endl;
    std::cout << "(Init)hblks: " << mInfo.hblks << " Number of mmapped regions" << std::endl;
    std::cout << "(Init)hblkhd: " << mInfo.hblkhd << " Space allocated in mmapped regions (bytes)" << std::endl;
    std::cout << "(Init)usmblks: " << mInfo.usmblks << " Maximum total allocated space (bytes)" << std::endl;
    std::cout << "(Init)fsmblks: " << mInfo.fsmblks << " Space in freed fastbin blocks (bytes)" << std::endl;
    std::cout << "(Init)uordblks: " << mInfo.uordblks << " Total allocated space (bytes)" << std::endl;
    std::cout << "(Init)fordblks: " << mInfo.fordblks << " Total free space (bytes)" << std::endl;
    std::cout << "(Init)keepcost: " << mInfo.keepcost << " Top-most, releasable space (bytes)" << std::endl;

    std::cout << "Program break: " << (long)sbrk(0) << std::endl;
    void *p = malloc(512*1024);
    memset(p, 1, 512*1024);
    mInfo = mallinfo();
    std::cout << "Program break: " << (long)sbrk(0) << std::endl;
    std::cout << "(Mmap)arena: " << mInfo.arena << " Non-mmapped space allocated (bytes)" << std::endl;
    std::cout << "(Mmap)hblks: " << mInfo.hblks << " Number of mmapped regions" << std::endl;
    std::cout << "(Mmap)hblkhd: " << mInfo.hblkhd << " Space allocated in mmapped regions (bytes)" << std::endl;
    free(p);
    void *pm[10];
    for (int i=0; i<10; i++) {
        pm[i] = malloc(32*1024);
        memset(pm[i], 1, 32*1024);
    }
    mInfo = mallinfo();
    std::cout << "Program break: " << (long)sbrk(0) << std::endl;
    std::cout << "arena: " << mInfo.arena << " Non-mmapped space allocated (bytes)" << std::endl;
    std::cout << "hblks: " << mInfo.hblks << " Number of mmapped regions" << std::endl;
    std::cout << "hblkhd: " << mInfo.hblkhd << " Space allocated in mmapped regions (bytes)" << std::endl;
    for (int j=0; j<10; j++) {
        free(pm[j]);
    }
    std::cout << "Program break: " << (long)sbrk(0) << std::endl;
    mInfo = mallinfo();
    std::cout << "arena: " << mInfo.arena << " Non-mmapped space allocated (bytes)" << std::endl;
}

*/


int main()
{
    //testStaticArrayInitialization();
    //testTypedefIntAndPointer();
    //testClassStaticMember();
    //testLong2Unsigned();
    //testRvalueReferences();
    //testThreadConcurrency();
    //testMakeHeapFromVector();
    //testLambda();
    testSignUnsign();

    return 0;
}
