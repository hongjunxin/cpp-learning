#include <iostream>
#include "worker.h"

int main(int argc, char** argv)
{
    Worker worker_proc;

    if (worker_proc.run() != Worker::State::kRunSuccess) {
        std::cout << "worker run failed" << std::endl;
    }

    auto ret = worker_proc.execShell("touch file.txt");
    std::cout << "ret: " << ret << std::endl;
    return 0;
}