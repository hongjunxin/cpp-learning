// g++ --std=c++11 -fsanitize=address sanitizer.cc 
#include <memory>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(int argc, char** argv)
{
    auto ptr = new std::string("hi");
    std::cout << *ptr << std::endl;
    std::this_thread::sleep_for(
        std::chrono::seconds(1));
    // delete(ptr); // -fsanitize=address 可以检查出可能存在的内存泄漏
    return 0;
}