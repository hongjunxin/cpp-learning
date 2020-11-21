#include <cstring>
#include <string>

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
    T ret() { return data; }
private:
    T data;
};

/* 特例化类的某个成员函数。注意，<> 只出现在类名后面 */
template<>
std::string A<std::string>::ret()
{
    return "hi";
}

/* 特例化整个类 */
template<>
class A<int> {
public:
    int ret() { return data; }
private:
    int data;
};

int main(void)
{
    compare("hi", "hello");  // 编译结果是 compare(const char[3], const char[6])

    return 0;
}
