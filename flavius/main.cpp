#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <algorithm>

typedef std::vector<bool> buf_t;


inline void print_cycle(buf_t& v, const size_t k) {
  size_t count = 0;
  size_t cur = k - 1;
  size_t vsize = v.size();
  
  while(true) {
    if(v[cur] == false) {
      v[cur] = true;
      std::cout << cur << " ";
      ++count;
      if(count == vsize) break;
    }  
    size_t tmp = 0;

    while(tmp != k) {
      if(cur + 1 == vsize) {
        cur = 0;
        if(v[cur] == false)
          ++tmp; 
        else if(count == vsize - 1) break;
      } else {
        if(v[cur + 1] == false)
          ++tmp; 
        ++cur;
        if(count == vsize - 1 && !v[cur]) break;
      }
    }      
  }
  std::cout << std::endl;
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  char c;
  buf_t num_vec;
  bool first = true;
  int tmp;
  
  num_vec.reserve(20);
  
  while(!ifs.eof()) {
    c = ifs.get();
    
    if(std::isdigit(c))
      ifs.unget();
    else continue;

    ifs >> tmp;
    
    switch(first) {
      case true:
        num_vec.resize(tmp);
        std::fill(num_vec.begin(), num_vec.end(), false);
        first = false;
        break;
      case false:
        first = true;
        print_cycle(num_vec, tmp);
        num_vec.clear();
        break;
    }
  }
}