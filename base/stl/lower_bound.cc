#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

int main(int argc, char** argv)
{
    std::vector<int> v = {1,2,3,4,7,8,9};
    auto pos = std::lower_bound(          // 找到第一个 >=7 的位置
        std::cbegin(v), std::cend(v), 7
    );  
    auto found = (pos != std::cend(v)) && (*pos == 7);
    assert(found);

    pos = std::lower_bound(               // 找到第一个 >=5 的位置
        std::cbegin(v), std::cend(v), 5
    );  
    assert(pos != std::cend(v));
    std::cout << *pos << std::endl;
}