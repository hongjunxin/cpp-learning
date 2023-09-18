#include <unistd.h>
#include <stdio.h>

int main(int argc, char * argv[])
{
    int ch;
    printf("optind=%d, opterr=%d, argc=%d\n",optind,opterr, argc);
    printf("option is \"ab:c:de::\"\n");
    printf("--------------------------\n");
    
    /* 如果没有参数可继续读取，才返回 -1，而读取的参数是非法时，返回的是 '?' */
    while ((ch = getopt(argc, argv, "ab:c:de::")) != -1)
    {
        switch (ch) {
        case 'a':
            printf("HAVE option: -a, optind=%d, optarg=%s\n", optind, optarg);
            break;
        case 'b':
            printf("HAVE option: -b, optind=%d, optarg=%s\n", optind, optarg);
            break;
        case 'c':
            printf("HAVE option: -c, optind=%d, optarg=%s\n", optind, optarg);
            break;
        case 'd':
            printf("HAVE option: -d, optind=%d, optarg=%s\n", optind, optarg);
            break;
        case 'e':
            printf("HAVE option: -e, optind=%d, optarg=%s\n", optind, optarg);
            break;

        /* getopt() prints an error message on standard error if opterr set to 1(default is 1),
           places the erroneous option character in optopt, and returns '?' as the function result. */
        case '?':
            printf("Unknown option: %c\n\n",(char)optopt);
            break;
        }
    }
    printf("----------------------------\n");
    printf("optind=%d, optarg=%s\n", optind, optarg);
}