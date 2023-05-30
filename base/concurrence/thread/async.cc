#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <cassert>

int main(int argc, char** argv)
{
    auto task = [](const auto x)
    {
        std::this_thread::sleep_for(
            x * std::chrono::microseconds(10));
        std::cout << "sleep for " << x << std::endl;
        return x;
    };

    auto f = std::async(task, 10);
    f.wait();

    assert(f.valid());                    // 确实已经完成了任务
    std::cout << f.get() << std::endl;
}