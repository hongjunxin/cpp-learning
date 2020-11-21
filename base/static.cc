#include <iostream>


class A {
public:
    static int data;  /* static 成员在类中只能声明，不能初始化 */
    const int constData = 1;  /* const 数据成员可以在类中进行初始化 */

    static void show();

    void showNonStatic();
private:
    int non_static_data = 100;
};

void A::showNonStatic()
{
    std::cout << "A::showNonStatic()" << std::endl;
}

void A::show()
{
    // showNonStatic(); // 有问题，不能调用非静态成员
}

int A::data = 1;  /* static 成员只能在类的外部进行初始化，不用再加 private 或 static 关键字 */

int main(void)
{
    A a;
    std::cout << a.constData << std::endl;
    std::cout << A::data << std::endl;
    A::show();
    return 0;
}