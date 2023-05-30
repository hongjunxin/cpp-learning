#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
    auto str = R"(
        {
            "a": "b",
            "c": {
                "d": "e",
                "f": "g",
                "h": 1
            }
        
    )";
    
    json json_msg;
    try {
        json_msg = json::parse(str);
        json_msg["a"] = "b-new";
        auto s = json_msg.dump();
        std::cout << s << std::endl;
    } catch (const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
        if (json_msg["a"].is_null()) {
            std::cout << "json[a] is null" << std::endl;
        }
    }
}