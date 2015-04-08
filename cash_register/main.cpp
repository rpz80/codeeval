#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>

enum cash_t : int {
    PENNY       =  1    ,
    NICKEL      =  5    ,
    DIME        =  10   ,
    QUARTER     =  25   ,
    HALF_DOLLAR =  50   ,
    ONE         =  100  ,
    TWO         =  200  ,
    FIVE        =  500  ,
    TEN         =  1000 ,
    TWENTY      =  2000 ,
    FIFTY       =  5000 ,
    ONE_HUNDRED =  10000
};

inline void print_change(int pp, int ch) {
  if(pp > ch) {
    std::cout << "ERROR\n";
    return;
  } else if(pp == ch) {
    std::cout << "ZERO\n";
    return;
  }
  
  int x = ch - pp;
  
  while(true) {
    if(x - ONE_HUNDRED >= 0) {
      std::cout << "ONE HUNDRED";
      x -= ONE_HUNDRED;
    } else if(x - FIFTY >= 0) {
      std::cout << "FIFTY";
      x -= FIFTY; 
    } else if(x - TWENTY >= 0) {
      std::cout << "TWENTY";
      x -= TWENTY; 
    } else if(x - TEN >= 0) {
      std::cout << "TEN";
      x -= TEN;
    } else if(x - FIVE >= 0) {
      std::cout << "FIVE";
      x -= FIVE; 
    } else if(x - TWO >= 0) {
      std::cout << "TWO";
      x -= TWO; 
    } else if(x - ONE >= 0) {
      std::cout << "ONE";
      x -= ONE; 
    } else if(x - HALF_DOLLAR >= 0) {
      std::cout << "HALF DOLLAR";
      x -= HALF_DOLLAR; 
    } else if(x - QUARTER >= 0) {
      std::cout << "QUARTER";
      x -= QUARTER; 
    } else if(x - DIME >= 0) {
      std::cout << "DIME";
      x -= DIME; 
    } else if(x - NICKEL >= 0) {
      std::cout << "NICKEL";
      x -= NICKEL; 
    } else if(x - PENNY >= 0) {
      std::cout << "PENNY";
      x -= PENNY; 
    }

    if(x < 0) {
      std::cout << "ERROR\n";
      return;
    } else if(x == 0) {
      std::cout << std::endl;
      return;
    } else {
      std::cout << ',';
      continue;
    }
  }
}
 
  
int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  size_t delim_pos = 0;
  
  while(!ifs.eof()) {
    std::getline(ifs, line);
    delim_pos = line.find(';');
    const char* lbeg = line.c_str();
    
    print_change((int)(std::ceil(std::strtod(lbeg, NULL)*100)),
                 (int)(std::ceil(std::strtod(lbeg + delim_pos + 1, NULL)*100)));
  }
}
