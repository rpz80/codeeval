#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::string strip(const std::string& s)
{
  std::string result;
  enum parse_state{PREF, SUF, BODY};
  
  parse_state state = PREF;
  size_t suf_pos = 0;
  
  for(size_t i = 0; i < s.size(); ++i)
  {
    char c = s[i];
    switch(state)
    {
      case PREF:
        if(c != ' ')
        {
          state = BODY;
          result.push_back(c);
        }
        break;
      case BODY:
        if(c == ' ')
        {
          state = SUF;
          suf_pos = i;
        }
        else result.push_back(c);
        break;
      case SUF:
        if(c != ' ')
        {
          state = BODY;
          for(size_t j = suf_pos; j <= i; ++j)
          {
            result.push_back(s[j]);
          }
        }
        break;
    }
  }
  return result;
}

struct batline
{
  using bat_vector_t = std::vector<size_t>;
  enum {EDGE_OFFSET = 6};
  
  size_t len;
  size_t dist;
  bat_vector_t bats;
  
  size_t calc_available()
  {
    const size_t bats_present = bats.size();
    size_t offset = EDGE_OFFSET;
    size_t count = 0;    
    size_t index = 0;
    
    while(offset <= len - EDGE_OFFSET)
    {
      if(bats_present != index)
      {
        if(offset > bats[index])
        {
          offset = bats[index++];
          --count;
        }
        offset += dist;
      }
      else
      {
        offset += dist;
      }
      ++count;
    }
    
    return count - bats_present;
  }
  
  void reset()
  {
    bats.clear();
  }
};

int main(int argc, const char* argv[])
{
  std::ifstream ifs(argv[1]);
  std::string line;
  std::stringstream ss;
  size_t tmp;
  batline bl;
  
  while(!ifs.eof())
  {
    std::getline(ifs, line);
    if(line == "\n" || line.empty())
      continue;
    
    ss.clear();
    ss.str(strip(line));
    
    ss >> std::skipws >> bl.len >> bl.dist >> tmp;

    while(!ss.eof())
    {
      ss >> tmp;
      bl.bats.push_back(tmp);
    }
    
    std::cout << bl.calc_available() << std::endl;
    bl.reset();
  }
}