#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

enum coin_state {
  FIVE  = 5,
  THREE = 3,
  ONE   = 1
};

inline
void process(
  int num
)
{
  size_t count = 0;
  int sum = 0;
  coin_state state(FIVE);
  
  while(sum != num)
  {
    switch(state)
    {
      case FIVE:
        if(sum + FIVE > num)
        {
          state = THREE;
          break;
        }
        else
        {
          sum += FIVE;
          ++count;
        }
        break;
      case THREE:
        if(sum + THREE > num)
        {
          state = ONE;
          break;
        }
        else
        {
          sum += THREE;
          ++count;
        }
        break;
      case ONE:
        sum += ONE;
        ++count;
        break;
    }
  }
  std::cout
    << count
    << std::endl;
}

int main(
  int argc,
  const char* argv[]
)
{
  std::ifstream 
  ifs(
    argv[1]
  );
  
  std::string line;
  
  while(!ifs.eof())
  {
    std::getline(
      ifs, 
      line
    );
    
    if(
      line == "\n" ||
      line.empty()
    )
      continue;
      
    process(
      std::strtol(
        line.c_str(),
        NULL,
        10
      )
    );
  }
}