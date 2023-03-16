#include <cassert>
#include <string>

int main(int argc, char** argv)
{
    assert(std::stoi("42") == 42);
    assert(std::stol("31") == 31L);
    assert(std::stod("2.0") == 2.0);
    assert(std::to_string(12) == "12");
}