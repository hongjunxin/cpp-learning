#include <thread>
#include <iostream>
#include <chrono>

int main(int argc, char **argv)
{
    std::thread task([](){
        std::cout << "task running" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "task thread exit" << std::endl;
    });

    // must call detach() if without call join()
    task.detach();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "main exit" << std::endl;

    // child thread exit by force as main thread exit
    return 0;
}