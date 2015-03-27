#include <iostream>
#include <fstream>

int main(int argc, const char* argv) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << "file name\n";
		return 1;
	}

	std::ifstream ifs(argv[1]);
	if (!ifs) {
		std::cout << "File open error\n";
		return 1;
	}
}
