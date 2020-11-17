/*
条款 33：避免覆盖了继承得到的所有同名函数
*/

#include <iostream>

class Base {
public:
    virtual void mf1() = 0;
    virtual void mf1(int) { std::cout << "Base::mf1(int)" << std::endl;}
    virtual void mf2() { std::cout << "Base::mf2()" << std::endl; }
    void mf3() { std::cout << "Base::mf3()" << std::endl; }
    void mf3(double) { std::cout << "Base::mf3(double)" << std::endl; }
};

class DerivedBad : public Base {
public:
    virtual void mf1() {}  /* 覆盖掉了基类中的所有同名函数 mf1 */
    void mf3() {}  /* 覆盖掉了基类中的所有同名函数 mf3 */
};

class DerivedOk : public Base {
public:
    using Base::mf1;  // 让 Base class 内名为 mf1 和 mf3 的所有东西在 Derived 作用域内都可见
    using Base::mf3; 
    virtual void mf1() { std::cout << "DerivedOk::mf1()" << std::endl; }
    void mf3() { std::cout << "DerivedOk::mf3()" << std::endl; }
};

int main(void) 
{ 
    DerivedBad d;  /* DerivedOk 就没有以下的编译问题 */
    int x;

    d.mf1();
    d.mf1(x);  // 编译不过，因为 DerivedBad 中没有 void mf1(int) 函数
    
    d.mf2();

    d.mf3();
    d.mf3(x);  // 编译不过，因为 DerivedBad 中没有 void mf3(double) 函数
}
