
#include <string>
#include <sstream>

int main(int argc, char **argv)
{
    FILE *fp = popen("dmidecode -t memory", "r");
    if (!fp) {
        return -1;
    }

    std::stringstream ss;
    int n;
    char buf[4096];
    for (;;) {
        n = fread(buf, 1, sizeof(buf) - 1, fp);
        if (n > 0) {
            buf[n] = '\0';
            ss << buf;
        } else {
            break;
        }
    }
    pclose(fp);
    std::string line;
    while (getline(ss, line)) {
        printf("%s\n", line.c_str());
    }
    return 0;
}