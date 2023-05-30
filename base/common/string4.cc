#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <unordered_set>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void stringSplit(const std::string& str, const std::string& token, std::vector<std::string>& res)
{
    std::regex reg(token);
    std::sregex_token_iterator pos(str.begin(), str.end(), reg, -1);
    decltype(pos) end;
    for (; pos != end; ++pos) {
        res.push_back(pos->str());
    }
}

int main(int argc, char** argv)
{
    auto str = R"(
        {
            "shellWhiteList": "settings,echo"
        }
    )";
    auto json_str = json::parse(str);
    auto shell_white_list = json_str["shellWhiteList"].get<std::string>();
    std::vector<std::string> res;
    stringSplit(shell_white_list, ",", res);
    for (auto& e : res) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    std::unordered_set<std::string> uset_res(res.begin(), res.end());
    for (auto& e : uset_res) {
        std::cout << e << " ";
    }
    std::cout << std::endl;

    if (uset_res.find("rm") == uset_res.end()) {
        std::cout << "rm not found" << std::endl;
    }
    if (uset_res.find("echo") != uset_res.end()) {
        std::cout << "echo found" << std::endl;
    }
}