#include <iostream>

/* 没有多态（vritual 函数），没有虚继承，从 sizeof 的结果中看出
   子类对象将基类对象的数据成员整入到自己的对象中。 */

class Base {
private:
    int a;
};

class DerivedA : public Base {
private:
    int b;
};

class DerivedB : public DerivedA {
private:
    int c;
};

int main(void)
{
    std::cout << "sizeof Base = " << sizeof(Base) << std::endl;  /* 输入 4 */
    std::cout << "sizeof DerivedA = " << sizeof(DerivedA) << std::endl;  /* 输出 8 */
    std::cout << "sizeof DerivedB = " << sizeof(DerivedB) << std::endl;  /* 输出 12 */

    return 0;
}

