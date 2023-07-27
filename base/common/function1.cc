/*
std::function是一个函数包装器，该函数包装器模板能包装任何类型的可调用实体，如普通函数，函数对象，lamda表达式等。
包装器可拷贝，移动等，并且包装器类型仅仅依赖于调用特征，而不依赖于可调用元素自身的类型。
std::function是 C++11 的新特性，包含在头文件<functional>中。
*/

#include <iostream>
#include <map>
#include <string>
#include <functional>

namespace {

int add(int i, int j)
{
    return i + j;
}

auto mod = [] (int i, int j) { return i % j; };

struct divide {
    int operator() (int denominator, int divisor) {
        return denominator / divisor;
    }
};

}

int main(int argc, char **argv)
{
    std::map<std::string, std::function<int (int, int)>> binops = {
        {"+", add},
        {"-", std::minus<int>()},
        {"/", divide()},
        {"*", [](int i, int j) {return i * j;}},
        {"%", mod}
    };

    std::cout << binops["+"](1, 2) << std::endl;
}