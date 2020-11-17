/*
条款 30：透彻了解 inlining

-- inline 函数放置于头文件中 --

inline 函数通常被置于头文件中，因为大多数编译器为了将一个“函数调用”替换为“被调用函数的本体”，
编译器需要知道那个函数长什么样子，这跟 template 需要放在头文件中是一样的原因。不过也有可以在
运行期间完成 inlining，但这样的环境是少数的。

-- inline 只是个申请 --

inline 是个申请，编译器可以忽略它。因为大部分编译器拒绝将太过复杂（比如带有循环或递归）的函数
inlining，所有对 virtual 函数的调用也都会使 inlining 落空。因为 virtual 意味着“等待，直到
运行期才确定调用哪个函数”，而 inline 意味着“执行前，先将调用动作替换为被调用函数的本体”。如果
编译器不知道该调用哪个函数，我们就很难责备它们拒绝将函数本体 inlining。

-- 库设计者的思考 --

inline 函数无法随着库的升级而升级，比如库中有个 inline 函数 f，客户将 f 编进自己的程序中，一旦
库设计者决定改变 f，则所有用到 f 的客户端程序都必须重新编译。

大部分调试器面对 inline 函数都束手无策，因为得考虑如何在一个并不存在的函数内设立断点。
*/

#include <iostream>

class Person {
public:
    Person(int _age = 100) : theAge(_age) { }
    int age() const { return theAge; }  // 隐喻的 inline 申请
private:
    int theAge;
};

int main(void)
{
    Person p;
    std::cout << p.age() << std::endl;

    return 0;
}