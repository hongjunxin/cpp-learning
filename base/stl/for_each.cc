#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv)
{
    std::vector<int> v = {1,2,3,4,5,6};
    std::for_each(v.cbegin(), v.cend(),
        [](const auto& e) {
            std::cout << e << " ";
        });
    std::cout << std::endl;
}