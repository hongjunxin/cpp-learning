#include <iostream>
#include <string>

void foo(std::string &a, std::string &b)
{
    b = a;  // change b value, but address not changed
    std::cout << "[foo] a: " << a << ", address: " << &a << std::endl;
    std::cout << "[foo] b: " << b << ", address: " << &b << std::endl;
}


int main(int argc, char **argv)
{
    std::string a("A");
    std::string b("B");
    std::cout << "[main] a: " << a << ", address: " << &a << std::endl;
    std::cout << "[main] b: " << b << ", address: " << &b << std::endl;
    foo(a, b);
    std::cout << "[main] b: " << b << ", address: " << &b << std::endl;
    
    // std::string &c; // 引用在初始化是必须赋值，而指针不用

    return 0;
}