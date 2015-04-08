#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const size_t STR_SIZE = 32;

auto process = [](const std::string& ex, 
                  const std::string& pat, 
                  std::vector<int>& table) {
  //std::cout << "examined: " << ex << ",  pattern: " << pat << std::endl;
  table.clear();
  
  size_t ex_size = ex.size();
  size_t pat_size = pat.size();
    
  table.resize(ex_size*pat_size);
    
  for(size_t i = 0; i < ex_size*pat_size; ++i) {
    table[i] = 0;
  }
  
  if(ex[0] == pat[0]) table[0] = 1;
  
  for(size_t j = 0; j < pat_size; ++j) {
    for(size_t i = 1; i < ex_size; ++i) {
      table[j*ex_size + i] = table[j*ex_size + i - 1];
      
      if(ex[i] == pat[j]) {
        if(j == 0)
          table[j*ex_size + i] += 1;
        else {
          table[j*ex_size + i] += table[(j-1)*ex_size + i - 1];
        }        
      }
    }
  }
  
  // for(size_t j = 0; j < pat_size; ++j) {
    // for(size_t i = 0; i < ex_size; ++i) {
      // std::cout << table[j*ex_size + i] << " ";
    // }
    // std::cout << std::endl;
  // }
  return table[ex_size*pat_size - 1];
};

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line, examined, pattern;
  std::vector<int> table;
  
  table.reserve(STR_SIZE*STR_SIZE);  
  line.reserve(STR_SIZE); 
  examined.reserve(STR_SIZE);
  pattern.reserve(STR_SIZE);
  
  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line == "\n" || line.empty()) {
      continue;
    }
    
    size_t delim_pos = line.find(',');
    if(delim_pos == std::string::npos)
      continue;
    
    examined.assign(line.begin(), line.begin() + delim_pos);
    pattern.assign(line.begin() + delim_pos + 1, line.end());
    
    std::cout << process(examined, pattern, table) << std::endl;
  }
}