#include <iostream>
#include <typeinfo>

int main(int argc, char** argv)
{
    auto        x = 10L;          // auto推导为long，x是long
    auto&       x1 = x;           // auto推导为long，x1是long&
    auto*       x2 = &x;          // auto推导为long，x2是long*
    const auto& x3 = x;           // auto推导为long，x3是const long&

    auto        x4 = &x3;         // auto推导为const long*，x4是const long*
    auto*       x5 = &x3;

    const std::type_info& x1_info = typeid(x1);
    const std::type_info& x2_info = typeid(x2);
    const std::type_info& x3_info = typeid(x3);
    const std::type_info& x4_info = typeid(x4);
    const std::type_info& x5_info = typeid(x5);

    std::cout << "x1: " << x1_info.name() << " | " << x1 << std::endl
        << "x2: " << x2_info.name() << " | " << x2 << std::endl
        << "x3: " << x3_info.name() << " | " << x3 << std::endl
        << "x4: " << x4_info.name() << " | " << x4 << std::endl
        << "x5: " << x5_info.name() << " | " << x5 << std::endl;
}