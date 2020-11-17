/*
条款 05：了解 C++ 默默编写并调用哪些函数
*/

#include <string>

template<class T>
class NamedObject {
public:
    NamedObject(std::string& name, const T& value) :
        nameValue(name), objectValue(value) { }
    
    /* 因为 nameValue 成员是引用来的，所以我们得自己定义 operator= */
    NamedObject& operator=(NamedObject& other) {
        nameValue = other.nameValue;
        objectValue = other.objectValue;
        return *this;  /* 令 operator= 返回一个 reference to *this，这只是个协议，并无强制要求 */
    }
    
private:
    std::string& nameValue;
    T objectValue;  /* 不能定义为 const，否则不给使用 operator= */

    NamedObject(NamedObject& other);  /* 禁止使用 copy 构造器 */
}; 

/*
编译器拒绝为类生成 copy assignment 操作符的情况：
- C++ 不允许“让 reference 改指向不同对象”，所以上面的“p=s”被拒绝编译，
  所以如果打算在一个内含 reference 成员的 class 内支持赋值操作，我们必须自己定义 copy assignment 操作符。

- 面对内含 const 成员的 class，编译器的反应也一样，更改 const 成员是不合法的，
  所以编译器不知道如何在它自己生成的赋值函数内面对它们。
  
- 如果某个 base class 将 copy assignment 操作符声明为 private，编译器将拒绝为其 derived class 
  生成一个 copy assignment 操作符。毕竟编译器为 derived class 所生成的 copy assignment 操作符想象中可以
  处理 base class 部分，但它们无法调用 derived class 无权调用的成员函数。
*/

int main(void) 
{
    std::string newDog("Persephone");
    std::string oldDog("Satch");
    NamedObject<int> p(newDog, 2);
    NamedObject<int> s(oldDog, 36);

    p = s ;  // 编译器拒绝编译，即编译器拒绝为 NamedObject 生成 copy assignment 操作符
}
