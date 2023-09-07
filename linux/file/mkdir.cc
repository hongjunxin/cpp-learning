#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <iostream>

static bool found_specified_str(std::string target_str, std::string file)
{
    int fd, count;
    bool found = false;
    char buf[512] = {'\0'};

    if (target_str.length() > sizeof(buf)) {
        std::cout << "error: target string too large, more than "
            << sizeof(buf) << " bytes"
            << std::endl;
        return false; 
    }

    fd = open(file.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cout << "open " << file << " failed, error: " << strerror(errno) << std::endl;
        return found;
    }

    int offset = 0;
    for (;;) {
        count = read(fd, buf + offset, sizeof(buf) - offset);
        if (count <= 0) {
            break;
        }
        if (strstr(buf, target_str.c_str()) != NULL) {
            found = true;
            break;
        }
        if (count > target_str.length()) {
            int j;
            if (buf[sizeof(buf) - 1] != '\0') {
                j = sizeof(buf) - target_str.length();
            } else {
                j = strlen(buf) - target_str.length();
            }
            for (int i = 0; i < target_str.length(); ++i, ++j) {
                buf[i] = buf[j];
            }
            offset = target_str.length();
            memset(buf + offset, '\0', sizeof(buf) - offset);
        } else {
            offset = 0;
        }
    }

    close(fd);
    return found;
}

static int create_dir(std::string path)
{
    char *p, *q;

    p = (char*) calloc(1, path.length() + 1);
    if (p == NULL) {
        std::cout << "calloc failed" << std::endl;
        return -1;
    }
    memcpy(p, path.c_str(), path.length());
    q = p;

    for (; *q != '\0'; ++q) {
        if (*q == '/' && p != q) {
            *q = '\0';
            if (access(p, F_OK) == -1) {
                mkdir(p, 0755);
            }
            *q = '/';
        }
    }
    if (access(p, F_OK) == -1) {
        mkdir(p, 0755);
    }

    free(p);
    return 0;
}

void read_dir(DIR *dir, std::string src_parent_path, std::string dst_parent_path)
{
    struct dirent *entry;
    
    while (entry = readdir(dir)) {
        if (entry->d_type == DT_DIR) {
            auto dirname = std::string(entry->d_name);
            if (dirname != "." && dirname != "..") {
                auto src_path = src_parent_path + "/" + dirname;
                auto dst_path = dst_parent_path + "/" + dirname;
                DIR *dir = opendir(src_path.c_str());
                if (dir == NULL) {
                    std::cout << "opendir " << src_path << " failed, error: " << strerror(errno) << std::endl; 
                    continue;
                }
                read_dir(dir, src_path, dst_path);
            }
        } else if (entry->d_type == DT_REG) {
            auto src_file = src_parent_path + "/" + std::string(entry->d_name);
            auto dst_file = dst_parent_path + "/" + std::string(entry->d_name);

            if (found_specified_str("done", src_file)) {            
                int src, dst;
                if (create_dir(dst_parent_path) == -1) {
                    std::cout << "create dir " << dst_parent_path << " failed"
                        << ", skip copy to " + dst_file
                        << std::endl;
                    continue;
                }

                dst = creat(dst_file.c_str(), 0644);
                if (dst == -1) {
                    std::cout << "create " << dst_file << " failed, error: " << strerror(errno) << std::endl;
                    continue;
                }
                
                src = open(src_file.c_str(), O_RDONLY);
                if (src == -1) {
                    std::cout << "open " << src_file << " failed, error: " << strerror(errno) << std::endl;
                    close(dst);
                    continue;
                }

                char buf[1024] = {'\0'};
                int count;
                for (;;) {
                    count = read(src, buf, sizeof(buf));
                    if (count == -1) {
                        std::cout << "read " << src_file << "failed, error: " << strerror(errno) << std::endl;
                        break;
                    } else if (count == 0) {
                        break;  // EOF
                    }
                    if (write(dst, buf, count) != count) {
                        std::cout << "error: write partial data to " << dst_file << std::endl;
                        break;
                    }
                }
                close(src);
                close(dst);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    DIR *etc = NULL;

    if ((etc = opendir("/etc")) == NULL) {
        std::cout << "open /etc failed, error: " << strerror(errno) << std::endl; 
        return -1;
    }

    read_dir(etc, "/etc", "./tmp");
}
