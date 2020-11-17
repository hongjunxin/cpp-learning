/*
条款 43：学习处理模板化基类内的名称
*/

#include <string>
#include <iostream>

class CompanyA {
public:
    void sendCleartext(const std::string& msg) { 
        std::cout << "CompanyA::sendCleartext" << std::endl;
    }

    void sendEncrypted(const std::string& msg) {
        std::cout << "CompanyA::sendEncrypted" << std::endl;
    }
};

class CompanyB {
public:
    void sendCleartext(const std::string& msg) { 
        std::cout << "CompanyB::sendCleartext" << std::endl;
    }

    void sendEncrypted(const std::string& msg) {
        std::cout << "CompanyB::sendEncrypted" << std::endl;
    }
};

class CompanyZ {
public:
    void sendEncrypted(const std::string& msg) {
        std::cout << "CompanyZ::sendEncrypted" << std::endl;
    }
};


class MsgInfo {  };

template<typename Company>
class MsgSender {
public:
    void sendClear(const MsgInfo& info)
    {
        std::string msg;
        Company c;
        c.sendCleartext(msg);
    }
    void sendSecret(const MsgInfo& info)
    {
        std::string msg;
        Company c;
        c.sendEncrypted(msg);
    }
};

template<>
class MsgSender<CompanyZ> {
public:
    // 不提供 sendClear()
    void sendSecret(const MsgInfo& info)
    {
        std::string msg;
        CompanyZ c;
        c.sendEncrypted(msg);
    }
};

template<typename Company>
class LoggingMsgSender : public MsgSender<Company> {
public:
    void sendClearMsg(const MsgInfo& info)
    {
        /* 编译器怕我们写出 LoggingMsgSender<CompanyZ> 的代码，因为对于特例化的
           MsgSender<CompanyZ>，它根本就没提供 sendClear 方法 */
        //sendClear(info);  // 调用 base class 函数，但无法通过编译

        /* 追加 MsgSender<Company> 前缀，用来告诉编译器，我们这里的设计是要调用
           MsgSender<Company>，而它是会保证提供 sendClear 方法 */
        MsgSender<Company>::sendClear(info);
    }
};

int main(void)
{
    LoggingMsgSender<CompanyA> la;
    la.sendClear(MsgInfo());

    LoggingMsgSender<CompanyZ> lz;
     /* 如果还是执意想对特例化的 MsgSender<CompanyZ> 调用它没定义的方法 sendClearMsg，
        还是会被编译器揪出这个错误的，编译不给过。 */
    // lz.sendClearMsg(MsgInfo()); 

    return 0;
}