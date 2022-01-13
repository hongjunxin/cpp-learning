// 成员初始化列表测试

#include <iostream>

class Inner {
public:
    Inner() {
        std::cout << "Inner::Inner()" << std::endl;
    }
    Inner(Inner& other) {
        std::cout << "Inner::Inner(Inner&)" << std::endl;
    }
    Inner& operator=(Inner& other) {
        std::cout << "Inner::operator=(Inner&)" << std::endl;
    }
    ~Inner() = default;
};

class Obj_1 {
public:
    Obj_1(Inner& _i) {
        std::cout << "Obj_1::Obj_1()" << std::endl;
        i = _i;
    }
private:
    Inner i;
};

class Obj_2 {
public:
    Obj_2(Inner& _i) : i(_i) {
        std::cout << "Obj_2::Obj_2()" << std::endl;
    }
private:
    Inner i;
};

int main(int argc, char **argv)
{
    Inner i;
    std::cout << std::endl;

    std::cout << "== create Obj_1 ==" << std::endl;
    Obj_1 o1(i);
    std::cout << std::endl;

    std::cout << "== create Obj_2 ==" << std::endl;
    Obj_2 o2(i);
    std::cout << std::endl;

    return 0;

/*
    运行结果：
    Inner::Inner()

    == create Obj_1 ==
    Inner::Inner()
    Obj_1::Obj_1()
    Inner::operator=(Inner&)

    == create Obj_2 ==
    Inner::Inner(Inner&)
    Obj_2::Obj_2()
*/

/*
    1. 使用成员初始化列表，则类类型的数据成员在构造并初始化的过程中，只
       调用了本身的拷贝构造器
    2. 没有使用成员初始化列表，则类类型的数据成员在构造并初始化的过程中，
       先调用了本身的默认构造器来创建实例，再在主体类的构造函数中（如果我们
       有进行赋值的话，比如本例中的 i = _i），执行拷贝赋值运算符、或移动赋值
       运算符进行赋值操作。
*/
}
