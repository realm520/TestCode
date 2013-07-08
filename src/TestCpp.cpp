#include <iostream>
#include <malloc.h>
#include <gtest/gtest.h>

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


class TestCpp : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(TestCpp, testStaticArrayInitialization) {
    typedef struct person {
        char name[20];
        int dead;
    } Per;

    static Per few[2] = {
        {"ztzhang", 1},
        {"lxh", 3}
    };

    EXPECT_STREQ("ztzhang", few[0].name);
    EXPECT_EQ(1, few[0].dead);
    EXPECT_STREQ("lxh", few[1].name);
    EXPECT_EQ(3, few[1].dead);
}

TEST_F(TestCpp, testTypedefIntAndPointer) {
    typedef int a, *b, c;
    a x1 = 10;
    b x2 = &x1;
    c x3 = *x2;

    EXPECT_EQ(10, x1);
    EXPECT_EQ(10, *x2);
    EXPECT_EQ(10, x3);
}

TEST_F(TestCpp, testClassStaticMember) {
    EXPECT_EQ(10, parent::getA());
    parent::increase();
    EXPECT_EQ(11, parent::getA());
}

TEST_F(TestCpp, testLong2Unsigned) {
    long l = -1;
    unsigned int u = l;
    EXPECT_EQ(-1l, u);
}

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

