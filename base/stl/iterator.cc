#include <array>
#include <cassert>
#include <algorithm>

int main(int argc, char** argv)
{
    std::array<int, 5> arr = {0,1,2,3,4};  // array静态数组容器

    auto b = std::begin(arr);          // 全局函数获取迭代器，首端
    auto e = std::end(arr);            // 全局函数获取迭代器，末端

    assert(std::distance(b, e) == 5);  // 迭代器的距离

    auto p = std::next(b);              // 获取“下一个”位置
    assert(std::distance(b, p) == 1);    // 迭代器的距离
    assert(std::distance(p, b) == -1);  // 反向计算迭代器的距离

    std::advance(p, 2);                // 迭代器前进两个位置，指向元素'3'
    assert(*p == 3);
    assert(p == std::prev(e, 2));     // 是末端迭代器的前两个位置
}