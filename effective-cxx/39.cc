/*
条款 39：适当地使用 private 继承

private 继承意味着 implemented-in-terms-of。如果我们让 class D 以 private 形式
继承 class B，我们的用意是为了使用 class B 内已经准备好的某些特性，而不是因为 B 对象
和 D 对象之间存在任何观念上的关系。private 继承纯粹只是一种实现技术，这也是为什么 private 
继承一个 base class 后，base class 的每样东西到了 derived class 中都变成了 private。
*/

#include <iostream>

class Timer {
public:
    explicit Timer() {}

    void show() const {
        std::cout << "Timer::show" << std::endl;
    }

    virtual void onTick() const {
        std::cout << "Timer::onTick" << std::endl;
    }

};

/* private 继承是为了让 base class 内的数据变成子类私有，子类
   并不想把 base class 的 public/proteced 内容直接提供给外界使用。 */

class Widget : private Timer {
/* 对基类的方法进行封装，再 public 给外界 */
public:
    void show() const {
        Timer::show();
    }
/* 一般是在子类的 private 域中重写基类的方法 */    
private:
    virtual void onTick() const {
        std::cout << "Widget::onTick" << std::endl;
    }
};

int main(void) 
{
    Widget w;
    w.show();
}