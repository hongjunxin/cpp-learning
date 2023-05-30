#include <memory>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    auto ori = std::make_shared<std::string>("hi");
    std::shared_ptr<std::string> ori2 = nullptr;

    std::weak_ptr<std::string> w1(ori);
    std::weak_ptr<std::string> w2(ori2);

    if (auto w = w1.lock()) {
        std::cout << "w1: " << *w << std::endl;
    }

    if (w2.lock() == nullptr) {
        std::cout << "w2 null" << std::endl;
    } else {
        std::cout << "w2: " << *(w2.lock()) << std::endl;
    }

    std::weak_ptr<std::string> w3 = w1;
    std::cout << "w3: " << *(w3.lock()) << std::endl;
}