#include <iostream>
#include <string>

// 变量是左值
// 左值持久、右值短暂
int main(int argc, char **argv)
{
    int i = 0;
    int &r = i;

    // 非常量的左值引用必须绑定到左值
    // int &error = 0;  // 错误
    const int &correct = 0;

    // 右值引用只能绑定到字面值常量、临时对象
    std::string &&s1 = "hi";
    std::cout << "before move, s1: " << s1 << std::endl;

    // 变量是左值，所以右边的 s1 是左值，
    // 我们无法将右值引用绑定到左值
    // std::string &&s2 = s1;  // 错误

    // std::move() 告诉编译器，我们有一个左值（这里是变量 s1）
    // 但我们希望它像一个右值一样被处理（右值的属性：临时性）
    std::string &&s2 = std::move(s1);
    std::cout << "after move, s1: " << s1 
        << ", s2: " << s2 << std::endl;

    // 两个右值引用绑定到了同一个对象
    s1[0] = 'k';
    std::cout << "after modified, s1: " << s1 
        << ", s2: " << s2 << std::endl << std::endl;

    std::string x1("hi");
    std::cout << "before move, x1: " << x1 << std::endl;
    // 错误。左值引用不能绑定到右值，因为左值引用需要绑定到
    // 一个持久的对象，而不是一个临时性的对象
    // std::string &x2 = std::move(x1);

    // x1 被 move() 处理成右值，并且被赋值给左值 x2,
    // x2 将接管 x1 的内部数据，所以 move 后 x1 变成了空字符串
    std::string x2 = std::move(x1);
    std::cout << "after move, x1: " << x1 
        << ", x2: " << x2 << std::endl << std::endl;

    // 右值引用只是绑定到 x2 指向的对象，所以没有接管 x2 的内部
    // 数据。应该说 |引用| 都只是指向现有对象，而不是独占现有对象
    std::string &&x3 = std::move(x2);
    std::cout << "after move, x2: " << x2 
        << ", x3: " << x3 << std::endl;    

    return 0;
}