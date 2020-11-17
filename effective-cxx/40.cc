/*
条款 40：谨慎适当使用多重继承
*/

#include <iostream>
#include <string>

class IPerson {
public:
    virtual ~IPerson() {}
    virtual std::string name() const = 0;
    virtual std::string birthDate() const = 0;
};

class PersonInfo {
public:
    explicit PersonInfo(int pid = 100) {}
    virtual ~PersonInfo() {}
    virtual const char* theName() const { return "PersonInfo::theName"; }
    virtual const char* theBirthDate() const { return "PersonInfo::theBirthDate"; }
    virtual const char* valueDelimOpen() const { return "PersonInfo::valueDelimOpen"; }
    virtual const char* valueDelimClose() const { return "PersonInfo::valueDelimClose"; }  
private:
    int pid;
};

/* 多重继承的确有正当用途，其中一个情景涉及“public 继承某个
   interface class”和“private 继承某个协助实现的 class”的组合 */
class CPerson : public IPerson, private PersonInfo {
public:
    explicit CPerson(int pid = 100) : PersonInfo(pid) { }

    virtual std::string name() const { 
        std::string ret = PersonInfo::theName();
        std::cout << ret << std::endl;
        return ret; 
    }

    virtual std::string birthDate() const { 
        std::string ret = PersonInfo::theBirthDate();
        std::cout << ret << std::endl;
        return ret; 
    }

private:
    const char* valueDelimOpen() const { return ""; }
    const char* valueDelimClose() const { return ""; }
};

int main(void)
{
    CPerson p;
    p.birthDate();
    p.name();

    return 0;
}