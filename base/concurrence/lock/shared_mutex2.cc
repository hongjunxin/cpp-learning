#include <mutex>
#include <shared_mutex>
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char **argv)
{
    std::shared_mutex mtx;

    auto reader = [&](std::string tag) {
        mtx.lock_shared();  // shared lock
        std::cout << "reader " << tag << " got lock" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "reader " << tag << " quit" << std::endl;
        mtx.unlock_shared();
    };

    auto writer = [&](std::string tag) {
        mtx.lock();  // exclusive lock
        std::cout << "writer " << tag << " got lock" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        std::cout << "writer " << tag << " quit" << std::endl;
        mtx.unlock();
    };

    std::thread w1(writer, "WA");
    std::thread r1(reader, "RA");
    std::thread r2(reader, "RB");
    std::thread w2(writer, "WB");
    std::thread r3(reader, "RC");

    r1.join();
    r2.join();
    r3.join();
    w1.join();
    w2.join();
}
