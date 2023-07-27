#include <iostream>

class PeerConnection {
public:
    // 加了 class 表示为限定作用域的枚举类型，特点：
    // 不支持类型隐式转换
    // 成员类型默认为 int
    enum class State : int {
        New,
        Connecting,
        Connected,
        Closing,
        Closed
    };
};

int main(int argc, char **argv)
{
    int s;
    auto es = PeerConnection::State::New;

    // 限定作用域的枚举类型，只能进行显式的类型转换
    s = static_cast<int>(es);
    std::cout << "s: " << s << std::endl;
}