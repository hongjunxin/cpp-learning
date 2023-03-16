#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv)
{
    std::vector<int> v = {3,5,1,7,10,99,42};  // vector容器
    std::sort(std::begin(v), std::end(v));        // 快速排序

    auto found = std::binary_search(         // 二分查找，只能确定元素是否存在
        std::cbegin(v), std::cend(v), 7
    );
    std::cout << "found: " << found << std::endl;
}