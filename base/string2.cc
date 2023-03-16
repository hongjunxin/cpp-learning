#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    auto str1 = R"(char""'')";    // 原样输出：char""''
    auto str2 = R"(\r\n\t\")";    // 原样输出：\r\n\t\"
    auto str3 = R"(\\\$)";        // 原样输出：\\\$
    auto str4 = "\\\\\\$";         // 转义后输出：\\\$    
    auto str5 = R"==(R"(\\\$)")==";

    std::cout << str1 << std::endl;
    std::cout << str2 << std::endl;
    std::cout << str3 << std::endl;
    std::cout << str4 << std::endl;
    std::cout << str5 << std::endl;
}