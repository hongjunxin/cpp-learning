#include <iostream>

class A {
public:
    virtual void show() {
        std::cout << "A::show" << std::endl;
    }
};

class B : public A {
public:
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
    A a;
    B b;
    show(a);  /* A::show */
    show(b);  /* B::show */
    show2(&a);  /* A::show */
    show2(&b);  /* B::show */
}
