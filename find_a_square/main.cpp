#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cmath>

struct point
{
  int x, y;
  point() 
    : x(0), y(0)
  {}
};

bool operator == (
  const point& p1,
  const point& p2
)
{
  return 
    p1.x == p2.x &&
    p1.y == p2.y
  ;
}

inline 
size_t dist(
  const point& p1,
  const point& p2
)
{
  return 
    std::pow(std::abs(p1.x - p2.x), 2) +
    std::pow(std::abs(p1.y - p2.y), 2)
  ;
}

std::ostream&
operator << (
  std::ostream& os,
  const point& p
)
{
  return
    os
      << "(" << p.x
      << "," << p.y
      << ")"
    ;
}

enum parse_state {X, Y};

typedef 
std::vector<
  point
> point_vector_t;
 
void get_points(
  std::stringstream& ss,
  point_vector_t& vec
)
{
  parse_state state(X);
  char c;
  point tmp;
  
  while(!ss.eof())
  {
    c = ss.get();
    
    if(std::isdigit(c))
    {
      ss.unget();
      
      if(state == X)
      {
        ss >> tmp.x;
        state = Y;
      }
      else
      {
        ss >> tmp.y;
        state = X;
        vec.push_back(tmp);
      }
    }
  }

  std::sort(
    vec.begin(),
    vec.end(),
    [](
      const point& p1,
      const point& p2
    )
    {
      return p1.x < p2.x;
    }
  );

  auto p0 = vec[0];
  
  // vector multiplication here
  // to order points relative to p[0]
  std::sort(
    vec.begin() + 1,
    vec.end(),
    [p0](
      const point& p1,
      const point& p2
    )
    {
      return
        ((p1.x-p0.x)*(p2.y-p1.y) -
         (p1.y-p0.y)*(p2.x-p1.x)) >= 0
      ;
    }
  );
}

void check(
  point_vector_t& vec
)
{
  for(size_t i = 0; i < 3; ++i)
  {
    if(vec[i] == vec[i + 1])
    {
      std::cout << "false\n";
      return;
    }
  }
  
  if(
    dist(vec[0], vec[2]) !=
    dist(vec[1], vec[3])
  )
  {
    std::cout << "false\n";
    return;
  }
  
  size_t side = 
    dist(
      vec[0], 
      vec[1]
    );
  
  for(size_t i = 1; i < 3; ++i)
  {
    if(dist(vec[i], vec[i+1]) != side)
    {
      std::cout << "false\n";
      return;
    }
  }
  
  std::cout << "true\n";
}

int main(
  int argc, 
  const char* argv[]
)
{
  std::ifstream ifs(argv[1]);
  std::string line;
  std::stringstream ss;  
  point_vector_t vec; 
  
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
    
    ss.clear();
    ss.str(line);
    vec.clear();
    
    get_points(
      ss,
      vec
    );
/*    
    for(const auto& p : vec)
      std::cout 
        << p
        << " "
      ;
    std::cout << std::endl;
*/
    check(vec);
  }
}