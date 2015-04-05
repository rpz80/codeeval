#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <deque>
#include <algorithm>
#include <chrono>

namespace bw {
  class decoder {
  public:
    std::string decode(const std::string& s) {
      table_.clear();
      init_str_ = s.substr(0, s.size() - 1);
      size_t size = init_str_.size();
      table_.resize(size);
      for(size_t i = 0; i < size; ++i) {
        for(size_t j = 0; j < size; ++j) {
          table_[j].push_front(init_str_[j]);
        }
        std::sort(table_.begin(), table_.end(),
                  [](const std::deque<char>& d1, const std::deque<char>& d2) {
                    for(size_t i = 0; i < d1.size(); ++i) {
                      if(d1[i] < d2[i]) {
                        return true;
                      }
                      else if(d2[i] < d1[i]) {
                        return false;
                      }
                    }
                    return false;
                  });
      }
      for(size_t i = 0; i < size; ++i) {
        if(table_[i][size-1] == '$') {
          std::string ret;
          ret.reserve(size);
          std::copy(table_[i].begin(), table_[i].end(), std::back_inserter(ret));
          return ret;
        }
      }
      return "";
    }
  private:
    std::vector<std::deque<char>> table_;
    std::string init_str_;
  };
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  bw::decoder d;
  auto start = std::chrono::system_clock::now();
  while(!ifs.eof()) {
    std::getline(ifs, line);
    std::cout << d.decode(line) << std::endl; 
  }
  auto end = std::chrono::system_clock::now();
  std::cout << "Elapsed "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
}