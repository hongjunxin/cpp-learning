#include <thread>
#include <iostream>
#include <chrono>

int main(int argc, char **argv)
{
    // std::thread can be a local variable
    {
        std::thread task([](){
            for (int i = 0; i < 3; ++i) {
                std::cout << "index: " << i << std::endl;
            }
            std::cout << "task quit" << std::endl;
        }); 
        task.detach();
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "main quit" << std::endl;
}