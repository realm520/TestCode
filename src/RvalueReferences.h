#ifndef __RVALUE_REFERENCES_H__
#define __RVALUE_REFERENCES_H__

#include <string>

/*
引用折叠规则：
    X&  + &     = > X&
    X&& + &     = > X&
    X&  + &&    = > X&
    X&& + &&    = > X&&
*/

template <typename T> struct Name;

template <> struct Name<std::string>
{
    static const char * get()
    {
        return "string";
    }
};

template <> struct Name<const std::string>
{
    static const char * get()
    {
        return "const string";
    }
};

template <> struct Name<std::string&>
{
    static const char * get()
    {
        return "string&";
    }
};

template <> struct Name<const std::string&>
{
    static const char * get()
    {
        return "const string&";
    }
};

template <> struct Name<std::string&&>
{
    static const char * get()
    {
        return "string&&";
    }
};

template <> struct Name<const std::string&&>
{
    static const char * get()
    {
        return "const string&&";
    }
};

template <typename T> void quark(T&& t)
{
    std::cout << "quark: " << std::endl;
    std::cout << "\tt: " << t << std::endl;
    std::cout << "\tT: " << Name<T>::get() << std::endl;
    std::cout << "\tT&&: " << Name<T&&>::get() << std::endl;
    std::cout << std::endl;
}

template <typename T> void quantum(const T&& t)
{
    std::cout << "quantum: " << std::endl;
    std::cout << "\tt: " << t << std::endl;
    std::cout << "\tT: " << Name<T>::get() << std::endl;
    std::cout << "\tT&&: " << Name<T&&>::get() << std::endl;
    std::cout << std::endl;
}

std::string strange()
{
    return "strange()";
}

const std::string charm()
{
    return "charm()";
}

#endif
