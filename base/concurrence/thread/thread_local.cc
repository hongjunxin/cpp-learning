#include <thread>
#include <mutex>
#include <iostream>

int main(int argc, char** argv)
{
    thread_local int x = 0;
    auto f = [](int y) {
        x += y;
        std::cout << x << std::endl;
    };
    std::thread t1(f, 10);  // 输出 10
    std::thread t2(f, 20);  // 输出 20
    t1.join();
    t2.join();
}