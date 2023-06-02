#include <iostream>
#include <vector>

class A {
public:
    A(int value=0) : mValue(value) { std::cout << "A() called, address: " << this << std::endl; }
    ~A() { std::cout << "~A() called, address: " << this << std::endl; }
    A(A &a) { std::cout << "A(&) called, address: " << this << std::endl; }
    // 不做任何的对象、数据转移
    A(A &&a) { std::cout << "A(&&) called, address: " << this << std::endl; }

    A& operator=(A &a) { std::cout << "=(A&) called" << std::endl; return *this; }
    A& operator=(A &&a) { std::cout << "=(A&&) called" << std::endl; return *this; }

    int mValue;
};

// 传递的参数为引用，不会触发构造函数的调用
void foo(A &a)
{
    std::cout << "foo\n";
}

void bar(A &&a)
{
    std::cout << "bar\n";
}

void testA()
{
    std::vector<A> vec;
    A a;
    std::cout << "outer A address: " << &a << std::endl;

    foo(a);
    bar(std::move(a));  // std::move(a) 不会触发 A 构造函数的调用

    std::cout << "=== push_back(std::move(A)) ===\n";
    vec.push_back(std::move(a));  // push_back(A&&) 内部会调用 A(A&&)
}

// testB 的测试为了警惕我们，在编写类时，要注意它的 拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符
// 是否会被调用，是否直接使用编译器给我们自动生成的这些构造器，还是要自己编写。
void testB()
{
    std::vector<A> vec;
    A a(1);

    std::cout << "outer A address: " << &a << std::endl;
    std::cout << "=== push_back(std::move(A)) ===\n";
    // push_back(A&&) 内部会调用 A(A&&)，但是我们在 A(A&&) 构造函数中
    // 没有做任何对象、数据的转移操作，所以导致 A(A&&) 生成的对象中，mValue 的值是个随机数
    vec.push_back(std::move(a));

    auto front = vec.front();  // 触发对 A(A&) 的调用
    std::cout << "front a.mValue=" << front.mValue << std::endl; // 输出随机值
}

int main(int argc, char **argv)
{
    std::cout << "=== Test A ===\n";
    testA();
    std::cout << "=== Test A done ===\n";

    std::cout << "=== Test B ===\n";
    testB();
    std::cout << "=== Test B done ===\n";
}
