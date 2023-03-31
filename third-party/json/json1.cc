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
                "f": "g"
            }
        }
    )";
    
    auto json_msg = json::parse(str);
    if (json_msg["a"].is_null()) {
        std::cout << R"(["a"] null)" << std::endl; 
    } else {
        std::cout << R"(["a"] value: )" << json_msg["a"].get<std::string>() << std::endl;
    }

    if (json_msg["c"]["d"].is_null()) {
        std::cout << R"(["c"]["d"] null)" << std::endl;
    } else {
        std::cout << R"(["c"]["d"] value: )" << json_msg["c"]["d"] << std::endl;
    }

    if (json_msg["cc"]["d"].is_null()) {
        std::cout << R"(["cc"]["d"] null)" << std::endl;
    } else {
        std::cout << R"(["cc"]["d"] value: )" << json_msg["cc"]["d"] << std::endl;
    }
}