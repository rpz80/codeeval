#include <fstream>
#include <cstdio>
#include <array>
#include <cstdint>
#include <string>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <sstream>
#include <chrono>

namespace cl {
  // for tests
  class field;

  namespace test {
    void test_field();
  }

  // interface
  class field {
    struct tile {
      uint32_t val;
      bool merged;
    };
  public:
    static const uint32_t max_size = 128;
    enum class direction {
      up,
      down,
      left,
      right,
      unknown
    };
  public:
    field(const uint32_t size, std::initializer_list<uint32_t> l);
    field(const uint32_t size);
    field();
  public:
    void move(direction dir);
    void append(uint32_t val);
    void print(bool wlb = false) const;
    void set_size(const uint32_t size) {size_ = size;}
  private:
    uint32_t& at(uint32_t row, uint32_t col);
    bool& at_merged(uint32_t row, uint32_t col);
    void move_tile(uint32_t row, uint32_t col, direction dir);

    friend void test::test_field();
  private:
    uint32_t size_;
    uint32_t cur_;
    std::array<tile, max_size> field_;
  };

  std::pair<field, field::direction> 
  from_codeeval_string(const std::string& s);

  // implementation
  std::pair<field, field::direction> 
  from_codeeval_string(const std::string& s) {
    std::stringstream ss(s);
    field::direction dir = field::direction::unknown;

    std::string word;
    ss >> word;
    
    if (word == "RIGHT;")
      dir = field::direction::right;
    else if (word == "LEFT;")
      dir = field::direction::left;
    else if (word == "UP;")
      dir = field::direction::up;
    else if (word == "DOWN;")
      dir = field::direction::down;

    enum class parse_state {
      size,
      elems
    } st = parse_state::size;
    
    field f;
    uint32_t tmp;

    while (!ss.eof()) {
      char c = ss.get();

      if (std::isdigit(c)) {
        ss.unget();
        ss >> tmp;
        switch (st) {
          case parse_state::size:
            f.set_size(tmp);
            st = parse_state::elems;
            break;
          case parse_state::elems:
            f.append(tmp);
            break;
        }
      }
    }

    return std::make_pair(f, dir);
  }

