#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>

class A {
public:
    A(std::mutex* m) : m_(m) 
    { 
        m_->lock();
        std::cout << "A()" << std::endl; 
    }

    ~A() 
    { 
        std::cout << "enter ~A() thread_id: " << std::this_thread::get_id() << std::endl; 
        // 模拟资源占用，析构函数中可能还没来得及释放资源，
        // 其他线程会等待该资源释放
        std::this_thread::sleep_for(std::chrono::seconds(3));
        m_->unlock();
        std::cout << "exit ~A()" << std::endl;
    }
private:
    std::mutex* m_;
};


int main(int argc, char** argv)
{
    std::mutex mtx;
    std::shared_ptr<A> sa = std::make_shared<A>(&mtx);
    sa.reset();
    auto t = std::thread([&mtx]() {
        mtx.lock();
        std::cout << "test get lock" << std::endl;
    });
    std::cout << "last output?" << std::endl;
    t.join();
}
