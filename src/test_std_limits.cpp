#include <limits>
#include <iostream>

void testMaxMin()
{
    std::cout << __FUNCTION__ << ":" << std::endl;

    std::cout << "\tstd::numeric_limits<short>::max(): " << std::numeric_limits<short>::max() << std::endl;
    std::cout << "\tstd::numeric_limits<short>::min(): " << std::numeric_limits<short>::min() << std::endl;

    std::cout << "\tstd::numeric_limits<int>::max(): " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "\tstd::numeric_limits<int>::min(): " << std::numeric_limits<int>::min() << std::endl;

    std::cout << "\tstd::numeric_limits<long>::max(): " << std::numeric_limits<long>::max() << std::endl;
    std::cout << "\tstd::numeric_limits<long>::min(): " << std::numeric_limits<long>::min() << std::endl;

    std::cout << "\tstd::numeric_limits<float>::max(): " << std::numeric_limits<float>::max() << std::endl;
    std::cout << "\tstd::numeric_limits<float>::min(): " << std::numeric_limits<float>::min() << std::endl;

    std::cout << "\tstd::numeric_limits<double>::max(): " << std::numeric_limits<double>::max() << std::endl;
    std::cout << "\tstd::numeric_limits<double>::min(): " << std::numeric_limits<double>::min() << std::endl;

    std::cout << "\tstd::numeric_limits<long double>::max(): " << std::numeric_limits<long double>::max() << std::endl;
    std::cout << "\tstd::numeric_limits<long double>::min(): " << std::numeric_limits<long double>::min() << std::endl;
}

void testNumberCheck()
{
    std::cout << __FUNCTION__ << ":" << std::endl;

    std::cout << std::boolalpha;
    // 标准库是否为其实现了特化  
    std::cout << "\tstd::numeric_limits<float>::is_specialized: " << std::numeric_limits<float>::is_specialized << std::endl;
    // 是否是有符号的，即可以表示正负值  
    std::cout << "\tstd::numeric_limits<float>::is_signed: " << std::numeric_limits<float>::is_signed << std::endl;
    // 不否是整形的  
    std::cout << "\tstd::numeric_limits<float>::is_integer: " << std::numeric_limits<float>::is_integer << std::endl;
    // 是否是精确表示的  
    std::cout << "\tstd::numeric_limits<float>::is_exact: " << std::numeric_limits<float>::is_exact << std::endl;
    // 是否存在大小界限  
    std::cout << "\tstd::numeric_limits<float>::is_bounded: " << std::numeric_limits<float>::is_bounded << std::endl;
    // 两个比较大的数相加而不会溢出，生成一个较小的值  
    std::cout << "\tstd::numeric_limits<float>::is_modulo: " << std::numeric_limits<float>::is_modulo << std::endl;
    // 是否符合某某标准  
    std::cout << "\tstd::numeric_limits<float>::is_iec559: " << std::numeric_limits<float>::is_iec559 << std::endl;
    // 不加+-号可以表示的位数  
    std::cout << "\tstd::numeric_limits<float>::digits: " << std::numeric_limits<float>::digits << std::endl;
    // 十进制数的个数  
    std::cout << "\tstd::numeric_limits<float>::digits10: " << std::numeric_limits<float>::digits10 << std::endl;
    // 一般基数为2  
    std::cout << "\tstd::numeric_limits<float>::radix: " << std::numeric_limits<float>::radix << std::endl;
    // 以2为基数的最小指数  
    std::cout << "\tstd::numeric_limits<float>::min_exponent: " << std::numeric_limits<float>::min_exponent << std::endl;
    // 以2为基数的最大指数  
    std::cout << "\tstd::numeric_limits<float>::max_exponent: " << std::numeric_limits<float>::max_exponent << std::endl;
    // 以10为基数的最小指数  
    std::cout << "\tstd::numeric_limits<float>::min_exponent10: " << std::numeric_limits<float>::min_exponent10 << std::endl;
    // 以10为基数的最大指数  
    std::cout << "\tstd::numeric_limits<float>::max_exponent10: " << std::numeric_limits<float>::max_exponent10 << std::endl;
    // 1值和最接近1值的差距  
    std::cout << "\tstd::numeric_limits<float>::epsilon(): " << std::numeric_limits<float>::epsilon() << std::endl;
    // 舍入方式  
    std::cout << "\tstd::numeric_limits<float>::round_style: " << std::numeric_limits<float>::round_style << std::endl;
}