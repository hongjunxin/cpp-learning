#include "../httplib.h"
#include <iostream>
#include <string>

const std::string unix_path = "/tmp/uslibsrv.sock";

int main(int argc, char **argv)
{
    httplib::Client cli(unix_path);
    cli.set_address_family(AF_UNIX);

    auto body = R"({"reqid":"1234567890ABCDEF", "req":"token.query", "token":"286311BA64D5992D000000017A98E854"})";

    const auto &result = cli.Post("/api/v1/uslibsrv", body, "text/plain");
    
    std::cout << "error: " << result.error()
              << ", value: " << result.value().body
              << std::endl;
    return 0;
}
