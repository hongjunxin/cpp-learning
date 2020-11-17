/*
条款 12：复制对象时不要忘记其每一个成分

当我们承担起为 derived class 编写 copying 函数的责任时，必须小心地复制其 base class 成分，
但这些成分通常是 private 的，所以我们应该让 derived class 的 copying 函数调用相应的 base class 函数。
*/

#include <iostream>

class Customer {
public:
    Customer() {
        std::cout << "Customer constructor" << std::endl;
    }

    Customer(const Customer& other) {
        std::cout << "Customer copy constructor" << std::endl;
    }

    Customer& operator=(const Customer& other) {
        std::cout << "Customer operator= constructor" << std::endl;
        return *this;
    }
};

class PriorityCustomer : public Customer {
public:
    PriorityCustomer(int _priority = 0) : priority(_priority) {
        std::cout << "PriorityCustomer constructor" << std::endl;
    }

    PriorityCustomer(const PriorityCustomer& rhs)
        : Customer(rhs), priority(rhs.priority)
    {
        std::cout << "PriorityCustomer copy constructor" << std::endl;
    }

    PriorityCustomer& operator=(const PriorityCustomer& rhs)
    {
        Customer::operator=(rhs);  // 对 base class 成分进行赋值操作
        priority = rhs.priority;
        
        std::cout << "PriorityCustomer operator= constructor" << std::endl;
        return *this;
    }
private:
    int priority;
};

int main(void)
{
    PriorityCustomer c;  /* 自动调用基类的构造器 */

    std::cout << std::endl;

    PriorityCustomer d(c);

    std::cout << std::endl;

    PriorityCustomer e = c;  /* 调用了 copy constructor，而不是赋值操作符 operator= */

    std::cout << std::endl;

    e = d;  /* 这个才调用了赋值操作符 operator= */
    return 0;
}
