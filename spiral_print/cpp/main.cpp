#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

namespace sp {
  class stringref {
  public:
    stringref(const char* b, const size_t size) :
      b_(b), size_(size) {}
    
    stringref(const char* b, const char* e) {
      assert(e >= b);
      b_ = b;
      size_ = e - b;
    }

    stringref(const std::string& s) 
      : b_(&s[0]),
        size_(s.size()) {}

    char operator [] (size_t i) const {
      assert(i < size_);
      return *(b_ + i);
    }

    const char* begin() const {
      return b_;
    }

    const char* end() const {
      return b_ + size_;
    }

    const size_t size() const {
      return size_;
    }
  private:
    const char* b_;
    size_t size_;
  };

  class matrix {
  public:
    matrix(const std::string& s) {
    }
  private:
  };
}

int main(int argc, const char* argv[]) {

}