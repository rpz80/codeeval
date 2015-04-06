#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cctype>
#include <algorithm>
#include <limits>
#include <cmath>
#include <ctime>

namespace mn {
  struct point {
    uint32_t x;
    uint32_t y;
    bool visited;
  };

  typedef std::vector<point> point_vector_type;
  typedef point_vector_type::iterator iterator_type;

  class calc {
  public:
    calc() : len_(0) {
      points_.reserve(64);
    }
   public:
    void reset(const std::string& s);
    uint32_t len() const;
   private:
    void calc_len();
    double find_min_len(iterator_type it);
   private:
    point_vector_type points_;
    double len_;
  };

  uint32_t calc::len() const {return std::ceil(len_);}

  template<typename Iter>
  uint32_t range_to_uint(Iter& begin, Iter end) {
    if(begin == end) {
      return 0;
    }
    uint32_t result = 0;
    for(; begin != end; ++begin) {
      if(!std::isdigit(*begin)) {
        ++begin;
        return result;
      }
      result = result*10 + *begin - 48;
    }
    return result;
  }

  uint32_t sq_len(uint32_t x1, uint32_t x2, uint32_t y1, uint32_t y2) {
    return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
  }

  double calc::find_min_len(iterator_type it) {
    double result = std::numeric_limits<double>::max();
    for(auto p_it = points_.begin(); p_it != points_.end(); ++p_it) {
      if(p_it->visited) {
        auto l = sq_len(it->x, p_it->x, it->y, p_it->y);
        if(l < result) {
          result = l;
        }
      }
    }
    return std::sqrt((double)result);
  }

  void calc::calc_len() {
    if(points_.empty()) {
      return;
    }
    points_[0].visited = true;
    double result = std::numeric_limits<double>::max();
    iterator_type it;
    while(std::find_if(points_.begin(), points_.end(), 
                             [](const point& p) {
                               return p.visited == false;
                             }) != points_.end()) {
      for(auto p_it = points_.begin(); p_it != points_.end(); ++p_it) {
        if(!p_it->visited) {
          auto p = find_min_len(p_it);
          if(p < result) {
            result = p;
            it = p_it;
          }
        }
      }
      len_ += result;
      result = std::numeric_limits<double>::max();
      it->visited = true;
    }
  }

  void calc::reset(const std::string& s) {
    if(s.empty())
      return;
    points_.clear();
    len_ = 0;
    auto b = s.begin();
    auto e = s.end();
    bool first = true;
    point tmp;
    tmp.visited = false;
    while(b != e) {
      switch (first) {
        case true:
          tmp.x = range_to_uint(b, e);
          first = false;
          break;
        case false:
          tmp.y = range_to_uint(b, e);
          if(std::find_if(points_.begin(), points_.end(),
                          [tmp](const point& p) {
                            return tmp.x == p.x && tmp.y == p.y;
                          }) == points_.end()) {
            points_.push_back(tmp);
          }
          first = true;
          break;
      }
    }
    calc_len();
  }
}

int main(int argc, const char* argv[]) {
  auto start = std::clock();
  if(argc != 2) {
    std::cerr << "Usage: mn <file-name>\n";
    return 1;
  }
  std::ifstream ifs(argv[1]);
  if(!ifs) {
    std::cerr << "File open error\n";
    return 1;
  }
  std::string line;
  mn::calc calc;
  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line.empty())
      continue;
    calc.reset(line);
    std::cout << calc.len() << std::endl;
  }
  std::cout << "Elapsed: " << (double)(std::clock() - start)*1000/CLOCKS_PER_SEC << "ms\n";
}