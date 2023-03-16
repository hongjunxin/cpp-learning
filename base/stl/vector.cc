#include <vector>
#include <iostream>

int main(int argc, char **argv)
{
    std::vector<int> nums;
    nums.reserve(100);

    for (int i = 0; i < 3; ++i) {
        nums.emplace_back(i);
    }

    for (auto i = nums.begin(); i != nums.end(); ++i) {
        std::cout << *i << std::endl;
    }

    for (const auto& i : nums) {
        std::cout << i << std::endl;
    }
    std::cout << "capacity: " << nums.capacity() 
        << ", size: " << nums.size()
        << std::endl;
}