/*
条款 37：绝不重新定义继承而来的默认参数值

C++ 坚持“默认参数取决于静态类型”的做法，是因为在乎运行期效率，
如果默认参数是动态绑定的，则编译器必须有某种方法在运行期为 virtual 函数决定适当的参数默认值。
*/

#include <iostream>

class Shape {
public:
    enum ShapeColor { Red, Green, Blue };
    virtual void draw(ShapeColor color = Red) const = 0;
};

class Rectangle : public Shape {
public:
    /* 将继承而来的默认参数值改为 Shape::Green */
    virtual void draw(ShapeColor color = Green) const {
        std::cout << "color=" << color << std::endl;
    }
};

int main(void)
{
    Shape* pr = new Rectangle;  /* pr 的静态类型是 Shape*, 动态类型是 Rectangle* */
    pr->draw();  // 执行的是 Rectangle::draw(Shape::Red)

    return 0;
}
