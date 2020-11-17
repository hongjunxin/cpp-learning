/*
条款 07：为多态基类声明 virtual 析构函数

PS：对多态基类来说，不仅是它的析构函数要声明为 virtual，它的其它函数如果是设计为
要被子类重写的，则都应该声明为 virtual，这样当子类在使用的过程中被降级为基类的引用
或基类的指针时，通过该引用或指针，仍能正确地调用到被子类重写的函数，而不是基类的函数。
*/

#include <iostream>
using namespace std;

class CShape  //基类
{
public:
    virtual ~CShape() { cout << "CShape::destructor" << endl; }  /* ~CShape() 应该定义为 virtual */
    
    virtual void show(void) const {
        std::cout << "CShape::show" << std::endl;
    }
};

class CRectangle : public CShape  //派生类
{
public:
    ~CRectangle() { cout << "CRectangle::destructor" << endl; }

    /* 对于 CRectangle::show，加不加 virtual 都可以，它的子类 CRectangleArc
       就算被降级为 CRectangle 引用或指针，执行的也是 CRectangleArc::show */
    void show(void) const {
        std::cout << "CRectangle::show" << std::endl;
    }
};

class CRectangleArc : public CRectangle {
public:
    ~CRectangleArc() { cout << "CRectangleArc::destructor" << std::endl; }

    void show(void) const {
        std::cout << "CRectangleArc::show" << std::endl;
    }
};

static void showName(CShape& shape)
{
    shape.show();
}

int main()
{
    CShape* p = new CRectangle;
    p->show();  /* 调用 CRectangle::show */
    delete p;   /* 调用的是基类的析构函数，这是不正确的表现 */

    std::cout << std::endl;
    {
        /* 这种情况下，会按我们的意愿执行 CRectangle::destrutor，
           而之所以也会执行 CShape::destrutor，是因为编译器帮我们将 CShape::destructor 代码
           加入到 CRectangle::destructor 之中 */
        CRectangle p;
        p.show();
    }

    std::cout << std::endl;
    
    /* CRectangleArc 被降级为 CRectangle* 或 CShape*，以下的执行结果都是一样的 */
    CRectangle* q = new CRectangleArc;
    q->show();
    delete q;

    return 0;
}