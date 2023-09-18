#include <iostream>

class Header {
public:
    Header() { std::cout << "Header() called" << std::endl; }
    ~Header() { std::cout << "~Header() called" << std::endl; }
};

class Response {
public:
    Response() { std::cout << "Response() called" << std::endl; }
    ~Response() { std::cout << "~Response() called" << std::endl; }
private:
    Header headers;
};

struct HttpContext {
    Response resp;
};

/*
    malloc 只是分配了内存，但不进行初始化
    new 不仅分配了内存，还会调用类的构造函数类来初始化内存，而类的构造函数又会触发对类数据成员构造函数的调用。

    对于 struct，如果它的数据成员都是基本类型，则可以使用 malloc，否则得使用 new，这样才能保证其类数据成员
    能被进行初始化（即调用类数据成员的构造函数）
*/
int main(int argc, char **argv)
{
    HttpContext *httpctx1, *httpcxt2;
    httpctx1 = (HttpContext*) malloc(sizeof(HttpContext));
    std::cout << "malloc httpctx1 done\n";

    httpcxt2 = new HttpContext();
    std::cout << "new httpctx2 done\n";

    /*
        output:
            malloc httpctx1 done
            Header() called
            Response() called
            new httpctx2 done
    */

    return 0;
}
