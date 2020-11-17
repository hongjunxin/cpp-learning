/*
条款 47：使用 traits class 来表现类型信息
*/

#include <iterator>
#include <vector>
#include <list>
#include <iostream>

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::random_access_iterator_tag)
{
    std::cout << "std::random_access_iterator_tag" << std::endl;
    iter += d;
}

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::bidirectional_iterator_tag)
{
    std::cout << "std::bidirectional_iterator_tag" << std::endl;
    if (d >= 0) { while (d--) ++iter; }
    else { while (d++) --iter; }
}

template<typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, std::input_iterator_tag)
{
    if (d < 0)
        throw std::out_of_range("Negative distance");
    while (d--) ++iter;
}

/* iterator 库中已经有  
   template<typename _InputIterator, typename _Distance>
    inline void
    advance(_InputIterator& __i, _Distance __n)
   所以这里不能出现同名模板，否则编译会报"overloaded advance..."
*/
template<typename IterT, typename DistT>
void myadvance(IterT iter, DistT d)
{
    /* iterator_traits::iterator_category 其实就是 IterT::iterator_category 的重命名 */
    doAdvance(iter, d, typename std::iterator_traits<IterT>::iterator_category());
}

int main(void)
{
    myadvance(std::vector<int>::iterator(), 1);

    std::list<int> list(10);
    myadvance(list.begin(), 1);

    return 0;
}

