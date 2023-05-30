#include "concurrent_map.h"
#include <string>
#include <iostream>
#include <cassert>

int main(int argc, char **argv)
{
    ConcurrentMap<int, std::string> cmap;
    auto ret = cmap.insert(std::make_pair<int, std::string>(1, "a"));
    assert(ret.second);

    auto found = cmap.find(1);
    assert(found != cmap.end());

    std::cout << "found: " << found->first 
        << ", value: " << found->second << std::endl;
    
    cmap.erase(1);
    assert(cmap.find(1) == cmap.end());
}