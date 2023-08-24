#pragma once

#include <string>

/*
    继承带参数的基类构造器
*/
class Handler {
public:
    Handler() = default;
    Handler(std::string name) : name_(name) {}
    virtual ~Handler() = default;
    virtual void handle() {};

protected:
    std::string name_;
};

class TokenHandler : public Handler {
public:
    // 这里如果不指定 ": Handler(name)"，则编译器会默认继承基类的默认构造器，即 Handler()
    TokenHandler(std::string name) : Handler(name) {}
    ~TokenHandler() = default;
    void handle() override;
};
