#include <iostream>
#include <gtest/gtest.h>

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

