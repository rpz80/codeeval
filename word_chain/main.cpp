#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cassert>

namespace aux
{
  template<typename CharT = char>
  class basic_substring
  {
  public:
    basic_substring() 
      : begin_(nullptr), 
        end_(nullptr) 
    {}
    basic_substring(const std::basic_string<CharT>& s)
      : begin_(s.c_str()), 
        end_(s.c_str() + s.size())
    {}
    basic_substring(const CharT* b, const CharT* e)
      : begin_(b), end_(e)
    {}
  public:
    const std::size_t size() const 
    {
      assert(begin_);
      assert(end_);
      return (end_ - begin_)/sizeof(CharT);
    }
    const CharT* c_str() const {assert(begin_); return begin_;}
    const CharT* last() const {assert(end_); return end_-1;}
  private:
    const CharT* begin_, *end_;
  };

  template<typename CharT>
  inline std::ostream& operator << (std::ostream& os, const basic_substring<CharT>& s)
  {
    os.write(s.c_str(), s.size());
    return os;
  }

  template<typename CharT>
  using substring_vector_type = std::vector<basic_substring<CharT>>;

  using substring = basic_substring<char>;
  using sv_type = substring_vector_type<char>;

  template<typename CharT>
  substring_vector_type<CharT>& split_to_substrings(substring_vector_type<CharT>& vec, 
                                                    const std::basic_string<CharT>& str, 
                                                    CharT sep)
  {
    basic_substring<CharT> tmp;
    const CharT* begin = str.c_str();
    const size_t str_size = str.size();

    for(size_t i = 0; i < str_size; ++i)
    {
      if(str[i] == sep || i == str_size - 1)
      {
        if(i!= str_size - 1)
        {
          vec.push_back(basic_substring<CharT>(begin, &str[i]));
          begin = &str[i+1];
        }
        else
        {
          vec.push_back(basic_substring<CharT>(begin, &str[i+1]));
          break;
        }
      }
    }
    return vec;
  }
}

using counts_type = std::vector<size_t>;
using word_state_type = std::pair<bool, aux::substring>;
using word_state_vector_type = std::vector<word_state_type>;

const size_t VEC_RESERVE = 32;

void process(const word_state_type& prev, 
             word_state_vector_type vec, 
             size_t count, counts_type& counts)
{
  bool found = false;
  for(auto& ws : vec)
  {
    if(!ws.first && *ws.second.c_str() == *prev.second.last())
    {
      ++count;
      ws.first = true;
      process(ws,vec,count,counts);
      --count;
      ws.first = false;
    }
  }
  if(!found)
    counts.push_back(count);
}

const word_state_vector_type& 
substrs_to_ws(const aux::sv_type& substrs,
              word_state_vector_type& ws)
{
  ws.clear();
  for(const auto& s: substrs)
    ws.push_back(std::make_pair(false, s));
  return ws;
}

int main(int argc, const char* argv[])
{
  aux::substring_vector_type<char> substr_vec;
  word_state_vector_type ws_vec;
  counts_type counts;

  std::ifstream ifs(argv[1]);
  std::string line;

  substr_vec.reserve(VEC_RESERVE);
  ws_vec.reserve(VEC_RESERVE);
  counts.reserve(VEC_RESERVE*VEC_RESERVE);

  while(!ifs.eof())
  {
    std::getline(ifs, line);
    if(line.empty() || line == "\n")
      continue;
    substr_vec.clear();
    counts.clear();
    aux::split_to_substrings(substr_vec, line, ',');
    substrs_to_ws(substr_vec, ws_vec);
    for(size_t i = 0; i < ws_vec.size(); ++i)
    {
      ws_vec[i].first = true;
      process(ws_vec[i], ws_vec, 1, counts);
      //for(size_t c: counts)
      //  std::cout << c << " ";
      //std::cout << std::endl;
      for(auto& w : ws_vec)
        w.first = false;
    }
    const size_t max = *std::max_element(counts.begin(), counts.end());
    if(max == 1)
      std::cout << "None" << std::endl;
    else
      std::cout << max << std::endl;
  }
}