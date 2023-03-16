#include <iostream>
#include <algorithm>
#include <vector>

int main(int argc, char** argv)
{
    auto print = [](const int& e) {
        std::cout << e << " ";
    };

    // top3。前三名会进行排序
    std::vector<int> v = {1,7,2,9,0,8,10};
    std::partial_sort(
        std::begin(v), std::next(std::begin(v), 3), std::end(v));  // 取前3名
    for_each(v.begin(), v.end(), print);
    std::cout << std::endl;

    // best3。前三名不会进行排序，只是选出来
    v = {1,7,2,9,0,8,10};
    std::nth_element(
        std::begin(v), std::next(std::begin(v), 3), std::end(v));  // 最好的3个
    for_each(v.begin(), v.end(), print);
    std::cout << std::endl;

    // Median
    auto mid_iter =                            // 中位数的位置
        std::next(std::begin(v), v.size()/2);
    std::nth_element(std::begin(v), mid_iter, std::end(v));// 排序得到中位数
    std::cout << "median is " << *mid_iter << std::endl;
        
    // partition
    auto pos = std::partition(                // 找出所有大于9的数
        std::begin(v), std::end(v),
        [](const auto& x)                    // 定义一个lambda表达式
        {
            return x > 9;
        }
    ); 
    for_each(std::begin(v), pos, print);         // 输出分组后的数据
    std::cout << std::endl;

    // min/max
    auto value = std::minmax_element(        //找出第一名和倒数第一
        std::cbegin(v), std::cend(v)
    );
    std::cout << "first=" << *value.first 
        << ", second=" << *value.second << std::endl;
}