#include <iostream>
#include <fstream>
#include <errno.h>
#include <string.h>

using namespace std;
int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "please enter a filename." << endl;
		exit(0);
	}

	ifstream infile(argv[1], ios::in);
    if (!infile.is_open()) {
        printf("open %s error, %s\n", argv[1], strerror(errno));
    }

	string line;
	while (getline(infile, line)) {
		cout << line << endl;
	}
	infile.close();
}
