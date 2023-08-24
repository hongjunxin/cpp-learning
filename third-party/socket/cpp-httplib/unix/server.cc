#define CPPHTTPLIB_USE_POLL

#include "../httplib.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <errno.h>
#include <stdio.h>

using namespace httplib;

const std::string unix_path = "/tmp/uslibsrv.socket";

int main(int argc, char **argv)
{
    httplib::Server srv;

    if (!srv.is_valid()) {
        printf("server has an error...\n");
        return -1;
    }

    srv.Post("/api/v1/uslibsrv",
        [&](const httplib::Request &req, httplib::Response &res) {
        
        // std::cout << "Content-Type: " << req.get_header_value("Content-Type") << std::endl;
        // std::cout << "Content-Length: " << req.get_header_value("Content-Length") << std::endl;
        std::cout << "body: " << req.body << std::endl;

        // todo: handle body
        auto data = std::string(R"({"result":"fail", "errno":12345, "reqid":"1234567890ABCDEF"})");
        res.set_content(data, "text/plain");
    });

    srv.Post("/api/v2/uslibsrv",
        [&](const httplib::Request &req, httplib::Response &res, 
            const httplib::ContentReader &content_reader) {
        
        // todo: use in which cases?
        std::string body;
        content_reader([&](const char *data, size_t data_length) {
            body.append(data, data_length);
            // std::cout << "body: " << body << std::endl;
            return true;
        });

        // todo: use in which cases?
        auto data = new std::string(R"({"result":"fail", "errno":12345, "reqid":"1234567890ABCDEF"})");
        res.set_content_provider(
            data->size(), // Content length
            "text/plain", // Content type
            [&, data](size_t offset, size_t length, DataSink &sink) {
                const auto &d = *data;
                sink.write(&d[offset], length);
                return true; // return 'false' if you want to cancel the process.
            },
            [data](bool success) {
                delete data;
            });
    });

    if (remove(unix_path.c_str()) == -1 && errno != ENOENT) {
        std::cout << "remove " << unix_path << " failed, error: "
            << strerror(errno) << std::endl;
        return -1;
    }

    auto ret = srv.set_address_family(AF_UNIX).listen(unix_path, 80);
    if (!ret) {
        std::cout << "listen failed, error: " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}
