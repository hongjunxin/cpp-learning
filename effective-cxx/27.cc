/*
条款 27：尽量少做转型动作

上述例子表明，单一对象可能拥有一个以上的地址，例如以 Base* 指向它时的地址，和以 Derived* 指向它时的地址。
实际上一旦使用多重继承，这事几乎一直发生着，即使在单一继承中也可能发生。所以我们不应该做出“对象在 C++ 中
如何布局”的假设，更不应该以此假设为基础执行任何转型动作。
*/

#include <iostream>

class Base { };

class Derived : public Base { };

int main(void)
{
    Derived d;
    std::cout << "Derived addr = " << &d << std::endl;

    Base* pb = &d;
    std::cout << "Base addr = " << pb << std::endl;

    /* PS：这里的测试结果是，Derived addr 等于Base addr。没明白“单一对象可能拥有一个以上的地址”
       的说法  */

    return 0;
}
