#include <iostream>
#include <string>

class A {
public:
    A(std::string tag);
    A(const A& other);
    A& operator=(const A& other);
    ~A();

    void sayhello() {
        std::cout << "hello" << std::endl;
    }
private:
    std::string tag;
};

A::A(std::string _tag="default") : tag(_tag)
{
    std::cout << "A::A(" << tag << ")" << std::endl;
}

A::A(const A& other)
{
    std::cout << "A::A(const A&)" << std::endl;
}

A& A::operator=(const A& other)
{
    std::cout << "A::operator=(const A& other)" <<std::endl;
    //return A(other);  // 不需要这一句
}

A::~A()
{
    std::cout << "A::~A(" << tag << ")" << std::endl;
}

/* 将临时变量返回给引用，反而会导致创建了两个实例
   将临时变量返回给非引用，则只创建了一个实例。
   结论：尽量将临时变量返回给非引用 */
A& returnReferenceA()
{
    std::cout << "return reference A" << std::endl;
    A a("a");
    // 执行顺序
    // A::A(a)
    // A::~A(a)
    // A::A(const A&). 该函数执行前，实例 a 已经调用了 ~A()，所以在该函数
    // 中不能再对 a 进行任何的操作
    return a;
}

A returnNonReferenceA()
{
    std::cout << "return non-reference A" << std::endl;
    A a("a");
    return a;
}

int main(void)
{
    A a = returnReferenceA();
    a.sayhello();
    std::cout << std::endl;

    A aa = returnNonReferenceA();
    aa.sayhello();
    std::cout << std::endl;

    A aaa("aaa");
    // operator= 执行后，原来的 aaa 实例被析构掉
    // aa 和 aaa 指向同一个实例
    aaa = aa;

    return 0;
}

