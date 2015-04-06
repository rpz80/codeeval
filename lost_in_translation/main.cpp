#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <fstream>

typedef std::map<char, char> letter_map_type;
letter_map_type letters = {
  {'a', 'y'}, {'b', 'h'}, {'c', 'e'}, {'d', 's'}, {'e', 'o'},
  {'f', 'c'}, {'g', 'v'}, {'h', 'x'}, {'i', 'd'}, {'j', 'u'},
  {'k', 'i'}, {'l', 'g'}, {'m', 'l'}, {'n', 'b'}, {'o', 'k'},
  {'p', 'r'}, {'q', 'z'}, {'r', 't'}, {'s', 'n'}, {'t', 'w'},
  {'u', 'j'}, {'v', 'p'}, {'w', 'f'}, {'x', 'm'}, {'y', 'a'},
  {'z', 'q'}
};

void translate(const std::string& s) {
  for(char c : s) {
    if(std::isalpha(c))
      c = letters[c];
    std::cout << c;
  }
  std::cout << std::endl;
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  while(!ifs.eof()) {
    std::getline(ifs, line);
    translate(line);
  }
}
