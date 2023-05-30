#include <cstring>
#include <string>
#include <iostream>

template<typename T, typename D> 
int compare(const T &v1, const D &v2)
{
    if(v1 > v2) return -1;
    if(v2 > v1) return 1;
    return 0;
}

/* T 特例化为 char* const，如果传入的是"hi"，则 char* const 被翻译为 char[3]
   D 特例化为 char* const, 如果传入的是"hello"，则 char* const 被翻译为 char[6]  */
template<>
int compare(const char* const &v1, const char* const &v2)
{
    return strcmp(v1, v2);
}

template <typename T>
class A {
public:
    T ret()
    {
        std::cout << "A<typename T>::ret()" << std::endl;
        return data; 
    }
private:
    T data;
};

// 模块特例化的需求背景：虽然模块规定了不同 typename T 的表现，比如 
// A<typename T>::ret() 的输出，但是某些特定的 T 或是 T 的某个函数
// 想要跟 A<typename T> 有不同的表现，所以才有了模板特例的出现。
// 编译器会优先使用模板特例，比如优先使用 A<std::string>::ret() 而不是
// A<typename T>::ret() 

/* 特例化类的某个成员函数。注意，<> 只出现在类名后面 */
template<>
std::string A<std::string>::ret()
{
    std::cout << "A<std::string>::ret()" << std::endl;
    return "hi";
}

/* 特例化整个类 */
template<>
class A<int> {
public:
    int ret()
    { 
        std::cout << "A<int>::ret()" << std::endl;
        return data; 
    }
private:
    int data;
};

int main(void)
{
    compare("hi", "hello");  // 编译结果是 compare(const char[3], const char[6])
    A<std::string> astr;
    astr.ret();

    A<int> aint;
    aint.ret();

    A<float> afloat;
    afloat.ret();

    return 0;
}
