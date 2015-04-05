#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>
#include <fstream>

namespace client
{
  struct point
  {
    int row;
    int col;
    
    point(int row_, int col_)
      : row(row_),
        col(col_)
    {}
  };
  
  class matrix
  {
  public:
    enum direction_type 
    {
      up,
      down,
      left,
      right
    };
  public:
    matrix(const std::string& s)
      : size_(static_cast<size_t>(std::sqrt(s.size()))),
        str_(s)
    {}
  public: 
    char next(
      const point&    current,
      direction_type  dir      
    ) const
    {
      int index = current.row*size_ + current.col;
      assert(((index >= 0) && (index < (int)str_.size())));
      
      int ind;
      switch(dir)
      {
        case up:
          ind = (current.row - 1)*size_ + current.col;
          return current.row - 1 >= 0 ? str_[ind] : '-';
        case down:
          ind = (current.row + 1)*size_ + current.col;
          return current.row + 1 < (int)size_ ? str_[ind] : '-';
        case left:
          ind = current.row*size_ + current.col - 1;
          return current.col - 1 >= 0 ? str_[ind] : '-';          
        case right:
          ind = current.row*size_ + current.col + 1;
          return current.col + 1 < (int)size_ ? str_[ind] : '-';
      }
      return '-';
    }
    
    char at(const point& p) const
    {
      int index = p.row*size_ + p.col;
      assert(((index >= 0) && (index < (int)str_.size())));
      
      return str_[(size_t)index];
    }
    
    size_t size() const
    {
      return size_;
    }
  private:
    const size_t        size_;    
    const std::string   str_;
  };
  
  class sequence
  {
  public:
    sequence(
      const matrix&   m,
      const point&    start_point
    ) 
    : m_(m),
      cur_point_(start_point),
      done_(false)
    {
      seq_.push_back(m.at(start_point));
    }
    
    sequence(const sequence& other)
      : seq_(other.seq_),
        m_(other.m_),
        cur_point_(other.cur_point_),
        done_(other.done_)
    {}
  public:
    void add(matrix::direction_type dir)
    {
      assert(!done_);
      char c = m_.next(cur_point_, dir);
      
      if(c == '-')
      {
        done_ = true;
        return;
      }        
      
      if(std::find(
          seq_.begin(), 
          seq_.end(), 
          c
        ) != seq_.end()
      )
      {
        done_ = true;
        return;
      }
      
      seq_.push_back(c);
      switch(dir)
      {
        case matrix::up:
          --cur_point_.row;
          break;
        case matrix::down:
          ++cur_point_.row;
          break;
        case matrix::left:
          --cur_point_.col;
          break;
        case matrix::right:
          ++cur_point_.col;
          break;          
      }
    }
    
    size_t size() const
    {
      return seq_.size();
    }
    
    bool done() const
    {
      return done_;
    }
  private:
    std::vector<char>   seq_;
    const matrix&       m_;
    point               cur_point_;
    bool                done_;
  };
  
  void advance(
    sequence&               seq,
    matrix::direction_type  dir,
    size_t&                 max_length
  )
  {
    seq.add(dir);
    
    if(seq.done())
    {
      if(max_length < seq.size())
        max_length = seq.size();
      return;
    }
    
    sequence right_seq(seq);
    sequence up_seq(seq);
    sequence down_seq(seq);
    
    advance(
      up_seq, 
      matrix::up, 
      max_length
    );
    
    advance(
      down_seq, 
      matrix::down, 
      max_length
    );
    
    advance(
      seq, 
      matrix::left, 
      max_length
    );
    
    advance(
      right_seq, 
      matrix::right, 
      max_length
    );    
  }
  
  size_t generate(
    const matrix&   m,
    const point&    p
  )
  {
    sequence up_seq(m, p);
    sequence down_seq(m, p);
    sequence left_seq(m, p);
    sequence right_seq(m, p);
    size_t max_length = 0;
    
    advance(
      up_seq, 
      matrix::up, 
      max_length
    );
    
    advance(
      down_seq, 
      matrix::down, 
      max_length
    );
    
    advance(
      left_seq, 
      matrix::left, 
      max_length
    );
    
    advance(
      right_seq, 
      matrix::right, 
      max_length
    );
    
    return max_length;
  }
  
  size_t find_max(const matrix& m)
  {
    size_t result = 0;    
    
    for(size_t i = 0; i < m.size(); ++i)
    {
      for(size_t j = 0; j < m.size(); ++j)
      {
        size_t tmp = generate(m, point(i, j));
        if(tmp > result)
          result = tmp;
      }
    }
    
    return result;
  }
}

void test()
{
  using namespace client;
  
  matrix m("abcdefghi");
  assert(m.size() == 3);
  
  assert(m.at(point(1, 1)) == 'e');
  
  assert(m.next(point(0, 0), matrix::right) == 'b');
  assert(m.next(point(0, 1), matrix::right) == 'c');
  assert(m.next(point(0, 2), matrix::right) == '-');
  
  assert(m.next(point(1, 1), matrix::up) == 'b');
  assert(m.next(point(0, 1), matrix::up) == '-');
  
  assert(m.next(point(1, 1), matrix::down) == 'h');
  assert(m.next(point(2, 1), matrix::down) == '-');
  
  assert(m.next(point(1, 1), matrix::left) == 'd');
  assert(m.next(point(1, 0), matrix::left) == '-');
  
  size_t result = generate(m, point(0,0));
  assert(result == 9);
  
  matrix m1("qttiwkajeerhdgpikkeaaabwl");

  assert(find_max(m1) == 15);
  assert(find_max(matrix("vavprkykiloeizzt")) == 11);
  assert(find_max(matrix("skwajgaaxqpfcxmadpwaraksnkbgcaukbgli")) == 16);
  assert(find_max(matrix("kaja")) == 3);
  assert(find_max(matrix("bjzanjikh")) == 7);
}

int main(int argc, char **argv)
{
//  test();  
  if(argc < 2)
    return 1;
    
  std::ifstream ifs(argv[1]);
  std::string   line;
  
  if(!ifs)
    return 1;
    
  while(!ifs.eof())
  {
    std::getline(ifs, line);
    std::cout 
      << client::find_max(client::matrix(line)) 
      << std::endl;
  }
    return 0;
}
