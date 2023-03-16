#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    int i = 0;
    noexcept(i == 0);

    int a = 2;
    noexcept(a != 2);
    return 0;
}