  field::field(const uint32_t size, std::initializer_list<uint32_t> l)
      : size_(size),
        cur_(0) {
    assert(l.size() <= max_size);
    
    int i = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
      field_[i++] = {*it, false};
    }
  }

  field::field(const uint32_t size)
    : size_(size), cur_(0) {}

  field::field() : size_(0), cur_(0) {}

  void field::append(uint32_t val) {
    assert(cur_ < size_*size_);
    field_[cur_++] = {val, false};
  }

  void field::print(bool wlb) const {
    for (size_t i = 0; i < size_; ++i) {
      for (size_t j = 0; j < size_; ++j) {
        std::cout << field_[i*size_ + j].val;
        if (j != size_ - 1) {
          std::cout << ' ';
        }
      }
      if (i != size_ - 1 && !wlb) {
        std::cout << '|';
      }
      else {
        std::cout << std::endl;
      }
    }
  }

  uint32_t& field::at(uint32_t row, uint32_t col) {
    assert(row < size_ && col < size_);
    return field_[row*size_ + col].val; 
  }

  bool& field::at_merged(uint32_t row, uint32_t col) {
    assert(row < size_ && col < size_);
    return field_[row*size_ + col].merged;
  }

  void field::move(field::direction dir) {
    for (size_t i = 1; i < size_; ++i) {
      for (size_t j = 0; j < size_; ++j) {
        switch (dir) {
          case direction::up:
            move_tile(i, j, dir);
            break;
          case direction::down:
            move_tile(size_ - i - 1, j, dir);
            break;
          case direction::left:
            move_tile(j, i, dir);
            break;
          case direction::right:
            move_tile(j, size_ - i - 1, dir);
        }
      }
    }
  }

  void field::move_tile(uint32_t row, uint32_t col, direction dir) {
    auto& cur = at(row, col);
    if (cur == 0) {
      return;
    }

    switch (dir) {
      case direction::up: {
        int last_zero_row = row;
        bool found = false;
        
        for (int i = row - 1; i >= 0; --i) {
          auto& next = at(i, col);
          auto& next_merged = at_merged(i, col);

          if (next == cur) {
            if (next_merged) {
              break;
            }
            found = true;
            next = cur*2;
            cur = 0;
            next_merged = true;
            break;
          }
          else if (next == 0) {
            --last_zero_row;
          }
          else if (next != 0 && next != cur) {
            found = true;
            if (last_zero_row != row) {
              at(last_zero_row, col) = cur;
              cur = 0;
            }
            break;
          }
        }

        if (!found) {
          if (last_zero_row != row) {
            at(last_zero_row, col) = cur;
            cur = 0;
          }
          break;
        }
        break;
      }
      case direction::down: {
        int last_zero_row = row;
        bool found = false;

        for (int i = row + 1; i < (int)size_; ++i) {
          auto& next = at(i, col);
          auto& next_merged = at_merged(i, col);

          if (next == cur) {
            if (next_merged) {
              break;
            }
            found = true;
            next = cur * 2;
            cur = 0;
            next_merged = true;
            break;
          }
          else if (next == 0) {
            ++last_zero_row;
          }
          else if (next != 0 && next != cur) {
            found = true;
            if (last_zero_row != row) {
              at(last_zero_row, col) = cur;
              cur = 0;
            }
            break;
          }
        }

        if (!found) {
          if (last_zero_row != row) {
            at(last_zero_row, col) = cur;
            cur = 0;
          }
          break;
        }
        break;
      }
      case direction::left: {
        int last_zero_col = col;
        bool found = false;

        for (int i = col - 1; i >= 0; --i) {
          auto& next = at(row, i);
          auto& next_merged = at_merged(row, i);

          if (next == cur) {
            if (next_merged) {
              break;
            }
            found = true;
            next = cur * 2;
            cur = 0;
            next_merged = true;
            break;
          }
          else if (next == 0) {
            --last_zero_col;
          }
          else if (next != 0 && next != cur) {
            found = true;
            if (last_zero_col != col) {
              at(row, last_zero_col) = cur;
              cur = 0;
            }
            break;
          }
        }

        if (!found) {
          if (last_zero_col != col) {
            at(row, last_zero_col) = cur;
            cur = 0;
          }
          break;
        }
        break;
      }
      case direction::right: {
        int last_zero_col = col;
        bool found = false;

        for (int i = col + 1; i < (int)size_; ++i) {
          auto& next = at(row, i);
          auto& next_merged = at_merged(row, i);

          if (next == cur) {
            if (next_merged) {
              break;
            }
            found = true;
            next = cur * 2;
            cur = 0;
            next_merged = true;
            break;
          }
          else if (next == 0) {
            ++last_zero_col;
          }
          else if (next != 0 && next != cur) {
            found = true;
            if (last_zero_col != col) {
              at(row, last_zero_col) = cur;
              cur = 0;
            }
            break;
          }
        }

        if (!found) {
          if (last_zero_col != col) {
            at(row, last_zero_col) = cur;
            cur = 0;
          }
          break;
        }
        break;
      }
    }
  }


  // tests
  namespace test {
#define REQUIRE(expr) \
    if (!(expr)) { \
      std::cout << __FILE__ << ":"   << __LINE__ \
                << " ("     << #expr << ") FAILED\n"; \
      std::exit(EXIT_FAILURE); \
    }

    void test_field() {
      field f_(4, {4, 0, 2, 0, 0, 0, 0, 8, 4, 0, 2, 4, 2, 4, 2, 2});

      field f1 = f_;
      REQUIRE(f1.size_ == 4);
      REQUIRE(f1.field_[0].val == 4 && f1.field_[15].val == 2);
      f1.print(true);

      f1.move(field::direction::up);
      std::cout << std::endl;
      f1.print(true);
      REQUIRE(f1.field_[0].val == 8);
      REQUIRE(f1.at(1, 0) == 2);
      REQUIRE(f1.at(2, 0) == 0 && f1.at(3, 0) == 0);

      field f2 = f_;
      f2.move(field::direction::down);
      std::cout << std::endl;
      f2.print(true);
      REQUIRE(f2.at(3, 0) == 2);
      REQUIRE(f2.at(2, 0) == 8);
      REQUIRE(f2.at(2, 2) == 2);
      REQUIRE(f2.at(3, 2) == 4);

      field f3 = f_;
      f3.move(field::direction::left);
      std::cout << std::endl;
      f3.print(true);
      REQUIRE(f3.at(0, 0) == 4);
      REQUIRE(f3.at(2, 0) == 4);
      REQUIRE(f3.at(2, 2) == 4);
      REQUIRE(f3.at(3, 2) == 4);

      field f4 = f_;
      f4.move(field::direction::right);
      std::cout << std::endl;
      f4.print(true);
      REQUIRE(f4.at(0, 0) == 0);
      REQUIRE(f4.at(1, 3) == 8);
      REQUIRE(f4.at(2, 2) == 2);
      REQUIRE(f4.at(3, 2) == 4);
    }
  }
}

int main(int argc, const char* argv[]) {
  //cl::test::test_field();
  auto start = std::chrono::high_resolution_clock::now();

  std::string line;
  std::ifstream ifs(argv[1]);

  if (!ifs) 
    return 1;

  while (!ifs.eof()) {
    std::getline(ifs, line);
    auto p = cl::from_codeeval_string(line);
    p.first.move(p.second);
    p.first.print();
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "Elapsed: " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << "ms" << std::endl;
}