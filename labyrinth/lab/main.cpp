#include <cstring>
#include <cstdio>
#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdint>
#include <fstream>

void test();

namespace lb {
  class labyrinth {
  private:
    friend void ::test();
  public:
    struct cell {
      uint32_t* c;
      size_t i;
      size_t j;
    };

    struct neighboors {
      cell top;
      cell bottom;
      cell left;
      cell right;
    };
  public:
    labyrinth(const char* str, size_t size) 
        : m_(0),
          n_(0) {
      const char* s = str;
      while(true) {
        if (*s != '*' && *s != ' ') {
          break;
        }
        if(*s == ' ') {
          start_col_ = n_;
        }
        ++n_;
        ++s;
      }
      
      s = str;
      for(;s < str + size; ++s) {
        if (*s =='\n') {
          ++m_;
        }
      }

      // check if '\n' is on the last line
      bool break_last_line = false;
      s = str + size - 1;
      for(size_t i = 0; i < 5; ++i) {
        if(*(s - i) == '\n') {
          break_last_line = true;
        }
      }
      if(!break_last_line)
        ++m_;

      s_.reserve(m_*n_);
      std::copy_if(str, str + size, 
                   std::back_inserter(s_),
                   [](char c) {
                     return c == '*' || c == ' ';
                   });

      for(size_t i = s_.size() - n_; i < s_.size(); ++i) {
        if(s_[i] == ' ') {
          end_col_ = i - (s_.size() - n_);
          break;
        }
      }
    }
  public:
    neighboors get_neighboors(cell* cell_) {      
      assert(cell_);
      assert(!(cell_->i < 0 || cell_->i >= m_ || cell_->j < 0 || cell_->j >= n_));

      cell topc;
      if(cell_->i == 0) {
        topc = {nullptr, 0, 0};
      }
      else {
        topc = at(cell_->i-1, cell_->j);
      }

      cell bottomc;
      if(cell_->i == m_ - 1) {
        bottomc = {nullptr, 0, 0};
      }
      else {
        bottomc = at(cell_->i+1, cell_->j);
      }

      cell leftc;
      if(cell_->j == 0) {
        leftc = {nullptr, 0, 0};
      }
      else {
        leftc = at(cell_->i, cell_->j-1);
      }

      cell rightc;
      if(cell_->j == n_ - 1) {
        rightc = {nullptr, 0, 0};
      }
      else {
        rightc = at(cell_->i, cell_->j+1);
      }

      return {topc, bottomc, leftc, rightc};
    }

    cell at(size_t i, size_t j) {
      assert(!(i < 0 || i >= m_ || j < 0 || j >= n_));
      return {&s_[i*n_ + j], i, j};
    }

    void print() {
      for(size_t i = 0; i < m_; ++i) {
        for(size_t j = 0; j < n_; ++j) {
          uint32_t tmp = s_[i*n_ + j];
          if(tmp == 42) {
            std::printf("%c", '*');
          }
          else if(tmp == 43) {
            std::printf("%c", '+');
          }
          else {
            std::printf("%c", ' ');
          }
        }
        std::printf("\n");
      }
    }

    void populate_paths() {
      auto start = at(0, start_col_);
      *start.c = '0';

      do_populate(&start);
    }

    void mark_path() {
      auto cur = at(m_ - 1, end_col_);

      while(true) {
        if(cur.i == 0 && cur.j == start_col_)
          break;

        auto nbrs = get_neighboors(&cur);

        if(nbrs.bottom.c && *nbrs.bottom.c != '*' && 
           *nbrs.bottom.c < *cur.c && *nbrs.bottom.c != '+') {
          *cur.c = '+';
          cur = nbrs.bottom;
          continue;
        }

        if(nbrs.right.c && *nbrs.right.c != '*' && 
           *nbrs.right.c < *cur.c && *nbrs.right.c != '+') {
          *cur.c = '+';
          cur = nbrs.right;
          continue;
        }

        if(nbrs.left.c && *nbrs.left.c != '*' && 
           *nbrs.left.c < *cur.c && *nbrs.left.c != '+') {
          *cur.c = '+';
          cur = nbrs.left;
          continue;
        }

        if(nbrs.top.c && *nbrs.top.c != '*' && 
           *nbrs.top.c < *cur.c && *nbrs.top.c != '+') {
          *cur.c = '+';
          cur = nbrs.top;
          continue;
        }
      }
      *cur.c = '+';
    }
  private:
    void do_populate(cell* c) {
      assert(c);
      auto nbrs = get_neighboors(c);

      if(nbrs.top.c != nullptr && *nbrs.top.c != '*') { 
        if(*nbrs.top.c == ' ' || *nbrs.top.c > *c->c + 1) {
          *nbrs.top.c = *c->c + 1;
           do_populate(&nbrs.top); 
        } 
      }

      if(nbrs.bottom.c != nullptr && *nbrs.bottom.c != '*') { 
        if(*nbrs.bottom.c == ' ' || *nbrs.bottom.c > *c->c + 1) {
          *nbrs.bottom.c = *c->c + 1;
           do_populate(&nbrs.bottom); 
        } 
      }

      if(nbrs.left.c != nullptr && *nbrs.left.c != '*') { 
        if(*nbrs.left.c == ' ' || *nbrs.left.c > *c->c + 1) {
          *nbrs.left.c = *c->c + 1;
           do_populate(&nbrs.left); 
        } 
      }

      if(nbrs.right.c != nullptr && *nbrs.right.c != '*') { 
        if(*nbrs.right.c == ' ' || *nbrs.right.c > *c->c + 1) {
          *nbrs.right.c = *c->c + 1;
           do_populate(&nbrs.right); 
        } 
      }

    }
  private:
    size_t m_;
    size_t n_;
    size_t start_col_;
    size_t end_col_;
    std::vector<uint32_t> s_;
    size_t size_;
  };
}

