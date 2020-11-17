/*
条款 49：了解 new-handler 的行为
*/

#include <new>
#include <iostream>
#include <string>

class NewHandlerHolder {
public:
    explicit NewHandlerHolder(std::new_handler nh) : handler(nh) { }

    ~NewHandlerHolder() { 
        std::set_new_handler(handler); 
    }

private:
    std::new_handler handler;
    NewHandlerHolder(const NewHandlerHolder&);  // 阻止 copying
    NewHandlerHolder& operator=(const NewHandlerHolder&);
};

template<typename T>
class NewHandlerSupport {
public:
    static std::new_handler set_new_handler(std::new_handler p) throw();
    static void* operator new(std::size_t size) throw(std::bad_alloc);  /* 重写 new */

private:
    static std::new_handler currentHandler;
};

template<typename T>
std::new_handler
NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = currentHandler;
    currentHandler = p;
    return oldHandler;
}

template<typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) throw(std::bad_alloc)
{
    /* NewHandlerHolder 保存旧的 std::new_handler，在它的析构函数中再将旧的
       std::new_handler 设置回系统 */
    NewHandlerHolder h(std::set_new_handler(currentHandler));
    return ::operator new(size);  /* 调用系统（全局）的 new */
}
template<typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;

class Widget : public NewHandlerSupport<Widget> { };

void outOfMem()
{
    std::cerr << "Unable to satisfy request for memory\n";
    std::abort();
}

int main(void)
{
    Widget::set_new_handler(outOfMem);
    Widget* pw1 = new Widget;  // 如果内存分配失败，则调用outOfMem

    std::string* ps = new std::string;  // 如果内存分配失败，则调用global new-handler

    Widget::set_new_handler(0); 
    Widget* pw2 = new Widget;  // 因为 std::new_handler 被设置为 nullptr，所以如果内存分配失败，则立刻抛出异常
}
