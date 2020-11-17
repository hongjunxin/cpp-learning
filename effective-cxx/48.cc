/*
条款 48：认识 template 元编程
*/

#include <iostream>

template<unsigned n>
struct Factorial {
    enum { value = n * Factorial<n-1>::value };
};

template<>
struct Factorial<0> {
    enum { value = 1 };
};

int main(void)
{
    Factorial<4> f;
    std::cout << f.value << std::endl;
    
    std::cout << Factorial<4>::value << std::endl;

    return 0;
}