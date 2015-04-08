#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <map>
#include <cstdio>

namespace client
{
  typedef std::vector<std::string> svec_t;
  typedef std::pair<std::string, bool> word_pair_type;
  typedef std::vector<std::string> wvec_t;
  typedef std::map<std::string, bool> wpmap_type;

  void populate_lists(std::istream& ifs, // input stream (typically file)
                      svec_t& tc, // test cases (vector<string>)
                      wvec_t& wl) { // word list (vector<bool, string>)
    bool is_wl = false;
    std::string line;
    line.reserve(64);
    tc.reserve(64); // test cases usuallly not large
    wl.reserve(8192); // word list is large, reserve more
    while(!ifs.eof()) {
      std::getline(ifs, line);
      if(line.empty())
        continue;      
      if(!is_wl) { // still reading test cases
        if(line != "END OF INPUT") // line is not a delimiter
          tc.push_back(line);
        else { // found delimiter
          is_wl = true; // set flag and
          continue;     // skip to next iteration
        }
      }
      else { // is_wl=false. test cases are finished
             // reading word list
        wl.push_back(line);
      }
    }
  }

// two-dimensional table emulator
  struct lev_table {
    lev_table() : x_(true) {}
    void clear() {
      for(char& c :tbl_)
        c = 0;
    }
    lev_table& operator[](size_t i) {
      if(x_) {
        m_ = i;
        x_ = false;
      } else {
        n_ = i;
        x_ = true;
      }
      return *this;
    }
    operator char&() {
      return tbl_[m_*32 + n_]; 
    }
    lev_table& operator = (char c) {
      tbl_[m_*32 + n_] = c;
      return *this;
    }
    lev_table& operator = (const lev_table&);
  private:
    char tbl_[1024];
    size_t m_, n_;
    bool x_;
  };

  lev_table& lt() {
    static lev_table lt_;
    return lt_;
  }

// calculate levenstein distance. If it is > 1 then
// words are not friends, otherwise they are  
  bool friends(const std::string& l, const std::string& r) {
    lev_table& t = lt();
    const size_t lsize = l.size();
    const size_t rsize = r.size();    
    t.clear();
    for(size_t i = 1; i < lsize + 1; ++i)
      t[i][0] = i;
    for(size_t i = 1; i < rsize + 1; ++i)
      t[0][i] = i;
    for(size_t j = 1; j < rsize + 1; ++j) {
      for(size_t i = 1; i < lsize + 1; ++i) {
        if(l[i-1] == r[j-1]) {
          t[i][j] = (char)t[i-1][j-1];
        } else {
          char min = std::min(t[i-1][j] + 1, t[i][j-1] + 1);
          t[i][j] = std::min((int)min, t[i-1][j-1] + 1);
        }
      }
    }
    return t[lsize][rsize] <= 1;
  }

  void find_friend_friends(const wvec_t& word_list,
                           wpmap_type& wpmap) {
    for(auto& wp : wpmap) {
      if(!wp.second) {
        wp.second = true;
        for(const std::string& s : word_list) {
          if(friends(wp.first, s) && wp.first != s) {
            wpmap.insert(std::make_pair(s, false));        
          }
        }
      }
    }
  }

  void process(const svec_t& test_cases, const wvec_t& word_list) {
    wpmap_type wpmap;
    for(const auto& tc: test_cases) {
      auto wp = std::make_pair(tc, true);
      if(wpmap.find(tc) != wpmap.end())
        continue;
      wpmap.insert(wp);
      for(const std::string& s : word_list) {
        if(friends(wp.first, s) && wp.first != s) {
          wpmap.insert(std::make_pair(s, false));        
        }
      }
      while(true) {
        find_friend_friends(word_list, wpmap);
        auto fit = std::find_if(wpmap.begin(), wpmap.end(),
                                [](const word_pair_type& wp) {
                                  return wp.second == false;
                                });
        if(fit == wpmap.end())
          break;
      }
      std::printf("%u\n", wpmap.size());
      wpmap.clear();
    }
    // for(const auto& wp : wpmap) {
    //   std::cout << wp.first << " ";
    // }
    // std::cout << std::endl;
  }
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  client::svec_t test_cases;
  client::wvec_t word_list;
  client::populate_lists(ifs, test_cases, word_list);
  client::process(test_cases, word_list);  
}