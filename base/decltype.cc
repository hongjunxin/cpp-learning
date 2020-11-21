struct 
{
    int d ;
    double b;
} anon_s;  /* 匿名结构体 */

/* 尾置返回类型
   我们通知编译器 fcn 的返回类型与解引用 beg 参数的结果类型相同。解引用运算符返回一个左值，
   因此通过 decltype 推断的类型为 beg 表示的元素的类型的引用。因此，如果对一个 string 
   序列调用 fcn，则返回的类型是 string& */
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    // 处理序列
    return *beg;  // 返回元素的引用
}

template <typename _Tx, typename _Ty>
auto multiply(_Tx x, _Ty y) -> decltype(x * y) {
    return x * y;
}

int main(void)
{
    anon_s.b = 0;
    decltype(anon_s) aa;  /* 借助 decltype，我们可以使用匿名结构体 */
    aa.d = 1;

    int a = 4;
    decltype(a) b;  /* decltype(a) 结果是 int */
    decltype((a)) d = a;  /* decltype((a)) 结果是 int& */

    return 0;
}
