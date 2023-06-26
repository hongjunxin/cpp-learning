#include <atomic>
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    std::atomic_uint32_t count(0);

    auto t1 = std::thread([&count]() {
        for (int i = 0; i < 100000; i++) {
            count++;
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    });

    auto t2 = std::thread([&count]() {
        for (int i = 0; i < 100000; i++) {
            count++;
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    });

    auto t3 = std::thread([&count]() {
        for (int i = 0; i < 20; i++) {
            std::cout << count << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        std::cout << "t3 quit" << std::endl;
    });

    t1.join();
    t2.join();
    t3.join();
    assert(count == 200000);

    std::cout << "test done, count: " << count << std::endl;
}