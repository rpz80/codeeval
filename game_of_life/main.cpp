#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

namespace gol {
  struct action {
    bool alive;
    size_t ind;
  };

  class game {
   public:
    void load_field(const char* fname);
    void print_field();
    size_t field_size() const {return field_.size();}
    void play(const size_t);
   private:
    size_t calc_alive_neighbors(const size_t index);
   private:
    bool has_row_above(const size_t index);
    bool has_row_under(const size_t index);
    bool has_column_left(const size_t index);
    bool has_column_right(const size_t index);
   private:
    size_t alive_s(const size_t index);
    size_t alive_n(const size_t index);
    size_t alive_w(const size_t index);
    size_t alive_e(const size_t index);
    size_t alive_ne(const size_t index);
    size_t alive_nw(const size_t index);
    size_t alive_se(const size_t index);
    size_t alive_sw(const size_t index);
   private:
    uint32_t field_dim_;
    std::vector<uint8_t> field_;
    std::vector<action> actions_;
   private:
    friend bool operator == (const game& g, const char* s);
  };
}

namespace gol {
  void game::load_field(const char* fname) {
    std::ifstream ifs(fname);
    if(!ifs)
      return;
    std::string line;
    bool first = true;

    while (!ifs.eof()) {
      std::getline(ifs, line);
      if (line.empty()) {
        continue;
      }
      if (first) {
        first = false;
        field_dim_ = line.size();
        field_.reserve(field_dim_*field_dim_);
      }
      for (char c : line) {
        if (c == '*' || c == '.')
          field_.push_back(c);
        else {
          field_.clear();
          return;
        }
      }
    }
  }

  void gol::game::print_field() {
    for (size_t i = 0; i < field_.size(); ++i) {
      std::cout << field_[i];
      if ((i + 1) % field_dim_ == 0)
        std::cout << '\n';
    }
  }

  void gol::game::play(const size_t times) {
    size_t alive_neighbors;

    for(size_t k = 0; k < times; ++k) {
      actions_.clear();

      for(size_t i = 0; i < field_size(); ++i) {
        alive_neighbors = calc_alive_neighbors(i);
        if(field_[i] == '*') {
          if(alive_neighbors < 2 || alive_neighbors > 3) {
            actions_.push_back({false, i});
          } 
          else {
            actions_.push_back({true, i});
          }
        }
        else {
          if(alive_neighbors == 3) {
            actions_.push_back({true, i});
          }
        }
      }

      for(size_t i = 0; i < actions_.size(); ++i) {
        if(actions_[i].alive)
          field_[actions_[i].ind] = '*';
        else
          field_[actions_[i].ind] = '.';
      }
    }
  }

  size_t gol::game::calc_alive_neighbors(const size_t index) {
    return alive_e(index) + alive_n(index) + alive_ne(index) +
           alive_nw(index) + alive_s(index) + alive_se(index) +
           alive_sw(index) + alive_w(index);
  }

  size_t gol::game::alive_s(const size_t index) {
    if(!has_row_under(index))
      return 0;
    return field_[index + field_dim_] == '*' ? 1 : 0;
  }
  
  size_t gol::game::alive_n(const size_t index) {
    if (!has_row_above(index))
      return 0;
    return field_[index - field_dim_] == '*' ? 1 : 0;
  }

  size_t gol::game::alive_w(const size_t index) {
    if (!has_column_left(index))
      return 0;
    return field_[index - 1] == '*' ? 1 : 0;
  }

  size_t gol::game::alive_e(const size_t index) {
    if (!has_column_right(index))
      return 0;
    return field_[index + 1] == '*' ? 1 : 0;
  }

  size_t gol::game::alive_ne(const size_t index) {
    if (!has_row_above(index) || !has_column_right(index))
      return 0;
    return field_[index - field_dim_ + 1] == '*' ? 1 : 0;
  }

  size_t gol::game::alive_nw(const size_t index) {
    if (!has_row_above(index) || !has_column_left(index))
      return 0;
    return field_[index - field_dim_ - 1] == '*' ? 1 : 0;
  }

  size_t gol::game::alive_se(const size_t index) {
    if (!has_row_under(index) || !has_column_right(index))
      return 0;
    return field_[index + field_dim_ + 1] == '*' ? true : false;
  }

  size_t gol::game::alive_sw(const size_t index) {
    if (!has_row_under(index) || !has_column_left(index))
      return 0;
    return field_[index + field_dim_ - 1] == '*' ? true : false;
  }

  bool gol::game::has_row_above(const size_t index){
    if(index < field_dim_)
      return false;
    return true;
  }

  bool gol::game::has_row_under(const size_t index){
    if(index >= field_dim_*(field_dim_ - 1))
      return false;
    return true;
  }

  bool gol::game::has_column_left(const size_t index){
    if(index % field_dim_ == 0)
      return false;
    return true;
  }

  bool gol::game::has_column_right(const size_t index){
    if((index + 1) % field_dim_ == 0)
      return false;
    return true;
  }
  
  bool operator == (const game& g, const char* s) {
    if(g.field_size() != std::strlen(s))
      return false;
    for(size_t i = 0; i < g.field_size(); ++i) {
      if (*s == '\0' || g.field_[i] != *s) {
        return false;
      }
      ++s;
    }
    return true;
  }
}

int main(int argc, const char* argv[]) {
  gol::game g;
  g.load_field(argv[1]);
  g.play(10);
  g.print_field();
}