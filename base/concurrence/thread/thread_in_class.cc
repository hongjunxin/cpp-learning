#include <iostream>
#include <thread>

// 线程函数是类的私有成员函数

class MyClass {
public:
    void startWorkerThread()
    {
        std::thread worker(&MyClass::workerThread, this);
        worker.join();
        std::cout << "startWorkerThread finished, cnt " << cnt << std::endl;
    }

private:
    int cnt;

    void workerThread()
    {
        std::cout << "Worker thread started." << std::endl;
        for (int i = 0; i < 5; ++i) {
            std::cout << "Working... " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            ++cnt;
        }
        std::cout << "Worker thread finished." << std::endl;
    }
};

int main(int argc, char **argv) 
{
    MyClass myObject;

    myObject.startWorkerThread();

    std::cout << "Main thread finished." << std::endl;

    return 0;
}
