#include <iostream>
#include <string>
#include <optional>
#include <errno.h>
#include <sys/wait.h>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

struct ShellMessage {
    std::string cmd;
    std::optional<std::string> parameter = std::nullopt;
};

std::string runCmd(std::string cmd)
{
    std::cout << "cmd: " << cmd << std::endl;
    int status = system(cmd.c_str());
    if (WIFEXITED(status)) {
        return strerror(WEXITSTATUS(status));
    } else {
        return "system() call terminated abnormally";
    }
}

int main(int argc, char** argv)
{
    auto str = R"(
        {
            "type": 3001,
            "message" : {
                "cmd" : "touch",
                "parameter": "file.txt && touch file2.txt"
            }
        }
    )";
    
    try {
        auto msg = json::parse(str);
        ShellMessage shell_msg;
		if (msg["message"]["cmd"].is_null() || 
				(shell_msg.cmd = msg["message"]["cmd"].get<std::string>()) == "") {
			throw std::runtime_error("CONTROL_TYPE_SHELL message.cmd empty");
		}
		if (!msg["message"]["parameter"].is_null() &&
					msg["message"]["parameter"].get<std::string>().length() != 0) {
			shell_msg.parameter = msg["message"]["parameter"].get<std::string>();
		}
        if (shell_msg.parameter != std::nullopt &&
                shell_msg.parameter.value().find("&&") != std::string::npos) {
            std::cout << "&& in parameter, forbidden" << std::endl;
            return -1;
        }
        std::ostringstream oss_cmd;
        oss_cmd << shell_msg.cmd;
        if (shell_msg.parameter != std::nullopt) {
            oss_cmd << " " << shell_msg.parameter.value();
        }
        oss_cmd << " > /dev/null 2>&1";
        auto ret = runCmd(oss_cmd.str());
        std::cout << "ret: " << ret << std::endl;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}