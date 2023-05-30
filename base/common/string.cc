#include <string>
#include <iostream>
#include <cassert>

using namespace std::literals::string_literals;  //必须打开名字空间

int main(int argc, char** argv)
{
    // C++14
    auto str = "std string"s;      // 后缀s，表示是标准字符串，直接类型推导
    std::cout << str << std::endl;
    assert("time"s.size() == 4);   // 标准字符串可以直接调用成员函数 
}