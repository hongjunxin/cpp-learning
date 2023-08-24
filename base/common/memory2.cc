#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct outbuff_t {
    int len;
    char *buff;
    int *num;
};

struct outbuff_t *outbuff;

void send(char *buff, int len, int *num)
{
    outbuff = (outbuff_t*) malloc(sizeof(struct outbuff_t));
    outbuff->buff = buff;
    outbuff->len = len;
    outbuff->num = num;
}

void foo()
{
    char buff[256] = {'\0'};
    buff[0] = 'C';
    buff[1] = 'P';
    buff[2] = 'P';

    int num = strlen(buff);

    send(buff, sizeof(buff), &num);
}

// gcc memory2.cc

int main(int argc, char **argv)
{
    foo();
    int i = 2;

    // 没懂! 这里能正常打印出 foo() 栈空间的变量
    // 可能的解释是,栈空间的脏数据还没被清除
    printf("%.*s, num %d\n", outbuff->len, outbuff->buff, *outbuff->num);
}
