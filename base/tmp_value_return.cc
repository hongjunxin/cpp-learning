#include <iostream>

class A {
public:
    A();
    A(const A& other);
    A& operator=(const A& other);
    ~A();

    void sayhello() {
        std::cout << "hello" << std::endl;
    }
};

A::A()
{
    std::cout << "A::A()" << std::endl;
}

A::A(const A& other)
{
    std::cout << "A::A(const A&)" << std::endl;
}

A& A::operator=(const A& other)
{
    std::cout << "A::operator=(const A& other)" <<std::endl;
}

A::~A()
{
    std::cout << "A::~A()" << std::endl;
}

/* 将临时变量返回给引用，反而会导致创建了两个实例
   将临时变量返回给非引用，则只创建了一个实例。
   结论：尽量将临时变量返回给非引用 */
A& returnTmpA()
{
    A a;
    return a;
}

int main(void)
{
    A a = returnTmpA();
    a.sayhello();

    return 0;
}

