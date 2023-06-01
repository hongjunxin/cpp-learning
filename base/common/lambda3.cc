#include <thread>
#include <string>
#include <iostream>
#include <memory>


// lambda 捕获共享指针时，得注意共享指针的生命周期。

// foo 的 str 是值传入，所以 foo 退出时，共享指针 str 被回收，
// 然而 task 捕获的是 str 的引用，所以导致在 task 中执行 *str 会 coredump。
// task 应改为捕获 str 的值，而不是引用，但这种情况下又得考虑 task 有可能一直
// 持有共享指针，导致 str 内部维护的对象一直无法释放，所以得根据情况考虑是否改为
// 捕获弱指针。
void foo(std::shared_ptr<std::string> str)
{
    std::thread task([&str]() {
        std::cout << "foo str addr: " << &str << std::endl; // 0x7ffd2e34b9a0
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "foo print " << *str << std::endl;  // *str coredump
    });
    task.detach();
    std::cout << "foo quit" << std::endl;
}

// bar 虽然不会 coredump，是因为 main 的 str 生命周期大于 task
void bar(std::shared_ptr<std::string> &str)
{
    std::thread task([&str]() {
        std::cout << "bar str addr: " << &str << std::endl;  // 0x7ffd2e34b980
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "bar print " << *str << std::endl;
    });
    task.detach();
    std::cout << "bar quit" << std::endl;
}

int main(int argc, char **argv)
{
    auto str = std::make_shared<std::string>("hi");
    std::cout << "main str addr: " << &str << std::endl;  // 0x7ffd2e34b980
    foo(str);
    bar(str);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}