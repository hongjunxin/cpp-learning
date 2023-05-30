// vector emplace_back() push_back() test

#include <vector> 
#include <iostream> 

using namespace std; 

class testDemo {
public: 
    testDemo(int num):num(num) { 
        std::cout << "调用构造函数" << endl; 
    } 
    
    testDemo(const testDemo& other) :num(other.num) {
        std::cout << "调用拷贝构造函数" << endl; 
    } 
    
    testDemo(testDemo&& other) :num(other.num) {
        std::cout << "调用移动构造函数" << endl;
    } 
    
private: 
    int num; 
}; 

static void test1()
{
    cout << "emplace_back:" << endl;
    std::vector<testDemo> demo1;
    demo1.emplace_back(2); /* 发生构造器隐式转换 */
    cout << "push_back:" << endl;
    std::vector<testDemo> demo2;
    demo2.push_back(2);

/*
    执行结果：
    emplace_back:
    调用构造函数
    push_back:
    调用构造函数
    调用移动构造函数

    如果将 testDemo 的移动构造函数注释掉，则执行结果是：
    emplace_back:
    调用构造函数
    push_back:
    调用构造函数
    调用拷贝构造函数

    结论：
    1. 如果构造函数发生隐式转化，emplace_back 才是只调用了构造函数，用于构造对象，并
       将该对象直接存放到 vector 中，即 emplace_back 只创建了一个对象。
       而 push_back 调用了构造函数，用于构造临时对象，接着调用移动或拷贝构造函数，将该临时
       对象复制到 vector 中，即 push_back 创建了两个对象（其中一个是临时对象）
    2. push_back() 在底层实现时，会优先选择调用移动构造函数，如果没有才会调用拷贝构造函数。
*/
}

static void test2()
{
    testDemo bak(1);
    cout << "emplace_back:" << endl;
    std::vector<testDemo> demo1;
    demo1.emplace_back(bak);  /* 只调用了拷贝构造函数 */
    cout << "push_back:" << endl;
    std::vector<testDemo> demo2;
    demo2.push_back(bak);  /* 只调用了拷贝构造函数 */

/*
    结论：如果添加的是一个已经存在的对象，则 emplace_back 和 push_back 的表现一样，
    都只调用了拷贝构造函数，即将已存在的对象拷贝到 vector 中，所以这种情况下两者都只
    创建了一个对象。
*/    
}

int main() 
{
    test1();
    std::cout << std::endl;
    test2();
    std::cout << std::endl;
}