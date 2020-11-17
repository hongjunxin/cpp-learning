/*
条款 45：运用成员函数模板接受所有兼容类型
*/

#include <iostream>

class Top {};

class Middle : public Top {};

class Bottom {};

template<typename T>
class SmartPtr {
public:
    SmartPtr() {}
    SmartPtr(const SmartPtr& other) : heldPtr(other.get()) {
        std::cout << "SmartPtr(const SmartPtr& other)" << std::endl;
    }

    template<typename U>
    SmartPtr(const SmartPtr<U>& other)  // 泛化 copy 构造函数， U 是 T 的子类
        : heldPtr(other.get())
    {
        std::cout << "SmartPtr(const SmartPtr<U>& other)" << std::endl;
    }

    template<typename U>
    SmartPtr& operator=(const SmartPtr<U>& other) {
        heldPtr = other.get();
        return *this;
    }

    T* get() const { return heldPtr; }
private:
    T* heldPtr;
};

int main(void)
{
    SmartPtr<Middle> pMiddle;
    SmartPtr<Middle> pMiddle2(pMiddle);

    SmartPtr<Top> pTop(pMiddle);  // Middle* 指针可以赋值给 Top*
    
    SmartPtr<Bottom> pBottom;
    /* 编译不过。 error: cannot convert ‘Bottom*’ to ‘Top*’ in initialization */
    // SmartPtr<Top> pTopBad(pBottom);

    return 0;
}
