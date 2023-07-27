#include <iostream>
#include <typeinfo>
#include <cassert>

class Base {
public:
    virtual void whoami();
private:
    int a;
};

void Base::whoami()
{
    std::cout << "I am Base" << std::endl;
}

class Derived : public Base {
public:
    virtual void whoami();
private:
    int b;
};

void Derived::whoami()
{
    std::cout << "I am Devired" << std::endl;
}

int main(int argc, char **argv)
{
    Derived *dp = new Derived;
    Base *bp = dp;

    dp->whoami();
    bp->whoami();

    // 如果 Base 不存在虚函数，则 typeid(*bp) 得到的是 Base，而不是 Derived
    if (typeid(*bp) == typeid(*dp)) {
        const std::type_info &info = typeid(*bp);
        std::cout << "*bp and *dp point to the same type"
                  << ", info name: " << info.name()
                  << ", info hash code: " << info.hash_code()
                  << std::endl;
    } else {
        std::cout << "*bp and *dp not point to the same type" << std::endl;
        const auto &binfo = typeid(*bp);
        const auto &dinfo = typeid(*dp);
        std::cout << "*bp info name: " << binfo.name() 
                  << ", info hash code: " << binfo.hash_code() << std::endl;
        std::cout << "*dp info name: " << dinfo.name() 
                  << ", info hash code: " << dinfo.hash_code() << std::endl;
    }
}
