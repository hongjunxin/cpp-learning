#include <stdexcept>
#include <iostream>

class my_exception : public std::runtime_error
{
public:
    using this_type     = my_exception;        // 给自己起个别名
    using super_type    = std::runtime_error;  // 给父类也起个别名
public:
    my_exception(const char* msg):            // 构造函数
        super_type(msg)                      // 别名也可以用于构造
    {}  

    my_exception() = default;                // 默认构造函数
   ~my_exception() = default;                // 默认析构函数
private:
    int code = 0;                            // 其他的内部私有数据
};

[[noreturn]]                      // 属性标签
void raise(const char* msg)      // 函数封装throw，没有返回值
{
    throw my_exception(msg);     // 抛出异常，也可以有更多的逻辑
}

int main(int argc, char** argv)
{
    try {
        raise("noting");
    } catch (const std::exception& e) {
        std::cout << "catch exception: " << e.what() << std::endl;
    }
}