#include <string>
#include <iostream>

int main(int argc, char **argv)
{
    std::string s = "world";
    std::string t = s + "!";  // 等价于 s.operator+("!")

    // C++ Primer 5th 提到这样写有问题，但是 c++11 编译、运行 OK
    std::string u = "hi " + s;

    std::cout << t << std::endl;
    std::cout << u << std::endl;
}