void test() {
  const char* const s = 
R"(************************* *************************
*                                   * *           *
* * *** *** ******************* ***** * * * * * ***
* * *   * *   *   * * *                 * * * *   *
***** * * *** * *** * * *** *** * ***** *** *******
*     * *   * *     *   * * *   *     * * *       *
*** ******* * ***** *** * * ******* * *** * *** * *
* *     *     *   *     *     *     * *       * * *
* * *********** * ********* * ******* * *** * *****
*     * *   * * *     *     * *   *   * *   *     *
* ***** * *** * ***** *** *** * * * ******* ***** *
* *     *   * * *       * * *   * * * *   *     * *
* * ***** *** *** *** *** * ***** *** *** ***** ***
*     *   * * *     * *       * *       * *     * *
* * ***** * * * *** * *** ***** *** ***** *** * * *
* * *           *   * *   *     *     *     * *   *
* ******* ******* * *** ******* *** * * ********* *
*   *       *     * *   *         * * * *     *   *
*** * * ***** * ***** ******* ******* * * * * * ***
*     *   *   *         *       * *   * * * * *   *
*** * *** * *** ***** ******* * * * *** *** * *** *
* * * * * * * *     * * *     *       *   * * * * *
* * *** * * * *** *** * * ********* ***** * * * * *
* * *   * * *     *   * *   *     *   *     * * * *
* * * *** ******* ***** * ******* *** * *** *** * *
* * *     *   *   *     * *     * * * *   *   * * *
* ***** * * * *** * ***** ***** * * * ***** * * * *
* *     * * * *     * *     *           * * *   * *
* ***** * *** * ***** *********** ******* * * * * *
*     * * * *             *   *     * * *   * * * *
* * * *** * *** * ***** ***** ******* * *** * * * *
* * *   * * *   *     * *             *     * * * *
* ***** * * *********** ******* *** * ******* * * *
* *     *   *   *     * *   *   * * *       * *   *
* * * ********* * ***** * *** *** *** * ***** * ***
* * *       *           *   * * *   * *   *   *   *
* ******* ***** ******* * *** * * *** *** * *******
*   *   *   *   *   *     *         * * * * * * * *
* ***** * *** ***** * ******* * ***** * *** * * * *
*     *           *     *     * * *   *   *     * *
*** *** ********************* *** *** *** *** * * *
*   *   *     *               * * *   *       *   *
*** *** * ***** * ******* *** * * *** * *** ***** *
*       *       *   *   * * *   *     *   * *   * *
*** ***** ***** *** *** *** ***** * * *** *** * * *
*       *   *   * * *       *   * * *   * *   *   *
*** *** * ***** * ***** *** *** *** *** ******* ***
*   *     *   *   *     * * * *     * * *     *   *
* ***** *** ***** ******* * * *** *** * *** ***** *
*   *                 *           *         *     *
************************* *************************)";

  lb::labyrinth lab(s, std::strlen(s));
  assert(*lab.at(50, 25).c == ' ');
  assert(*lab.at(49, 22).c == '*');
  assert(lab.start_col_ == 25);
  assert(lab.end_col_ == 25);

  auto ce = lab.at(0, lab.start_col_);
  auto n1 = lab.get_neighboors(&ce);
  assert(n1.top.c == nullptr && *n1.left.c == '*' && 
        *n1.right.c == '*' && *n1.bottom.c == ' ');

  lab.populate_paths();
  lab.mark_path();
  lab.print();
}

int main(int argc, const char* argv[]) {
  std::ifstream ifs(argv[1]);
  std::string line((std::istreambuf_iterator<char>(ifs)), 
                   std::istreambuf_iterator<char>());
  
  lb::labyrinth lab(line.data(), line.size());
  lab.populate_paths();
  lab.mark_path();
  lab.print();
  //test();
}