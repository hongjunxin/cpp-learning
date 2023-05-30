#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    // C++14 () 中支持范型
    auto f = [](const auto& x)
    {
        return x + x;
    };

    std::cout << f(3) << std::endl;             // 参数类型是int
    std::cout << f(0.618) << std::endl;         // 参数类型是double

    std::string str = "matrix";
    std::cout << f(str) << std::endl;          // 参数类型是string
}