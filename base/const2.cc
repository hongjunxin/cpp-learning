#include <iostream>

int main(int argc, char** argv)
{
    // volatile 覆盖了 const 的属性
    const volatile int vol_value  = 1024;

    auto ptr = (int*)(&vol_value);
    *ptr = 2048;
    std::cout << "volatile_value: " << vol_value << std::endl;  // 输出2048

    const int const_value = 1024;
    ptr = (int*)(&const_value);
    *ptr = 2048;
    // const 属性告知了编译器运行期间不会做修改，所以编译器用了 1024 替换了
    // 读取 const_value 的地方
    std::cout << "const_value: " << const_value << std::endl;  // 输出1024
}
