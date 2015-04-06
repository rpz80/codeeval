#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

namespace mr {
  class matrix {
  public:
    matrix() {
      m_.reserve(128);
      tmp_.reserve(128);
      line_.reserve(256);
    }
  public:
    void rotate();
  private:
    std::vector<char> m_;
    std::vector<char> tmp_;
    std::string line_;
    size_t size_;
  private:
    friend std::istream& operator >>(std::istream& is, matrix& m);
    friend std::ostream& operator <<(std::ostream& os, const matrix& m);
  };

  std::istream& operator >>(std::istream& is, matrix& m) {
    m.m_.clear();
    m.line_.clear();
    std::getline(is, m.line_);
    for(size_t i = 0; i < m.line_.size(); ++i) {
      if(m.line_[i] != ' ')
        m.m_.push_back(m.line_[i]);
    }
    m.size_ = (size_t)std::sqrt(m.m_.size());
    return is;
  }

  std::ostream& operator <<(std::ostream& os, const matrix& m) {
    for(size_t i = 0; i < m.m_.size(); ++i) {
      os << m.m_[i];
      if(i != m.m_.size() - 1)
        os << " ";
    }
    os << std::endl;
    return os;
  }

  void matrix::rotate() {
    tmp_.clear();
    for(size_t i = 0; i < size_; ++i) {
      for(size_t j = size_ - 1;; --j) {
        tmp_.push_back(m_[size_*j + i]);
        if(j == 0)
          break;
      }
    }
    m_.swap(tmp_);
  }
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  mr::matrix m;
  if(!ifs)
    return 1;
  while(!ifs.eof()) {
    ifs >> m;
    m.rotate();
    std::cout << m;
  }
  return 0;
}
