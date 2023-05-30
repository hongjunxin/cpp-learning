#include <iostream>

/* 如果我们自已定义了其它类型的构造器，比如拷贝构造器，则编译器不会再为我们
   合成默认构造器，除非使用 default 来向编译器表明要为我们合成默认构造器 */
class A {
public:
    // A() = default; 
    A(const A& other) {
        std::cout << " copy constructor" << std::endl;
    }
};

void handleA(A a) 
{
    std::cout << "handleA()" << std::endl;
}

int main(void)
{
    A a;  /* 编译错：error: no matching function for call to ‘A::A()’ */
    handleA(a);

    return 0;
}
