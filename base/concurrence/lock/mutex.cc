#include <mutex>
#include <thread>
#include <iostream>
#include <chrono>

void sayhi(std::string tag)
{
    static std::mutex mtx;

    mtx.lock();
    std::cout << tag << " got mtx" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    mtx.unlock();
    std::cout << tag << " release mtx" << std::endl;
}

int main(int argc, char **argv)
{
    auto f1 = std::thread([](std::string tag) {
        sayhi(tag);
    }, "task 1");

    auto f2 = std::thread([](std::string tag) {
        sayhi(tag);
    }, "task 2");

    f1.join();
    f2.join();
}
