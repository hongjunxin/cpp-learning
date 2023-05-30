// 写一段代码判断使用 gcc 还是用 g++ 进行编译的

#include <stdio.h>

#ifdef __cplusplus
#define USING_C 0
#else
#define USING_C 1
#endif

int main(int argc, char **argv)
{
    if (USING_C) {
        printf("using c compiler\n");
    } else {
        printf("using c++ compiler\n");
    }
    return 0;
}

/*
测试结果：
[root@hong base]# gcc cplusplus_macro.c
[root@hong base]# 
[root@hong base]# ./a.out 
using c compiler
[root@hong base]# g++ cplusplus_macro.c
[root@hong base]# ./a.out 
using c++ compiler

NOTE：测试环境 vscode ide，内置 windows linux wsl 进行编译。
发现将文件保存为 .cc 后缀时，不管用 gcc 还是用 g++，都是输出 "using c++ compiler"，
保存成 .c 后缀时，就能输出上面的测试结果。
*/
