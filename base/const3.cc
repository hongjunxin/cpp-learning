#include <iostream>
#include <string>

class People {
public:
    People() = default;
    ~People() = default;

    std::string show();
    std::string show() const;
};

std::string People::show() {
    return "Not-Const show()";
}

std::string People::show() const {
    return "Const show()";
}

int main(int argc, char** argv)
{
    People p;
    const People p2;
    std::cout << p.show() << std::endl;
    std::cout << p2.show() << std::endl;

    return 0;
}