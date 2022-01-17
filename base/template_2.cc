#include <iostream>
#include <string>

template <typename Func, typename ...Args>
void callback(Func f, Args... args)
{
    f((args)...);
}

void foo1(int a, int b, std::string c)
{
    std::cout << "foo1() a=" << a 
        << ", b=" << b
        << ", c=" << c
        << std::endl;
}

void foo2(int a, std::string b)
{
    std::cout << "foo2() a=" << a
        << ", b=" << b
        << std::endl;
}

int main(int argc, char **argv)
{
    callback(foo1, 1, 2, "foo1");
    callback(foo2, 1, "foo2");
    return 0;
}
