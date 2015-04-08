#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

typedef std::vector<int> iv_t;

const size_t BUF_SIZE = 256;

inline void process(iv_t& v, iv_t& target) {
  size_t vsize = v.size();
  
  for(size_t i = 0; i < vsize - 1; ++i) {
    target.push_back(std::abs(v[i+1] - v[i]));
  }
  size_t target_size = target.size();
  
  if(target_size != vsize - 1) {
    std::cout << "Not jolly\n";
    return;
  } else {
    std::sort(target.begin(), target.end());
    for(size_t i = 0; i < target_size; ++i) {
      if(target[i] != (int)i + 1) {
        std::cout << "Not jolly\n";
        return;
      }
    }
  }
  std::cout << "Jolly\n";
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  std::stringstream ss;
  
  int tmp;
  iv_t v, target_v;
  
  v.reserve(BUF_SIZE);
  target_v.reserve(BUF_SIZE);
    
  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line == "\n" || line.empty())
      continue;
    
    ss.clear();
    ss.str(line);

    v.clear();
    target_v.clear();
    
    ss >> tmp;
    
    while(!ss.eof()) {
      ss >> tmp;
      v.push_back(tmp);
    }
    process(v, target_v);
  }  
}
