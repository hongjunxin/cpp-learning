#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/eventfd.h>
#include <pthread.h>

/*
    Linux 通过 eventfd()系统调用额外提供了一种非标准的同步机制。这个系统调用创建了一个 eventfd 对象，该对象
    拥有一个相关的由内核维护的 8 字节无符号整数，它返回一个指向该对象的文件描述符。向这个文件描述符中写入一个
    整数将会把该整数加到对象值上。当对象值为 0 时对该文件描述符的 read()操作将会被阻塞。如果对象的值非零，
    那么 read()会返回该值并将对象值重置为 0。

    eventfd 的写操作是非阻塞的。当你使用 eventfd_write 向事件文件描述符写入事件时，它不会阻塞当前线程或进程。
    即使没有其他线程或进程在等待事件，写操作也会成功，并且事件的计数会增加。
*/

static int efd;

static void* thread_function(void* arg)
{
    uint64_t value;
    ssize_t s;

    for (;;) {
        s = read(efd, &value, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Thread received event. Value: %lu\n", value);
        if (value == 2) {
            printf("[thread] got 2, quit\n");
            break;
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t thread;
    uint64_t event_value = 0;  // eventfd 初始值

    // EFD_SEMAPHORE flag 会导致读取时，读到的值都是 1
    efd = eventfd(event_value, EFD_CLOEXEC);
    if (efd == -1) {
        perror("eventfd");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 5; i++) {
        eventfd_write(efd, 1);
        sleep(1);
    }
    eventfd_write(efd, 2);

    pthread_join(thread, NULL);
    close(efd);

    return 0;
}
