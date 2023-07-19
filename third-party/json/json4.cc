#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv)
{
    auto str = R"(
{
    "info": {
        "result": true,
        "access_infos": [
            {
                "type":"10000",
                "listen_port":10000,
                "access_port":10005,
                "public_ip":"36.139.86.155"
            },
            {
                "type":"10001",
                "listen_port":10001,
                "access_port":10006,
                "public_ip":"36.139.86.155"
            }
        ]
    }
}
    )";
    
    json json_msg;
    try {
        json_msg = json::parse(str);
        std::cout << "result: " << json_msg["info"]["result"].get<bool>() << std::endl; 
        auto infos = json_msg["info"]["access_infos"];
        if (!infos.is_null() && infos.is_array()) {
            for (auto &e : infos) {
                std::cout << e << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}