#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include <cctype>

enum class parse_state {
  FIRST,
  SECOND,
  WORD
};

enum class process_state {
  NOT_STARTED,
  STARTED
};

typedef std::tuple<bool, int, int> record_type;
typedef std::vector<record_type> record_vector_type;

auto process = [](std::stringstream& ss)
{
  char c;
  auto state = parse_state::FIRST;
  auto last_state = state;
  record_vector_type records;
  record_type tmp;
  std::get<0>(tmp) = false;  
  enum begin_end {
    BEG = 01,
    END = 02
  };
  unsigned be_found = 0;
  
  while(!ss.eof())
  {
    c = ss.get();
    switch(state)
    {
      case parse_state::FIRST:
        if(std::isalpha(c))
        {
          if(c != 'B')
          {
            std::cout << "BAD\n";
            return;
          }
          std::get<1>(tmp) = -1;
          state = parse_state::WORD;
          last_state = parse_state::FIRST;
          if((be_found & 1) == BEG)
          {
            std::cout << "BAD\n";
            return;
          }
          be_found |= BEG;
          break;
        }
        else if(std::isdigit(c))
        {
          ss.unget();
          ss >> std::get<1>(tmp);
          state = parse_state::SECOND;
        }
        break;
      case parse_state::SECOND:
        if(std::isalpha(c))
        {
          if(c != 'E')
          {
            std::cout << "BAD\n";
            return;
          }
          std::get<2>(tmp) = -2;
          state = parse_state::WORD;
          last_state = parse_state::SECOND;
          if((be_found & 2) == END)
          {
            std::cout << "BAD\n";
            return;
          }
          be_found |= END;
          break;
        }
        else if(std::isdigit(c))
        {
          ss.unget();
          ss >> std::get<2>(tmp);
          state = parse_state::FIRST;
          records.push_back(tmp);
        }
        break;
      case parse_state::WORD:
        if(!std::isalpha(c))
        {
          if(last_state == parse_state::FIRST)
          {
            state = parse_state::SECOND;
          }
          else
          {
            state = parse_state::FIRST;
            records.push_back(tmp);
          }
        }
        break;
    } // switch
  } // while
  if(state == parse_state::WORD)
    records.push_back(tmp);
/*  
  for(const auto& r : records)
    std::cout << std::boolalpha 
              << "(" << std::get<0>(r) << ", " 
                     << std::get<1>(r) << ", "
                     << std::get<2>(r) << ")" << std::endl;
  std::cout << std::endl;
*/
  if(be_found != (BEG | END))
  {
    std::cout << "BAD\n";
    return;
  }
  
  const size_t records_size = records.size();
  size_t processed = 0;
  int last_in_chain = -100;
  bool found;
  
  while(true)
  {
    found = false;
    for(size_t i = 0; i < records_size; ++i)
    {
      auto& cur_rec = records[i];
      if((std::get<1>(cur_rec) == last_in_chain || std::get<1>(cur_rec) == -1) && !std::get<0>(cur_rec))
      {
        found = true;
        std::get<0>(cur_rec) = true;
        ++processed;
        last_in_chain = std::get<2>(cur_rec);
        if(std::get<2>(cur_rec) == -2)
        {
          if(processed == records_size)
          {
            std::cout << "GOOD\n";
            return;
          }
          else
          {
            std::cout << "BAD\n";
            return;
          }
        }
      }
      if (found) break;
    } // for
    if(!found)
    {
      std::cout << "BAD\n";
      return;
    }
  } // while
};

int main(int argc, const char* argv[])
{
  std::ifstream ifs(argv[1]);
  std::string line;
  std::stringstream ss;
  
  while(!ifs.eof())
  {
    std::getline(ifs, line);
    if(line == "\n" || line.empty())
      continue;
      
    ss.clear();
    ss.str(line);
    process(ss);
  }
}