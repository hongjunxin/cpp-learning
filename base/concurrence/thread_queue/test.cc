#include <iostream>
#include <chrono>
#include <sys/prctl.h>
#include "dispatchqueue.h"

int main(int argc, char** argv)
{
    prctl(PR_SET_NAME, "queue_main");

    DispatchQueue work_queue("queue_work");
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
}