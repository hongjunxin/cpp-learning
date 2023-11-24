#include <stdio.h>
#include <string>
#include <sstream>

static int decode_base64_internal(std::string &dst, std::string &src, const u_char *basis)
{
    size_t len;

    for (len = 0; len < src.length(); len++) {
        if (src[len] == '=') {
            break;
        }

        if (basis[src[len]] == 77) {
            return -1;
        }
    }

    if (len % 4 == 1) {
        return -1;
    }

    auto s = src.c_str();
    std::stringstream ss;

    while (len > 3) {
        ss << (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
        ss << (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
        ss << (u_char) (basis[s[2]] << 6 | basis[s[3]]);
        s += 4;
        len -= 4;
    }

    if (len > 1) {
        ss << (u_char) (basis[s[0]] << 2 | basis[s[1]] >> 4);
    }

    if (len > 2) {
        ss << (u_char) (basis[s[1]] << 4 | basis[s[2]] >> 2);
    }

    dst = ss.str();

    return 0;
}

int decode_base64(std::string &dst, std::string &src)
{
    static u_char   basis64[] = {
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 62, 77, 77, 77, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 77, 77, 77, 77, 77,
        77,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 77, 77, 77, 77, 77,
        77, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 77, 77, 77, 77, 77,

        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77, 77
    };

    return decode_base64_internal(dst, src, basis64);
}

int main(int argc, char **argv)
{
    std::string src = "MTIzNDU2Nzg5Ky0qLw==";
    std::string dst;

    decode_base64(dst, src);
    printf("base64 decode: '%s'\n", dst.c_str());

    return 0;
}