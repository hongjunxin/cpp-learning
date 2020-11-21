#include <memory>
#include <iostream>

class B;
class A {
public:
    std::shared_ptr<B> ptr;

    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

class B {
public:
    std::shared_ptr<A> ptr;

    ~B() {
        std::cout << "~B()" << std::endl;
    }
};

void weak_ptr_test()
{
    std::weak_ptr<A> wpa;
    {
        std::shared_ptr<A> spa = std::make_shared<A>();
        wpa = spa;    
    }
    if (!wpa.lock())
        std::cout << "pa.lock() return nullptr" << std::endl;
}

/* 智能指针的循环引用，会造成内存泄漏，该函数运行结束后，~A() ~B() 不会被调用
   解决方法：将类 A 和 B 中的 std::shared_ptr 改为 std::weak_ptr  */
void shared_ptr_recursive_ref_test()
{
    std::shared_ptr<A> pa(new A());
    std::shared_ptr<B> pb(new B());
    pa->ptr = pb;
    pb->ptr = pa;   

    std::cout << "A object refcount = " << pa.use_count() << std::endl;
    std::cout << "B object refcount = " << pb.use_count() << std::endl;
}

int main(void)
{
    shared_ptr_recursive_ref_test();
    std::cout << std::endl;
    weak_ptr_test();
    return 0;
}
