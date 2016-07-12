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
    // ��׼���Ƿ�Ϊ��ʵ�����ػ�  
    std::cout << "\tstd::numeric_limits<float>::is_specialized: " << std::numeric_limits<float>::is_specialized << std::endl;
    // �Ƿ����з��ŵģ������Ա�ʾ����ֵ  
    std::cout << "\tstd::numeric_limits<float>::is_signed: " << std::numeric_limits<float>::is_signed << std::endl;
    // ���������ε�  
    std::cout << "\tstd::numeric_limits<float>::is_integer: " << std::numeric_limits<float>::is_integer << std::endl;
    // �Ƿ��Ǿ�ȷ��ʾ��  
    std::cout << "\tstd::numeric_limits<float>::is_exact: " << std::numeric_limits<float>::is_exact << std::endl;
    // �Ƿ���ڴ�С����  
    std::cout << "\tstd::numeric_limits<float>::is_bounded: " << std::numeric_limits<float>::is_bounded << std::endl;
    // �����Ƚϴ������Ӷ��������������һ����С��ֵ  
    std::cout << "\tstd::numeric_limits<float>::is_modulo: " << std::numeric_limits<float>::is_modulo << std::endl;
    // �Ƿ����ĳĳ��׼  
    std::cout << "\tstd::numeric_limits<float>::is_iec559: " << std::numeric_limits<float>::is_iec559 << std::endl;
    // ����+-�ſ��Ա�ʾ��λ��  
    std::cout << "\tstd::numeric_limits<float>::digits: " << std::numeric_limits<float>::digits << std::endl;
    // ʮ�������ĸ���  
    std::cout << "\tstd::numeric_limits<float>::digits10: " << std::numeric_limits<float>::digits10 << std::endl;
    // һ�����Ϊ2  
    std::cout << "\tstd::numeric_limits<float>::radix: " << std::numeric_limits<float>::radix << std::endl;
    // ��2Ϊ��������Сָ��  
    std::cout << "\tstd::numeric_limits<float>::min_exponent: " << std::numeric_limits<float>::min_exponent << std::endl;
    // ��2Ϊ���������ָ��  
    std::cout << "\tstd::numeric_limits<float>::max_exponent: " << std::numeric_limits<float>::max_exponent << std::endl;
    // ��10Ϊ��������Сָ��  
    std::cout << "\tstd::numeric_limits<float>::min_exponent10: " << std::numeric_limits<float>::min_exponent10 << std::endl;
    // ��10Ϊ���������ָ��  
    std::cout << "\tstd::numeric_limits<float>::max_exponent10: " << std::numeric_limits<float>::max_exponent10 << std::endl;
    // 1ֵ����ӽ�1ֵ�Ĳ��  
    std::cout << "\tstd::numeric_limits<float>::epsilon(): " << std::numeric_limits<float>::epsilon() << std::endl;
    // ���뷽ʽ  
    std::cout << "\tstd::numeric_limits<float>::round_style: " << std::numeric_limits<float>::round_style << std::endl;
}