#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>

int main(int argc, char** argv)
{
    std::vector<int> v = {1,9,11,3,5,7};  // vector容器

    auto pos = std::find(                 // 查找算法，找到第一个出现的位置
        std::begin(v), std::end(v), 3
    );  
    assert(pos != std::end(v));         // 与end()比较才能知道是否找到

    pos = std::find_if(            // 查找算法，用lambda判断条件
        std::begin(v), std::end(v),
        [](const auto& x) {              // 定义一个lambda表达式
            return x % 2 == 0;    // 判断是否偶数
        }
    );  
    assert(pos == std::end(v));        // 与end()比较才能知道是否找到

    std::array<int, 2> arr = {3,5};    // array容器
    pos = std::find_first_of(      // 查找一个子区间
        std::begin(v), std::end(v),
        std::begin(arr), std::end(arr)
    );  
    assert(pos != std::end(v));       // 与end()比较才能知道是否找到 
}