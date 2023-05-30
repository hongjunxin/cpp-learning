#include <curl/curl.h>
#include <cassert>
#include <iostream>

size_t write_callback(char* , size_t , size_t , void* );

int main(int argc, char** argv) 
{
    auto curl = curl_easy_init();
    assert(curl);

    curl_easy_setopt(curl, CURLOPT_URL, "http://nginx.org");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,        // 设置回调函数
        (decltype(&write_callback))                      // decltype获取函数指针类型，显式转换
        [](char *ptr, size_t size, size_t nmemb, void *userdata) {
            std::cout << "size = " << size * nmemb << std::endl;   // 简单的处理
            return size * nmemb;                                   // 返回接收的字节数
        }
    );

    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
}
