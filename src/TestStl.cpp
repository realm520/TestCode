#include <gtest/gtest.h>
#include <memory>
#include <iostream>

class A {
public:
    int i;
};

class TestStl : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(TestStl, testAutoPtr) {
    EXPECT_EXIT({
            A *p = new A;
            std::auto_ptr<A> ptr1(p);
            // Error to get p owned by two auto_ptr
            std::auto_ptr<A> ptr2(p);
            ptr1->i = 10;
            EXPECT_EQ(10, ptr1->i);
            ptr2->i = 20;
            EXPECT_EQ(20, ptr2->i);
            EXPECT_EQ(20, ptr1->i);
            // p will be deleted twice before function return
//}, ::testing::ExitedWithCode(134), "");
    }, ::testing::KilledBySignal(6), "");
}

