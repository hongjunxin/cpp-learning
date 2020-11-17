/*
条款 46：需要类型转换时请为模板定义非成员函数

当我们编写一个 class template，而它所提供的“与此 template 相关的”函数
支持“所有参数的隐式类型转换”时，请将那些函数定义为“class template 内部的 friend 函数”
*/

#include <iostream>

template<typename T>
class Rational {
public:
    Rational(const T& numerator = 0, const T& denominator = 1)
        : _numerator(numerator), _denominator(denominator) {}

    const T numerator() const { return _numerator; }
    const T denominator() const { return _denominator; }

    /* 当创建 Rational<int> 对象时，class Rational<int> 于是被具体化出来，
       friend 函数 operator* 也会被自动声明出来，因此编译器在调用 operator* 时
       就可以执行隐式转行函数了（比如 Rational 的 non-explicit 构造函数）。*/
    friend const Rational<T> operator*(const Rational<T>& lhs,
                                       const Rational<T>& rhs)
    {
        return Rational(lhs.numerator() * rhs.numerator(),
                        lhs.denominator() * rhs.denominator());
    }

private:
    T _numerator;
    T _denominator;
};

/*
这种写法会导致 Rational<int> result = oneHalf * 2 编译不过。
因为 operator* 的第二个参数被声明为 Rational<T>，但传递给它的实际参数类型是 int，
编译器在 template 实参的推导过程中不会考虑隐式类型转换。

template<typename T>
const Rational<T> operator*(const Rational<T>& lhs,
                            const Rational<T>& rhs)
{
    return Rational(lhs.numerator() * rhs.numerator(),
                lhs.denominator() * rhs.denominator());
}
*/

int main(void)
{
    Rational<int> oneHalf(1, 2);
    Rational<int> result = oneHalf * 4;

    std::cout << result.numerator() << std::endl;

    return 0;
}
