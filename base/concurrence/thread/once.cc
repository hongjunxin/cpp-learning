#include <thread>
#include <mutex>
#include <iostream>

int main(int argc, char** argv)
{
    static std::once_flag flag;
    auto f = []()
    {   
        std::call_once(flag,
            [](){                // 匿名lambda，初始化函数，只会执行一次
                std::cout << "only once" << std::endl;
            }
        );
    };

    std::thread t1(f);
    std::thread t2(f);

    // 必须加 join，否则会 coredump
    t1.join();
    t2.join();
}