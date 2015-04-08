#include <iostream>
#include <bitset>
#include <fstream>
#include <string>

void print_binary(const std::string& s) {
	std::bitset<32> b(std::stol(s));
	std::string bs = b.to_string();
	size_t one_pos = bs.find('1');

	if(one_pos == std::string::npos)
		std::cout << 0 << std::endl;
	else {
		bs = bs.substr(one_pos);
		std::cout << bs << std::endl;
	}
}

int main(int argc, const char* argv[]) {
	std::ifstream ifs(argv[1]);
	std::string line;
	while(!ifs.eof()) {
		std::getline(ifs, line);
		if(line.empty())
			continue;
		print_binary(line);
	}
	return 0;
}