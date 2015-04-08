#include <iostream>
#include <fstream>

bool is_substr(const char* l, const char* r) {
  const char* left = l;
  const char* right = r;
  
  bool asteriks = false;
  bool slash = false;
  
  while(*l != '\0') {    
    while(*right != 0) {
      slash = false;
      
      if(*right == '\\') {
        ++right;
        slash = true;
      }
      
      if(*right == '*' && !slash) {
        asteriks = true;
      }  
      
      if(!asteriks) {
        if(*left++ == *right++) {
          if(*left == '\0') {
            if(*right == '\0')
              return true;
          } 
          else if(*right == '\0')
              return true;
          else if(*right == '*' && !slash) {
            asteriks = true;
            continue;
          }  
        } 
        else {
          left = ++l;
          right = r;
          break;
        }

      } else {
        char after_asteriks = *(right + 1);
        if(after_asteriks == '\0') return true;

        if(after_asteriks == *left) {
          asteriks = false;
          ++right;
        } else {
          if(*(left + 1) == '\0') {
            return false;
          }
          ++left;
        }
      }
    } // inner while
  } // outer while
  return false;
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line;
  
  while(!ifs.eof()) {
    std::getline(ifs, line);
    
    size_t comma_ind = line.find(',');
    if(
      is_substr(line.substr(0, comma_ind).c_str(), 
                line.substr(comma_ind + 1, line.size()).c_str())
      ) std::cout << "true\n";
    else std::cout << "false\n";
  }
}
