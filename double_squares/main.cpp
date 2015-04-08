#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void process(size_t num)
{
  size_t max_perf_square;
  size_t count = 0;
  size_t min_ps = 0;
  
  for(size_t i = 0; ; ++i)
  {
    if(i*i > num)
    {
      max_perf_square = i-1;
      break;
    }
  }
  // std::cout << "max ps = " << max_perf_square << std::endl;
  
  while(max_perf_square*max_perf_square*2 >= num)
  {
    size_t i = min_ps;
    auto max_ps = max_perf_square*max_perf_square;
    while(true)
    {
      size_t tmp = i*i + max_ps;
      
      // std::cout << "max ps = " << max_perf_square << " i = " << i <<std::endl;
      
      if(tmp > num)
      {      
        break;
        min_ps = i+1;
      }
      else if(tmp == num)
      {
        // std::cout << "bingo!\n";
        ++count;
        min_ps = i+1;
        break;
      }
      ++i;
    }
    --max_perf_square;
    ++min_ps;
  }
  std::cout << count << std::endl;
}

int main(int argc, const char* argv[])
{
  std::string line;
  std::ifstream ifs(argv[1]);
  std::stringstream ss;
  size_t num;
  std::getline(ifs, line);  
  
  while(!ifs.eof())
  {
    std::getline(ifs, line);
    ss.clear();
    ss.str(line);
    ss >> num;
    if(num == 0 || num == 1)
      std::cout << 1 << std::endl;
    else
      process(num);
  }
}