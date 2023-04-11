#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>
#include <thread>

void thread_task(int n) {
    std::this_thread::sleep_for(std::chrono::seconds(n));
    std::cout << "hello thread "
        << std::this_thread::get_id()
        << " paused " << n << " seconds" << std::endl;
}

int main(int argc, const char *argv[])
{
    std::thread threads[5];
    std::cout << "Spawning 5 threads...\n";
    for (int i = 0; i < 5; i++) {
        // 换成 std::ref(i) 会导致 thread_task 中的 n 不是 0-4
        threads[i] = std::thread(thread_task, i);
    }
    std::cout << "Done spawning threads! Now wait for them to join\n";
    for (auto& t: threads) {
        t.join();
    }
    std::cout << "All threads joined.\n";

    return EXIT_SUCCESS;
}