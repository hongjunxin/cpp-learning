#include <iostream>

class Point {
public:
    virtual ~Point();
    float x();
    float y();
    virtual float z();
};

Point::~Point() {}
float Point::x() {}
float Point::y() {}
float Point::z() {}

/* 《深入探索C++对象模型》 给出的这个 demo 好像有问题 */
int main(void)
{
    std::cout << "&Point::~Point = " << &Point::~Point << std::endl;  // 得到的结果是 1

    std::cout << "&Point::x = " << &Point::x << std::endl;  // 函数实体 x 在内存中的地址
    
    std::cout << "&Point::y = " << &Point::y << std::endl;  // 函数实体 y 在内存中的地址

    std::cout << "&Point::z = " << &Point::z << std::endl;  // 得到的结果是 2

    return 0;
}
