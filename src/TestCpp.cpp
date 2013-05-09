#include <iostream>
#include <gtest/gtest.h>

/*

typedef int a, *b, c;


class parent
{
private:
    static int a;
public:
    static void print();
};

void parent::print()
{
    std::cout << a << std::endl;
    a++;
    std::cout << a << std::endl;
}

int parent::a = 10;
*/

typedef struct person
{
    char name[20];
    int dead;
} Per;
static Per few[2] = {{"ztzhang", 1}, {"lxh", 3}};

class TestCpp : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(TestCpp, testStaticArrayInitialization) {
    EXPECT_STREQ("ztzhang", few[0].name);
    EXPECT_EQ(1, few[0].dead);
    EXPECT_STREQ("lxh", few[1].name);
    EXPECT_EQ(3, few[1].dead);
}

/*
int main()
{
  a x1 = 10;
  b x2 = &x1;
  c x3 = *x2;
  cout << x1 << endl;
  cout << *x2 << endl;
  cout << x3 << endl;
  cout << few[1].name << " " << few[1].dead << endl;
  cout << few[0].name << " " << few[0].dead << endl;
  parent::print();

}
*/

