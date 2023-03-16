#include <iostream>
#include <cassert>
#include <memory>

int main(int argc, char** argv)
{
    std::unique_ptr<int> uptr = std::make_unique<int>(42);
    assert(uptr != nullptr && *uptr == 42);

    std::unique_ptr<int> uptr2 = std::move(uptr);
    assert(uptr == nullptr && uptr2 != nullptr && *uptr2 == 42);
    
    return 0;
 }