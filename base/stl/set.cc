#include <set>
#include <iostream>

int main(int argc, char** argv)
{
    // set 默认是升序排序。本例子展示使用降序排序
    auto comp = [](const auto& a, const auto& b) {
        return a > b;
    };
    std::set<int, decltype(comp)> gs(comp);
    gs.emplace(2);
    gs.emplace(1);
    gs.emplace(4);

    for (const auto& a : gs) {
        std::cout << a << " ";
    }
    std::cout << std::endl;
}