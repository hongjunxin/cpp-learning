#include "concurrent_map.h"
#include <string>
#include <iostream>
#include <cassert>
#include <thread>
#include <unistd.h>

int main(int argc, char **argv)
{
    int worker_nums, bucket_num;

    if (argc != 3) {
        worker_nums = 16;
        bucket_num = 16;
    } else {
        worker_nums = atoi(argv[1]);
        bucket_num = atoi(argv[2]);
    }

    std::thread workers[worker_nums];
    ConcurrentMap<std::string, int> cmap(bucket_num);

    auto f = [&cmap]() {
        for (int i = 0; i < 100000; i++) {
            auto key = std::to_string(gettid()) + "+" + std::to_string(i);
            auto ret = cmap.insert(std::pair<std::string, int>(key, i));
            if (!ret.second) {
                std::cout << "insert " << key << " failed" << std::endl;
                exit(-1);
            }
            auto found = cmap.find(key);
            assert(found != cmap.end());
            if (found->second != i) {
                std::cout << "found value: " << found->second
                    <<", expection value: " << i
                    << std::endl;
                exit(-1);
            }
        }
    };

    for (int i = 0; i < worker_nums; i++) {
        auto t = std::thread(f);
        workers[i] = std::move(t);
    }

    for (int i = 0; i < worker_nums; i++) {
        workers[i].join();
    }
    auto info = cmap.mapInfo();

    std::cout << "main quit" << std::endl;
    // for (auto &e : info) {
    //     std::cout << e << std::endl;
    // }
}