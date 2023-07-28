#include <iostream>

class A {
public:
    virtual ~A() {
        std::cout << "~A() address " << this << std::endl;
    }
    virtual void show() {
        std::cout << "A::show" << std::endl;
    }
};

class B : public A {
public:
    ~B() {
        std::cout << "~B() address "  << this << std::endl;
    }
    void show() {
        std::cout << "B::show" << std::endl;
    }
};

static void show(A &obj)
{
    obj.show();
}

static void show2(A *obj)
{
    obj->show();
}

int main(int argc, char **argv) 
{
    {
        A a;
        B b;
        show(a);  /* A::show */
        show(b);  /* B::show */
        show2(&a);  /* A::show */
        show2(&b);  /* B::show */

        // 基类 A 的析构函数不管是不是 virtual 函数，析构函数的执行结果都一样
        // ~B() address 0x7ffdfd8678c8
        // ~A() address 0x7ffdfd8678c8
        // ~A() address 0x7ffdfd8678c0
    }

    std::cout << std::endl;

    {
        A *b = new B;
        delete b;
        // 基类 A 的析构函数如果不是 virtual 函数，则这里只会调用 A 的析构函数，
        // 而不会调用 B 的析构函数。
        // 结论：当我们 delete 一个动态分配的对象的指针时，要确保基类的析构函数
        // 是 virtual 函数
    }
}
