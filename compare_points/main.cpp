#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>

struct point {
  int x;
  int y;
};

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  std::stringstream ss;
  point p1, p2;
  std::array<char,2> buf;
  while (!ifs.eof()) {
    std::getline(ifs, line);
    if (line.empty())
      continue;
    ss.clear();
    ss.str(line);
    ss >> p1.x >> p1.y >> p2.x >> p2.y;    
    if (p1.x == p2.x && p1.y == p2.y) {
      std::cout << "here" << std::endl;
      continue;
    }
    else {
      int dx = p1.x - p2.x;
      int dy = p1.y - p2.y;      
      int second_index = dy == 0 ? 0 : 1;
      if (dx == 0 || dy == 0) {
        buf[1] = 0;
      }      
      if (dy > 0)
        buf[0] = 'S';
      else if (dy < 0)
        buf[0] = 'N';
      if (dx > 0)
        buf[second_index] = 'W';
      else if (dx < 0)
        buf[second_index] = 'E';
    }
    for (size_t i = 0; i < 2; ++i) {
      if (buf[i] != 0) {
        std::cout << buf[i];
      }
    }
    std::cout << std::endl;
  }
}