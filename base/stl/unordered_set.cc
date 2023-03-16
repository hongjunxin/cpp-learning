#include <iostream>
#include <unordered_set>

class Point {
public:
    Point(int _x) : x(_x) {}
    int x;
};

std::ostream& operator<<(std::ostream& os, const Point& p)
{
    std::cout << p.x;
    return os;
}

bool operator==(const Point& a, const Point& b)
{
    return a.x == b.x;
}

int main(int argc, char **argv)
{
    // c++14 以上
    auto hasher = [](const auto& p) {
        return std::hash<int>()(p.x);
    };
    std::unordered_set<Point, decltype(hasher)> s(10, hasher);
    s.emplace(7);
    s.emplace(3);

    for (auto& e : s) {
        std::cout << e << std::endl;
    }
}