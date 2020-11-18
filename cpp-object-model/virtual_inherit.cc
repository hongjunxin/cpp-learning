#include <iostream>

/* 为了更好展示，不添加数据成员 */

/* 空内容的 class， 它的实例大小是 1 个字节 */
class X {};
class XX {};
class XXX{};

/* virtual base table: 表格，用于存放指向虚继承中，基类实例的地址 */

/* 关于虚继承，不管虚继承了多少个基类，实例的大小只增大 8 个字节
   用于存放指向 virtual base table 的地址 */
class Y : public virtual X {};
class Z : public virtual X {};
class A : public virtual X,  public virtual XX, public virtual XXX {};

/* 不管继承了多少个（非虚继承）空的基类，实例的大小都是 1 个字节 */
class B : public X {};
class C : public X, public XX {};

/* 因为虚继承导致实例增加了 8 字节大小的指针， 用于存放指向 virtual base table 的地址
   所以 D 不再需要编译器因空类而追加的那一个字节。 */
class D : public X, public virtual XX {};

/* E 的实例大小为 16，所以在实例 E 中，它并没有合并 Y 和 Z 实例中各自的
   virtual base table */
class E : public Y, public Z {};

/* 因为 F 虚继承 XX，导致 F 的实例中增加了一个 virtual base table，但它整合了
   Y 的 virtual base table，所以 F 实例的大小为 8 个字节。  */
class F : public Y, public virtual XX {};

int main()
{
    std::cout << "sizeof X=" << sizeof(X) << std::endl;  /* 输出 1 */

    /* 以下都输出 8 */
    std::cout << "sizeof Y=" << sizeof(Y) << std::endl;
    std::cout << "sizeof Z=" << sizeof(Z) << std::endl;
    std::cout << "sizeof A=" << sizeof(A) << std::endl;

    /* 以下都输出 1 */
    std::cout << "sizeof B=" << sizeof(B) << std::endl;
    std::cout << "sizeof C=" << sizeof(C) << std::endl;

    std::cout << "sizeof D=" << sizeof(D) << std::endl;  /* 输出 8 */

    std::cout << "sizeof E=" << sizeof(E) << std::endl;  /* 输出 16 */

    std::cout << "sizeof F=" << sizeof(F) << std::endl;  /* 输出 8 */

    return 0;
}