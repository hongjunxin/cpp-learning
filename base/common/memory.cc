#include <iostream>

class InnerObj {
public:
    InnerObj() {
        std::cout << "InnerObj()::InnerObj()" << std::endl;
    }
    ~InnerObj() {
        std::cout << "InnerObj()::~InnerObj()" << std::endl;
    }
};

class Obj {
public:
    Obj() {
        std::cout << "Obj::Obj()" << std::endl; 
    }
    ~Obj() {
        std::cout << "Obj::~Obj()" << std::endl;
    }
private:
    InnerObj inner;
};

void malloc_free_test()
{
    // 单纯地只进行内存的分配和释放，不会自动调用 Obj 
    // 的构造函数和析构函数
    Obj *a = (Obj*) malloc(sizeof(Obj));
    free(a);
}

void new_delete_test()
{
    Obj *a = new Obj();  // 内部会调用 Obj::Obj()
    delete a;  // 内部会调用 Obj::~Obj()
}

int main(int argc, char **argv)
{
    malloc_free_test(); // 没有 log
    new_delete_test();
/*
    执行结果:
    InnerObj()::InnerObj()
    Obj::Obj()
    Obj::~Obj()
    InnerObj()::~InnerObj()    
*/
}
