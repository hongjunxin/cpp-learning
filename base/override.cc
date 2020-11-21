#include <iostream>

class A {
public:
    virtual void show();
};

class B : public A {
public:
    void show() override;  /* override 只能用于基类的 virtual 方法 */
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

