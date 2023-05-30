#include <map>
#include <unordered_map>
#include <iostream>

std::map<int, int> map;
std::unordered_map<int, int> umap;

static void fill(int value)
{
    map[value] = value;
    umap[value] = value;
}

int main(void)
{
    for (int i = 0; i < 20; ++i)
        fill(i);
    
    /* 输出：0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 */
    for (auto e : map)
        std::cout << e.first << " ";
    std::cout << std::endl;

    /* 输出：19 18 17 16 15 14 13 12 11 10 3 2 1 0 4 5 6 7 8 9 */
    for (auto e : umap)
        std:: cout << e.first << " ";
    std::cout << std::endl;

    return 0;
}
