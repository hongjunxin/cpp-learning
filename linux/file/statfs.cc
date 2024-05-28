#include <iostream>
#include <sys/vfs.h>
#include <errno.h>
#include <cstring>

void print_statfs(const char *path) {
    struct statfs buf;

    if (statfs(path, &buf) != 0) {
        std::cerr << "Failed to get file system statistics: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "File system information for " << path << ":\n";
    std::cout << "  File system type: 0x" << std::hex << buf.f_type << std::dec << "\n";
    std::cout << "  Optimal transfer block size: " << buf.f_bsize << " bytes\n";
    std::cout << "  Total data blocks: " << buf.f_blocks << "\n";
    std::cout << "  Free blocks: " << buf.f_bfree << "\n";
    std::cout << "  Available blocks: " << buf.f_bavail << "\n";
    std::cout << "  Total file nodes: " << buf.f_files << "\n";
    std::cout << "  Free file nodes: " << buf.f_ffree << "\n";
    std::cout << "  Maximum filename length: " << buf.f_namelen << "\n";

    // 计算总空间和可用空间，以字节为单位
    unsigned long long total_space = buf.f_blocks * buf.f_bsize;
    unsigned long long free_space = buf.f_bfree * buf.f_bsize;

    // 打印总空间和可用空间
    std::cout << "File system space for " << path << ":\n";
    std::cout << "  Total space: " << total_space << " bytes\n";
    std::cout << "  Free space: " << free_space << " bytes\n";    
}

/*
    获取与文件系统相关的信息，如文件系统的类型、总大小、可用空间等
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path>" << std::endl;
        return 1;
    }

    print_statfs(argv[1]);

    return 0;
}
