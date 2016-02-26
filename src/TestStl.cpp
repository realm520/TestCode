#include <gtest/gtest.h>
#include <memory>
#include <iostream>

class TestStl : public testing::Test {
protected:
    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

TEST_F(TestStl, testConfig) {
#   if defined(__sgi)
    EXPECT_STREQ("__sgi", __sgi);
std::cout << "__sgi" << std::endl;
#   endif

#   if defined(__GNUC__)
    EXPECT_EQ(4, __GNUC__);
    EXPECT_EQ(8, __GNUC_MINOR__);
#   endif
}

template <typename T>
class StaticMemberTemplateClass {
public:
    static int _data;
};

template <> int StaticMemberTemplateClass<int>::_data = 1;
//This assignment override the previous one. Is this a bug? -- fixed in gcc 4.8
template <> int StaticMemberTemplateClass<char>::_data = 2;

TEST_F(TestStl, testStaticTemplateMember) {
    EXPECT_EQ(1, StaticMemberTemplateClass<int>::_data);
    EXPECT_EQ(2, StaticMemberTemplateClass<char>::_data);

    StaticMemberTemplateClass<int> obji1, obji2;
    StaticMemberTemplateClass<char> objc1, objc2;
    EXPECT_EQ(1, obji1._data);
    EXPECT_EQ(1, obji2._data);
    EXPECT_EQ(2, objc1._data);
    EXPECT_EQ(2, objc2._data);

    StaticMemberTemplateClass<int>::_data = 3;
    StaticMemberTemplateClass<char>::_data = 4;
    EXPECT_EQ(3, StaticMemberTemplateClass<int>::_data);
    EXPECT_EQ(4, StaticMemberTemplateClass<char>::_data);
    EXPECT_EQ(3, obji1._data);
    EXPECT_EQ(3, obji2._data);
    EXPECT_EQ(4, objc1._data);
    EXPECT_EQ(4, objc2._data);

    obji1._data = 5;
    objc2._data = 6;
    EXPECT_EQ(5, obji1._data);
    EXPECT_EQ(5, obji2._data);
    EXPECT_EQ(6, objc1._data);
    EXPECT_EQ(6, objc2._data);
}

template <class I, class O>
class ClassForPartialSpecialization {
public:
    ClassForPartialSpecialization() {
        i = 1;
    }

    int i;
};

template <class T>
class ClassForPartialSpecialization<T*, T*> {
public:
    ClassForPartialSpecialization() {
        i = 2;
    }

    int i;
};

template <class T>
class ClassForPartialSpecialization<const T*, T*> {
public:
    ClassForPartialSpecialization() {
        i = 3;
    }

    int i;
};

TEST_F(TestStl, testClassPartialSpecialization) {
    ClassForPartialSpecialization<int, char> obj1;
    ClassForPartialSpecialization<int*, int*> obj2;
    ClassForPartialSpecialization<const int*, int*> obj3;
    EXPECT_EQ(1, obj1.i);
    EXPECT_EQ(2, obj2.i);
    EXPECT_EQ(3, obj3.i);
}

class A {
public:
    int i;
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
    }, ::testing::KilledBySignal(6), "pointer being freed was not allocated");
}

