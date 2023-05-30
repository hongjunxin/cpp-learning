#include <atomic>
#include <memory>
#include <thread>
#include <iostream>
#include <chrono>
#include <stdlib.h>

class A {
public:
    A(int value = 0) : mValue(value) {}
    ~A() = default;

    int getValue(void) {
        return mValue;
    }
    int mAddress;
private:
    int mValue;
};

int main(int argc, char **argv)
{
    std::shared_ptr<A> ret;

    auto task1 = std::thread([&ret](){
        srandom(20);
        std::this_thread::sleep_for(std::chrono::microseconds(rand()%100));
        auto a = std::make_shared<A>(10);
        std::cout << "a->mAddress: " << &a->mAddress << std::endl;
        std::atomic_store(&ret, a);  // let ret point to a
    });

    auto task2 = std::thread([&ret](){
        srandom(10);
        std::this_thread::sleep_for(std::chrono::microseconds(random()%100));
        auto b = std::make_shared<A>(20);
        std::cout << "b->mAddress: " << &b->mAddress << std::endl;
        std::atomic_store(&ret, b);  // let ret point to b
    });

    task1.join();
    task2.join();

    auto alia = std::atomic_load(&ret);  // not sure alia points to a or b
    std::cout << "alia->mAddress: " << &alia->mAddress << std::endl;
    std::cout << alia->getValue() << std::endl;
}