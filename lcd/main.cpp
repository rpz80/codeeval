#include <iostream>
#include <map>
#include <string>
#include <cassert>
#include <vector>
#include <cstring>
#include <fstream>

namespace blcd {
  const std::map<char, std::string> digits = {
    {'0', "11111100"},
    {'1', "01100000"},
    {'2', "11011010"},
    {'3', "11110010"},
    {'4', "01100110"},
    {'5', "10110110"},
    {'6', "10111110"},
    {'7', "11100000"},
    {'8', "11111110"},
    {'9', "11110110"}
  };

  const size_t DIGIT_SIZE = 8;
  const size_t LCD_COUNT = 12;

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

  bool match(const stringref lcd_digit, char num_digit, bool point) {
    auto lcd_num_digit_it = digits.find(num_digit);
    assert(lcd_num_digit_it != digits.end());

    const std::string& lcd_num_digit = lcd_num_digit_it->second;
    if(point && lcd_digit[7] == '0') {
      return false;
    }
    
    for(size_t i = 0; i < DIGIT_SIZE; ++i) {
      if(lcd_num_digit[i] == '1' && lcd_digit[i] != '1')
        return false;
    }
    return true;
  }

  bool try_match(const std::vector<stringref>& lcds, 
                 const stringref& num,
                 size_t first_ind) {
    bool point_met = false;
    const char* cur = num.begin();

    for(; cur != num.end(); ++cur) {
      if(*cur == '.')
        continue;
      bool point = false;
      if(!point_met && *(cur+1) == '.') {
        point_met = true;
        point = true;
      }
      if(!match(lcds[first_ind++], *cur, point)) {
        return false;
      }
    }

    return true;
  }

  void process(const stringref& line) {
    const char* beg = line.begin();
    const char* cur = beg;
    std::vector<stringref> lcds;

    while(true) {
      bool done = false;
      switch(*cur) {
      case ' ':
        lcds.push_back(stringref(beg, cur));
        beg = cur + 1;
        break;
      case ';':
        lcds.push_back(stringref(beg, cur));
        beg = cur + 1;
        done = true;
        break;
      }
      ++cur;
      if(done) {
        break;
      }
    }

    stringref number(cur, line.end());
    size_t number_size = number.size() - 1;
    size_t first_lcd = 0;

    while(first_lcd + number_size <= LCD_COUNT) {
      if(try_match(lcds, number, first_lcd)) {
        std::printf("1\n");
        return;
      }
      ++first_lcd;
    }

    std::printf("0\n");
  }
}

void test() {
  assert(!blcd::match(blcd::stringref("10110001", 8), '1', false));
  assert(blcd::match(blcd::stringref("01100001", 8), '1', true));
  assert(blcd::match(blcd::stringref("11111111", 8), '8', true));
  assert(blcd::match(blcd::stringref("11111110", 8), '8', false));

  std::string s1 = "10110001 11111000 11111110 11111111 11111111 11111111 11111111 11101101 11111111 01111111 11110010 10100111;84.525784";
  std::string s2 = "11111111 11110110 11101111 11110111 10111110 11110110 10111011 10100111 11111100 01100100 11111101 01011110;5.57";
  std::string s3 = "11000010 00001111 11111111 10111111 11101011 11110011 01111110 11011111 11111111 11111111 11111001 01101110;857.71284";
  std::string s4 = "11111111 01110111 10111011 11001101 11111011 11101010 11110100 01001101 11011111 11111010 10010110 10111111;66.92";
  std::string s5 = "11111011 10010001 11111011 11111101 10011111 10111110 01111100 11011101 10111001 11111110 11101111 11110110;188.87";
  
  blcd::process(s1);
  blcd::process(s2);
  blcd::process(s3);
  blcd::process(s4);
  blcd::process(s5);
}

int main(int argc, const char* argv[]) {
  //test();
  std::ifstream ifs(argv[1]);
  std::string line;

  while(!ifs.eof()) {
    std::getline(ifs, line);
    blcd::process(line);
  }
}