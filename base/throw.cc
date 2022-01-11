#include <iostream>
#include <cstdlib>

//自定义的异常类型
class OutOfRange {
public:
    OutOfRange(): m_flag(1) { };
    // 初始化的顺序是: m_flag, m_len, m_index，即按照类中它们被定义的顺序
    OutOfRange(int len, int index): m_len(len), m_index(index), m_flag(2) { }
public:
    void what() const;  //获取具体的错误信息
private:
    int m_flag;  //不同的flag表示不同的错误
    int m_len;  //当前数组的长度
    int m_index;  //当前使用的数组下标
};

void OutOfRange::what() const {
    if (m_flag == 1) {
        std::cout << "Error: empty array, no elements to pop." << std::endl;
    } else if (m_flag == 2) {
        std::cout << "Error: out of range( array length " << m_len 
            << ", access index " << m_index << " )" << std::endl;
    } else {
        std::cout << "Unknown exception." << std::endl;
    }
}

//实现动态数组
class Array {
public:
    Array();
    ~Array() { free(m_p); };
public:
    int operator[](int i) const;  //获取数组元素
    int push(int ele);  //在末尾插入数组元素
    int pop();  //在末尾删除数组元素
    int length() const { return m_len; };  //获取数组长度
private:
    int m_len;  //数组长度
    int m_capacity;  //当前的内存能容纳多少个元素
    int *m_p;  //内存指针
private:
    static const int m_stepSize = 50;  //每次扩容的步长
};

Array::Array() {
    m_p = (int*) malloc(sizeof(int) * m_stepSize);
    m_capacity = m_stepSize;
    m_len = 0;
}

int Array::operator[](int index) const {
    if (index < 0 || index >= m_len) {
        throw OutOfRange(m_len, index);
    }

    return *(m_p + index);
}

int Array::push(int ele) {
    if (m_len >= m_capacity) {
        m_capacity += m_stepSize;
        m_p = (int*) realloc(m_p, sizeof(int) * m_capacity);
    }

    *(m_p + m_len) = ele;
    m_len++;
    return m_len-1;
}

int Array::pop() {
    if (m_len == 0) {
         throw OutOfRange();
    }

    m_len--;
    return *(m_p + m_len);
}

void printArray(Array &arr) {
    int len = arr.length();

    if (len == 0) {
        std::cout << "Empty array! No elements to print." << std::endl;
        return;
    }

    for (int i = 0; i < len; i++) {
        if (i == len - 1)
            std::cout << arr[i] << std::endl;
        else
            std::cout << arr[i] << ", ";
    }
}

int main(int argc, char** argv)
{
    Array nums;

    for (int i = 0; i < 10; i++)
        nums.push(i);

    printArray(nums);

    try {
        std::cout << nums[20] << std::endl;
    } catch (OutOfRange &e) {
        e.what();
    }

    try {
        for (int i = 0; i < 20; i++)
            nums.pop();
    } catch (OutOfRange &e) {
        e.what();
    }

    printArray(nums);

    return 0;
}