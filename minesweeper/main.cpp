#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <iostream>

//#define MSWP_DEBUG

const size_t BUF_SIZE = 64;

class minesweep {
public:
  minesweep() : m_(0), n_(0) { field_.reserve(BUF_SIZE); }
  void process(const std::string& line);
private:
  void fill_field(const std::string& line);
  void print_pretty();
private:
  std::stringstream ss_;
  std::vector<int> field_;
  int m_, n_;
};

void minesweep::print_pretty()
{
  for(auto i : field_) {
    if(i == -1)
      std::cout << "*";
    else std::cout << i;  
  }
  std::cout << std::endl;
}

void minesweep::process(const std::string& line) 
{
  fill_field(line);
  
  auto check_and_inc_same = [&](int init_index, int check_index, int row) {
#ifdef MSWP_DEBUG
    std::cout << "init index: " << init_index << " check index: " << check_index
      << " row: " << row  << std::endl;
#endif
    if(row < 0 || row > m_) return;
    
    if(check_index - init_index > 0) {
      if(check_index < m_*n_ && check_index < n_*(row + 1) && field_[check_index] != -1)
        ++field_[check_index];
    } else if(check_index - init_index < 0) {
      if(check_index >= row*n_ && field_[check_index] != -1)
        ++field_[check_index];
    } else {
      if(field_[check_index] != -1)
        ++field_[check_index];
    }
  };
  
  auto inc_mine_counter = [&](int index, int row) 
  {
    check_and_inc_same(index, index - 1, row);
    check_and_inc_same(index, index + 1, row);
    
    check_and_inc_same(index + n_, index + n_, row + 1);
    check_and_inc_same(index + n_, index + n_ - 1, row + 1);
    check_and_inc_same(index + n_, index + n_ + 1, row + 1);    

    check_and_inc_same(index - n_, index - n_, row - 1);
    check_and_inc_same(index - n_, index - n_ - 1, row - 1);
    check_and_inc_same(index - n_, index - n_ + 1, row - 1);        
  };
  
  for(int i = 0; i < m_; ++i) {
    for(int j = 0; j < n_; ++j) {
      if(field_[j + i*n_] == -1) {
        inc_mine_counter(j + i*n_, i);
      }
    }
  }
  
  print_pretty();
  
}

void minesweep::fill_field(const std::string& line) 
{
  ss_.clear();
  ss_.str(line);

  field_.clear();

  char c;
  bool has_sep = false;
  bool has_m   = false;
  
  while(!ss_.eof()) {
    c = ss_.get();
    switch(has_sep) {
      case false: {
        if(std::isdigit(c)) {
          ss_.unget();
          switch(has_m) {
            case false:
              ss_ >> m_;
              has_m = true;
              break;
            case true:
              ss_ >> n_;
              break;
          }
        } else if(c == ';') {
          has_sep = true;
        }
        break;
      }
      case true: {
        if(c == '*') {
          field_.push_back(-1);
        } else if(c == '.'){
          field_.push_back(0);
        }
      }
    }
  }
#ifdef MSWP_DEBUG
  std::cout <<    "m = " << m_ 
            << " | n = " << n_ 
            << std::endl << "field: ";

  for(int i : field_)
    std::cout << i << " ";
  std::cout << std::endl;  
#endif  
}

int main(int argc, const char* argv[]) 
{
  std::ifstream  ifs(argv[1]);
  
  if(!ifs) {
    std::cerr << "File open failed\n";
    return 1;
  }
  
  std::string    line;
  minesweep      msw;
  
  line.reserve(BUF_SIZE);
  
  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line == "\n" || line.empty())
      continue;
    msw.process(line);
  }
}



