#include <iostream>

int main(int argc, char** argv)
{
    int x = 1, y = 2;
    auto l1 = [=]() {
        //x += 1; // [=] 拷贝的变量在 lambda 内部不能修改
        std::cout << "l1 x=" << x << std::endl;
    };
    auto l2 = [&]() {
        x += 1;
        std::cout << "l2 x=" << x << std::endl;
    };
    auto l3 = [=, &y]() {
        // 除了 y 是引用，其他的是拷贝
        y += 1;
        // x=1 y=3
        std::cout << "l3 x=" << x << " y=" << y << std::endl;
    };
    auto l4 = [&]() {
        // x=2 y=3
        std::cout << "l4 x=" << x << " y=" << y << std::endl;
    };
    auto l5 = [=]() mutable {
        x += 1; // mutable 允许了 [=] 拷贝的值可被修改
        std::cout << "l5 x=" << x << std::endl;
    };

    l1();
    l2();
    l3();
    l4();
    l5();
    
    return 0;
}