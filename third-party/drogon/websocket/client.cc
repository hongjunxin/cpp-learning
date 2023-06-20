#include <drogon/WebSocketClient.h>
#include <drogon/HttpAppFramework.h>

#include <iostream>
#include <optional>
#include <unistd.h>

using namespace drogon;
using namespace std::chrono_literals;

namespace {

std::string server = "ws://192.168.10.72:18000";

void parse_arg(int argc, char * argv[])
{
    int ch;
    while ((ch = getopt(argc, argv, "s:")) != -1)
    {
        switch (ch) {
        case 's':
            server = std::string(optarg);
            break;
        case '?':
            printf("Unknown option: %c\n\n",(char)optopt);
            break;
        }
    }
}    
}  // namespace
// 编译器会为匿名 namespace 生成一个唯一的名字，并且生成 using 指令来是使用该匿名空间
// 匿名 namespace 用于替代 static
// static 不能修饰 class

int main(int argc, char *argv[])
{
    parse_arg(argc, argv);
    auto wsPtr = WebSocketClient::newWebSocketClient(server);
    auto req = HttpRequest::newHttpRequest();
    req->setPath("/signal?client_id=123456");

    wsPtr->setMessageHandler([](const std::string &message,
                                const WebSocketClientPtr &,
                                const WebSocketMessageType &type) {
        std::string messageType = "Unknown";
        if (type == WebSocketMessageType::Text)
            messageType = "text";
        else if (type == WebSocketMessageType::Pong)
            messageType = "pong";
        else if (type == WebSocketMessageType::Ping)
            messageType = "ping";
        else if (type == WebSocketMessageType::Binary)
            messageType = "binary";
        else if (type == WebSocketMessageType::Close)
            messageType = "Close";

        LOG_INFO << "new message (" << messageType << "): " << message;
    });

    wsPtr->setConnectionClosedHandler([](const WebSocketClientPtr &wsPtr) {
        LOG_INFO << "WebSocket connection closed!";
        wsPtr->stop();
        app().quit();
    });

    LOG_INFO << "Connecting to WebSocket at " << server;
    wsPtr->connectToServer(
        req,
        [](ReqResult r,
           const HttpResponsePtr &,
           const WebSocketClientPtr &wsPtr) {
            if (r != ReqResult::Ok)
            {
                LOG_ERROR << "Failed to establish WebSocket connection!, ret=" << r;
                wsPtr->stop();
                return;
            }
            LOG_INFO << "WebSocket connected!";
            wsPtr->getConnection()->setPingMessage("", 10s);
            wsPtr->getConnection()->send("{\"type\":\"ping\"}");
        });

    // Quit the application after 60 seconds
    app().getLoop()->runAfter(60, []() { app().quit(); });

    app().setLogLevel(trantor::Logger::kDebug);
    app().run();
    LOG_INFO << "bye!";
    return 0;
}

