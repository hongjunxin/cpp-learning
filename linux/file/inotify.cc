#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>

int main(int argc, char **argv)
{
    const char *target = "/tmp";
     
    if (argc == 2) {
        target = argv[1];
    }

    int fd = inotify_init();
    if (fd == -1) {
        perror("inotify_init");
        return 1;
    }

    int wd = inotify_add_watch(fd, target, IN_CREATE);
    if (wd == -1) {
        perror("inotify_add_watch");
        return 1;
    }

    char buffer[4096];
    ssize_t count;

    printf("Monitoring %s for new files...\n", target);

    while (1) {
        count = read(fd, buffer, sizeof(buffer));

        if (count <= 0) {
            perror("read");
            return 1;
        }

        for (char *ptr = buffer; ptr < buffer + count;) {
            struct inotify_event *event = (struct inotify_event *)ptr;

            if (event->mask & IN_CREATE) {
                printf("New file or directory created: %s\n", event->name);
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    close(fd);
    return 0;
}
