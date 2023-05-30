#include <string>

class Name {
public:
    void setName(const std::string &s) const;
    void show() {}
private:
    char *_name;
};
 
void Name::setName(const std::string &s) const {
    //_name = s.c_str();      // 错误！不能修改m_sName;
 
    for (int i = 0; i < s.size(); ++i) 
        _name[i] = s[i];    // 不好的风格，但不是错误的
    
    //show();  /* 错误，const 成员函数中不能调用非 const 成员函数 */
}

int main(void)
{}
