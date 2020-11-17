/*
条款 13：以对象管理资源
*/

#include <memory>
#include <string>
#include <vector>

int main(void)
{

    /* auto_ptr 和 shared_ptr 两者都在析构函数中调用 delete 而不是 delete[]，
       所以在动态分配得到的 array 身上使用 auto_ptr 和 shared_ptr 是不合理的做法，
       但这样的代码却能通过编译。*/

 // std::shared_ptr<std::string> a(new std::string[10]);

    using namespace std;

    /* 所以还是尽量使用 vector，而不是数组 */
    std::shared_ptr<vector<string>> b(new vector<string>(10));

    return 0;
}