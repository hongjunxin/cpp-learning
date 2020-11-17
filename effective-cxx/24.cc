/*
条款 24：若所有参数皆需要类型转换，请为此采用 non-member 函数
*/

#include <iostream>

class Rational {
public:
    Rational(int _numerator = 0, int _denominator = 1) : 
         numerator(_numerator), denominator(_denominator) { }  // 支持隐式转换

    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
    
    // Rational operator*(const Rational& rhs) const;  // 有问题

    void showValue(void) const {
        std::cout << "numerator:" << numerator << ", denominator:"
            << denominator << std::endl;
    }
private:
    int numerator;
    int denominator;
};

const Rational operator*(const Rational& lhs, const Rational& rhs)
{
    return Rational(lhs.getNumerator() * rhs.getNumerator(),
                    lhs.getDenominator() * rhs.getDenominator());
}

int main(void)
{
    Rational result;
    result.showValue();

    Rational oneHalf(1, 2);
    result = oneHalf * 2;
    result = 2 * oneHalf;  // 正确：能通过编译

    return 0;
}