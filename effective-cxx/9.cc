/*
条款 09：绝不在构造函数和析构过程中调用 virtual 函数
*/

#include <iostream>

class Transaction {
public:
    Transaction();
    virtual void logTransaction() const = 0;
};
Transaction::Transaction()
{
    // ...
    logTransaction();  // 错误：不要在构造函数中调用 virtual 函数
}

class BuyTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "BuyTransaction::logTransaction() called" <<  std::endl;
    }
    // ...
};

int main(void)
{
    BuyTransaction b;
    return 0;
}