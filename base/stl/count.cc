#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv)
{
    std::vector<int> v = {1,2,3,4,5,6};
    int ret = std::count(v.begin(), v.end(), 1);
    std::cout << ret <<  std::endl;

    ret = std::count_if(v.begin(), v.end(),
        [](const int& a) {
            return a > 2;
        });
    std::cout << ret << std::endl;
}