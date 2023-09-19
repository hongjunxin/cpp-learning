#include <iostream>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

std::string urldecode(const std::string &encoded_str)
{
    std::string decoded_str;
    char hex[3] = {0};
    
    for (size_t i = 0; i < encoded_str.size(); ++i) {
        char c = encoded_str[i];
        
        if (c == '%') {
            if (i + 2 < encoded_str.size()) {
                hex[0] = encoded_str[i + 1];
                hex[1] = encoded_str[i + 2];
                int decoded_char;
                std::istringstream(hex) >> std::hex >> decoded_char;
                decoded_str += static_cast<char>(decoded_char);
                i += 2;
            }
        } else if (c == '+') {
            decoded_str += ' ';
        } else {
            decoded_str += c;
        }
    }
    
    return decoded_str;
}

// eg: ./a.out %E5%B1%B1%E6%B5%B7%E7%BB%8F%E4%B9%8B%E5%86%8D%E8%A7%81%E6%80%AA%E5%85%BD.2022.Goodbye.Monster
int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "input encoded url" << std::endl;
        return -1;
    }

    std::string encoded_str = argv[1];
    std::string decoded_str = urldecode(encoded_str);

    std::cout << "decoded: " << std::endl;
    std::cout << decoded_str << std::endl;

    auto file = "/dev/shm/media/" + decoded_str;
    if (access(file.c_str(), F_OK) == 0) {
        std::cout << "\n" << file << " found" << std::endl;
    }
    
    return 0;
}
