#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void util_show_progress(const char *hint, int done, int total)
{
    char bar[52] = {'\0'};
    int percent, i;

    percent = done * 100 / total;

    for (i = 0; i < percent / 2; ++i) {
        bar[i] = '=';
    }
    bar[i] = '>';

    printf("%s %%%d [%-51s] [%d/%d]\r", 
        hint, percent, bar, done, total);
    fflush(stdout);  // must add this
}

int main(int argc, char **argv)
{
    int i;
    for (i = 1; i < 101; i++) {
        util_show_progress("test...", i, 100);
        usleep(100000);
    }
    printf("\n");
}