#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <thread>

#define MAX_EVENTS 10

/*
    水平触发：LT 模式是 epoll 的默认模式。在 LT 模式下，当文件描述符上的事件处于就绪状态时，每次调用 epoll_wait 都会返回该事件，
    直到该事件被处理掉。即使事件没有被处理，下一次调用 epoll_wait 时仍然会返回该事件，直到该事件不再处于就绪状态。

    边缘触发（EPOLLET）：在 ET 模式下，只有在文件描述符上的事件状态发生变化时，epoll_wait 才会返回该事件。
    一旦事件被检测到并返回，我们需要立即处理该事件。如果不处理，下一次调用 epoll_wait 不会再返回该事件，即使它仍然处于就绪状态。
*/

static int efd; 
static int epoll_fd;

int main(int argc, char **argv)
{
    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];
    uint64_t event_value = 1;
    pthread_t thread;

    efd = eventfd(event_value, EFD_NONBLOCK);
    if (efd == -1) {
        perror("eventfd");
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    event.data.fd = efd;
    event.events = EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, efd, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    auto post_event = [](int index) {
        struct epoll_event ev;
        // 因为 eventfd 写操作是非阻塞的，导致 EPOLLOUT 会一直就绪，所以触发模式只能用 ET。
        ev.events = EPOLLET | EPOLLOUT;
        ev.data.fd = efd;

        // 每次重新执行 EPOLL_CTL_MOD，epoll_wait 会再次返回该 event 事件
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, efd, &ev) < 0) {
            printf("epoll_ctl EPOLL_CTL_MOD error, %s\n", strerror(errno));
            return;
        }
        printf("[thread %d] post eventfd\n", index);
    };

    auto epoll_loop_task = std::thread([&]() {
        for (;;) {
            int n, i;
            n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            if (n == -1) {
                perror("epoll_wait");
                exit(EXIT_FAILURE);
            }
            for (i = 0; i < n; i++) {
                if (events[i].data.fd == efd) {
                    printf("got eventfd event, events: ");
                    if (events[i].events & EPOLLOUT) {
                        printf("EPOLLOUT ");
                    }
                    // 返回的 events 中不包含 EPOLLET
                    if (events[i].events & EPOLLET) {
                        printf("EPOLLET ");
                    }
                    printf("\n");
                    continue;
                }
            }
        }
    });

    epoll_loop_task.detach();

    sleep(1);
    auto f1 = std::thread(post_event, 1);
    sleep(1);
    auto f2 = std::thread(post_event, 2);

    f1.join();
    f2.join();
    
    close(efd);
    close(epoll_fd);
    sleep(1);

    return 0;
}
