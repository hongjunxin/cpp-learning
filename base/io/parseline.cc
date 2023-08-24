#include <iostream>
#include <fstream>
#include <unordered_map>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[])
{
	std::unordered_map<std::string, std::string> map;
	const char *file = "/etc/shadow";

	std::ifstream infile(file, std::ios::in);
    if (!infile.is_open()) {
        printf("open %s error, %s\n", file, strerror(errno));
    }

	std::string line;
	while (getline(infile, line)) {
		auto n = line.find(':');
		if (n == line.npos) {
			continue;
		}
		auto name = std::string(line, 0, n);
		if (line.at(n + 1) != '$') {
			map.insert({std::move(name), "N/A"});
			continue;
		}
		auto m = line.find_last_of('$');
		if (m == line.npos || m == n + 1) {
			map.insert({std::move(name), "N/A"});
			continue;
		}
		auto salt = std::string(line, n + 1, m - n - 1);
		map.insert({std::move(name), std::move(salt)});
	}
	infile.close();

	for (auto &e : map) {
		std::cout << e.first << ": " << e.second << std::endl;
	}
	std::cout << std::endl;
}
