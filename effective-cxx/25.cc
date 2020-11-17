/*
条款 25：考虑写出一个不抛出异常的 swap 函数
*/

#include <iostream>
#include <vector>
#include <string>
#include <memory>

class WidgetImpl {
public:
    WidgetImpl() {
        data = std::vector<std::string>(10);
    }
private:
    std::vector<std::string> data;

    /* 在c++的类体中，方法以外的区域不允许有初始化，简单类型是
       可以的（例如 int 等简单类型变量，以及静态成员变量），但是
       有构造函数的复杂对象就不可以了（例如 vector）。*/
   // std::vector<std::string> data(10);  // 编译不过
};

class Widget {
public:
    void swap(Widget& other)
    {
        std::cout << "Widget::swap(Widget& other) called" << std::endl;
        std::swap(pImpl, other.pImpl);  /* 提高交换效率，所以只交换数据的指针 */
    }
private:
    WidgetImpl* pImpl;
};

namespace std {
    template<>  /* 特例化该模板 */
    void swap<Widget>(Widget& a, Widget& b)
    {
        std::cout << "std::swap<Widget>(Widget& a, Widget& b) called" << std::endl;
        a.swap(b);
    }
}

int main(void)
{
    Widget a, b;
    std::swap(a, b);

    return 0;
}