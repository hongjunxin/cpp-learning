/*
条款 17：以独立语句将 newed 对象置入智能指针
*/

#include <memory>
#include <string>
#include <iostream>

static std::string priority()
{
    return "Hi";
}

static void processWidget(std::shared_ptr<std::string> p, std::string s)
{
    std::cout << "processWidget() called" << std::endl;
}

int main(void)
{
/*
    以上调用可能会出现资源泄露，因为在同一条语句内，编译器改变操作顺序的自由度较大，
    所以以上调用最终获得的操作顺序可能是：
	-- 执行 new std::string
	-- 调用 priority
	-- 调用 shared_ptr 构造函数

    如果对 priority 的调用出现异常，则在这之前的 new std::string 返回的指针将会遗失，
    因为它还没被置入 shared_ptr 内。为避免这类问题，我们应该使用分离语句。
*/
    processWidget(std::shared_ptr<std::string>(new std::string), priority());  /* 临时参数是右值 */

    /* 应该使用分离的写法 */
    std::shared_ptr<std::string> pw(new std::string);
    processWidget(pw, priority());

    /* 或者使用 std::make_shared<> */
    processWidget(std::make_shared<std::string>(), priority());

    return 0;
}