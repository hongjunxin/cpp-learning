#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

int main(int argc, char** argv)
{
    static std::atomic_flag flag {false};
    static std::atomic<int> n{0};

    auto f = [&]()
    {
        auto value = flag.test_and_set();  // TAS 检查原子标志量

        if (value) {
            std::cout << "flag has been set." << std::endl;
        } else {
            std::cout << "set flag by " <<
                std::this_thread::get_id() << std::endl;
        }

        n += 100;

        std::this_thread::sleep_for(
            n.load() * std::chrono::milliseconds(3));
        std::cout << n << std::endl;
    };

    std::thread t1(f);
    std::thread t2(f);

    t1.join();  
    t2.join(); 
}