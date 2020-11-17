/*
条款 20：倾向于用 pass-by-reference-to-const 替换 pass-by-value

在 C++ 编译器的底层，reference 往往以指针实现出来，因此 pass by reference 
通常意味着真正传递的是指针。但是如果对象属于内置类型，则 pass by value 往往
比 pass by reference 效率高，STL 的迭代器和函数对象习惯上都被设计为 pass by value。
*/

#include <iostream>

class Window {
public:
    /* 如果要让子类重写，必须使用 virtual，这样才能保证子类重写的方法会被使用 */
    virtual void display(void) const {
        std::cout << "Window display() called" << std::endl;
    }
    
    /* virtual 方法在 C++ 和 Java 中的不同点：
       C++ --  1. 要求子类必须实现的方法，则基类要使用纯虚函数来定义该方法；
               2. 要求子类可以重写基类的方法，则使用 virtual 修饰该基类方法，基类还可以实现该方法
       Java -- 1. 要求子类必须实现的方法，则基类只需要用 virtual 修饰该方法就够了，而且被 virtual
                  修饰的方法不能有实现，它的实现被推迟到子类中，并且是强制子类必须实现该方法。
               2. 子类直接重写基类的方法就可以了，并不要求基类的方法必须要有 virtual 修饰。          
    */
    virtual void name(void) const {
        std::cout << "Window name() called" << std::endl;
    }
};

class WindowWithScrollBars : public Window {
public:
    void display(void) const {
        std::cout << "WindowWithScrollBars display() called" << std::endl;
    }

    void name(void) const {
        std::cout << "WindowWithScrollBars name() called" << std::endl;
    }    
};

/* 就算输入的参数是 Window 的子类，但也会被降级为 Window，这点与 Java 不同。 */
static void printNameAndDisplay(Window w)
{
    w.display();
}

/* 这里虽然用了 Window 的引用，但为了确保传入的 Window 子类的方法能被执行，还必须
   要求基类对应的方法使用了 virtual 关键字。另外，因为在 C++ 编译器的底层，reference 
   往往以指针实现出来，所以这里的参数如果是 Window*，跟 Window& 也是一样的表现。 */
static void showName(Window& w) {
    w.name();
}

int main(void) 
{
    WindowWithScrollBars wwsb;
    printNameAndDisplay(wwsb);  // 调用的是 Window::display
    showName(wwsb);  // 调用的是 WindowWithScrollBars::name

    return 0;
}
