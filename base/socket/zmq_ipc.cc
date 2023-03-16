// g++ zmq.cpp -std=c++14 -lzmq -lpthread -o a.out
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <cassert>

static const auto thread_num = 1;
static zmq::context_t context(thread_num);

int main(int argc, char** argv)
{
    const std::string addr = "ipc:///dev/shm/zmq.sock";  // 通信地址

    auto receiver = [=]()
    {
        auto sock = zmq::socket_t(context, ZMQ_PULL);    // 创建ZMQ套接字，拉数据

        sock.bind(addr);
        std::cout << "receicer bind success" << std::endl;
        assert(sock.connected());

        zmq::message_t msg;
        sock.recv(&msg);

        std::string s = {msg.data<char>(), msg.size()};
        std::cout << s << std::endl;
    };

    auto sender = [=]()
    {
        auto sock = zmq::socket_t(context, ZMQ_PUSH);     // 创建ZMQ套接字，推数据

        sock.connect(addr);                               // 连接到对端
        std::cout << "sender connect success" << std::endl;
        assert(sock.connected());

        std::string s = "hello zmq";
        sock.send(s.data(), s.size());
        std::cout << "sender send done" << std::endl;
    };
    
    sender();
    receiver();
}