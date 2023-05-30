#include <iostream>

class A {
public:
    virtual void show();
    void nonVirtualFunc();
};

class B : public A {
public:
    void show() override;  /* override 只能用于基类的 virtual 方法 */
    
    /* java 是可以重写基类中除了 final 以外的方法，但 C++ 觉得只有基类中
       的 virtual 方法才是为了被重写而存在 */
    //void nonVirtualFunc() override;
};

void A::show()
{
    std::cout << "A::show()" << std::endl;
}

void B::show()
{
    std::cout << "B::show()" << std::endl;
}

int main(void)
{
    return 0;
}

