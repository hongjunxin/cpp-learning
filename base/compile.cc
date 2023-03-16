#include <iostream>

int main(int argc, char **argv)
{
    // 属性标签
    [[gnu::unused]]
    int a;

    // 静态断言，运行在编译阶段
    static_assert(
        sizeof(long) >= 8, "must run on x64");
    static_assert(
        sizeof(int)  == 4, "int must be 32bit");
    
    return 0;
}