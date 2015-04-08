#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

namespace pt {
  enum ecode_t{
    E_OK = 0,
    E_INVALID_LEVEL = 1
  };
  const char* const E_OK_MES = "Success";
  const char* const E_INVALID_LEVEL_MES = "Invalid level value";

  struct error_type {
    error_type(const char* mes=E_OK_MES, ecode_t code=E_OK)
      : message(mes), code(code) {}
    const char* message;
    ecode_t code;
  };

  class builder {
  public:
    builder(int32_t level);
    builder();
  public:
    const error_type& error() {return error_;}
    void print(const bool with_line_breaks=false);
    std::vector<uint32_t>& result() {return result_;}
    void reset(int32_t level);
  private:
    void process();
    void setup(int32_t level);
  private:
    error_type error_;
    std::vector<uint32_t> v1_, v2_, result_;
    int32_t level_;
    bool first_vector_used_;
  };
}

namespace pt {
  builder::builder(int32_t level) : first_vector_used_(true) {
    setup(level);
  }

  builder::builder() : first_vector_used_(true) {}

  void builder::reset(int32_t level) {
    v1_.clear();
    v2_.clear();
    result_.clear();
    first_vector_used_ = true;
    setup(level);
  }

  void builder::setup(int32_t level) {
    level_ = level;

    if(level <= 0) {
      error_.code = E_INVALID_LEVEL;
      error_.message = E_INVALID_LEVEL_MES;
      return;
    }
    v1_.reserve(128);
    v2_.reserve(128);
    v1_.push_back(1);
    result_.reserve(512);
    result_.push_back(1);
    process();
  }

  void builder::process() {
    for(int32_t i = 0; i < level_-1; ++i) {
      std::vector<uint32_t>& cur_vec = first_vector_used_ ? v1_ : v2_;
      std::vector<uint32_t>& new_vec = first_vector_used_ ? v2_ : v1_;

      new_vec.clear();
      new_vec.push_back(1);
      result_.push_back(1);
      for(size_t j = 0; j < cur_vec.size()-1; ++j) {
        uint32_t tmp = cur_vec[j] + cur_vec[j+1];
        new_vec.push_back(tmp);
        result_.push_back(tmp);
      }
      new_vec.push_back(1);
      result_.push_back(1);
      first_vector_used_ = !first_vector_used_;
    }
  }

  void builder::print(const bool with_line_breaks) {
    if(with_line_breaks)
      assert(false);
    else {
      const size_t result_size = result_.size();
      for(size_t i = 0; i < result_.size(); ++i) {
        std::cout << result_[i];
        if(i != result_size - 1)
          std::cout << " ";
      }
    }
  }
}

int main(int argc, const char* argv[]) {
  if(argc != 2) {
    std::cerr << "Usage: pt <file-name>";
    return -1;
  }
  std::ifstream ifs(argv[1]);
  
  if(!ifs) {
    std::cerr << "File open failed";
    return 1;
  }
  pt::builder b;
  std::string line;

  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line.empty())
      continue;
    b.reset(std::stol(line));

    if(b.error().code != pt::E_OK) {
      std::cerr << b.error().message;
      continue;
    }
    else {
      b.print();
      std::cout << std::endl;
    }
  }
}