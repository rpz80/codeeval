#include <iostream>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <vector>
#include <map>
#include <fstream>

namespace ttn {
  class text_supplier {
   public:
    text_supplier(const char* fname);
    ~text_supplier();
   public:
    std::string& get_line();
    bool eof() {return ifs_.eof();}
   private:
    std::string line_;
    std::ifstream ifs_;
  };

  class string_ref {
   public:
    string_ref(const char* null_s);
    string_ref(const char* s, const uint32_t size);
   public:
    std::string to_string() const;
   private:
    const char* s_;
    uint32_t size_;
   private:
    friend bool operator < (const string_ref& l, const string_ref& r);
  };

  std::vector<string_ref> string_to_string_ref_vec(const std::string& s);

  class converter {
   public:
    converter();
   public:
    const uint32_t operator [](const char* s);
    const size_t values_size() const {return values_.size();}
    const int32_t operator()(const std::string& text);
  private:
    int32_t convert_helper(const std::vector<string_ref>& ar);
   private:
    std::fstream ifs_;
    std::map<string_ref,  uint32_t> values_;
  };
}

namespace ttn {
  string_ref::string_ref(const char* null_s) 
      : s_(null_s), size_(std::strlen(null_s)) {}
  
  string_ref::string_ref(const char* s, const uint32_t size) 
      : s_(s), size_(size) {}

  std::string string_ref::to_string() const{
    return std::string(s_, size_);
  }

  bool operator < (const string_ref& l, const string_ref& r) {
    if(l.size_ < r.size_)
      return true;
    else if(l.size_ > r.size_)
      return false;
    else {
      for(size_t i = 0; i < l.size_; ++i) {
        if(l.s_[i] < r.s_[i])
          return true;
        else if(l.s_[i] > r.s_[i])
          return false;
      }
    }
    return false;
  }
  text_supplier::text_supplier(const char* name) {
    ifs_.open(name);
    if(!ifs_)
      throw std::runtime_error("File open error");
  }

  text_supplier::~text_supplier() {
    ifs_.close();
  }

  std::string& text_supplier::get_line() {
    line_.clear();
    while(line_.empty()) {
        if(ifs_.eof())
          throw std::runtime_error("EOF");
        else
          std::getline(ifs_, line_);
    }
    return line_;
  }

  converter::converter() {  
    values_.insert(std::make_pair("million", 1000000));
    values_.insert(std::make_pair("thousand", 1000));
    values_.insert(std::make_pair("hundred", 100));
      
    values_.insert(std::make_pair("ninety", 90));
    values_.insert(std::make_pair("eighty", 80));
    values_.insert(std::make_pair("seventy", 70));
    values_.insert(std::make_pair("sixty", 60));
    values_.insert(std::make_pair("fifty", 50));
    values_.insert(std::make_pair("forty", 40));
    values_.insert(std::make_pair("thirty", 30));
    values_.insert(std::make_pair("twenty", 20));
    values_.insert(std::make_pair("ten", 10));

    values_.insert(std::make_pair("zero", 0));
    values_.insert(std::make_pair("one", 1));
    values_.insert(std::make_pair("two", 2));
    values_.insert(std::make_pair("three", 3));
    values_.insert(std::make_pair("four", 4));
    values_.insert(std::make_pair("five", 5));
    values_.insert(std::make_pair("six", 6));
    values_.insert(std::make_pair("seven", 7));
    values_.insert(std::make_pair("eight", 8));
    values_.insert(std::make_pair("nine", 9));
       
    values_.insert(std::make_pair("eleven", 11));
    values_.insert(std::make_pair("twelve", 12));
    values_.insert(std::make_pair("thirteen", 13));
    values_.insert(std::make_pair("fourteen", 14));
    values_.insert(std::make_pair("fifteen", 15));
    values_.insert(std::make_pair("sixteen", 16));
    values_.insert(std::make_pair("seventeen", 17));
    values_.insert(std::make_pair("eighteen", 18));
    values_.insert(std::make_pair("nineteen", 19));
  }

  const uint32_t ttn::converter::operator [](const char* s) {
    return values_[s];
  }

  std::vector<string_ref> string_to_string_ref_vec(const std::string& s) {
    std::vector<string_ref> vec;
    const char* start = nullptr;
    enum state_t {
      BEFORE,
      WORD,
      SPACE
    } state = BEFORE;

    for(size_t i = 0; i < s.size(); ++i) {
      switch(state) {
       case BEFORE:
        if(s[i] == ' ')
          break;
        else {
          state = WORD;
          start = &s[i];
          break;
        }
        break;
       case WORD:
        if(s[i] == ' ') {
          vec.push_back(string_ref(start, &s[i] - start));
          state = SPACE;
          break;
        }
        break;
       case SPACE:
        if(s[i] == ' ')
          break;
        else {
          state = WORD;
          start = &s[i];
          break;
        }
        break;
       default:
         assert(false);
      }
    }
    if(s[s.size()-1] != ' ')
      vec.push_back(string_ref(start, &s[s.size()-1] - start + 1));
    return vec;
  }

  const int32_t 
  ttn::converter::operator()(const std::string& text) {
    return convert_helper(string_to_string_ref_vec(text));
  }

  int32_t converter::convert_helper(const std::vector<string_ref>& ar) {
    uint32_t result = 0;
    uint32_t tmp = 0;
    uint32_t mult = 1;
    uint32_t hundred_mult = 1;
    bool require_mult = false;
    bool neg = false;
    
    if(ar[0].to_string() == "negative")
      neg = true;
    size_t down_ind = (size_t)(neg ? 1 : 0);

    for(size_t i = ar.size() - 1; i >= down_ind; --i) {
      assert(values_.find(ar[i]) != values_.end());
      uint32_t cur = values_[ar[i]];

      switch(cur) {
      case 1000000:
      case 1000:
      case 100:
        if(cur == 1000)
          hundred_mult = 1000;
        else if(cur == 1000000)
          hundred_mult = 1000000;
        if(!require_mult)
          require_mult = true;
        else {
          result += tmp*mult;
          tmp = 0;
        }
        mult = cur;
        break;
      default:
        if(!require_mult) 
          result += cur;
        else {
          if(mult == 100)
            mult *= hundred_mult;
          tmp += cur;
          if(i == down_ind)
            result += tmp*mult;
        }
        break;
      }
      if(i == 0)
        break;
    }
    return neg ? -(int32_t)result : result;
  }
}

int main(int argc, const char* argv[]) {
  ttn::text_supplier ts(argv[1]);
  ttn::converter conv;

  while(!ts.eof()) {
    try {
      std::cout << conv(ts.get_line()) << std::endl;
    }
    catch(...) {}
  }
}