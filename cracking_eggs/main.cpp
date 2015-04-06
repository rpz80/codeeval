#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include <vector>
#include <ctime>

typedef std::vector<std::vector<uint32_t> > table_type;

const size_t EGGS_MAX = 12;
const size_t FLOORS_MAX = 10000;

uint32_t recursive_drop(uint32_t eggs, uint32_t floors);
const table_type& prepare_table(table_type& table, uint32_t eggs, 
                                                   uint32_t floors);
void print_table(const table_type& t);

double calc_sqr_eq(double a, double b, double c) {
  double d = b*b - 4*a*c;
  double x1 = (-b + std::sqrt(d))/2*a;
  double x2 = (-b - std::sqrt(d))/2*a;
  // std::cout << x1 << " " << x2 << std::endl;
  return x1 >=0 ? x1 : x2;
}

void process(table_type& t, int eggs, int floors) {
  prepare_table(t, eggs, floors);
  print_table(t);
}

uint32_t recursive_drop(uint32_t eggs, uint32_t floors) {
  if(eggs == 1)
    return floors;
  if(floors == 0 || floors == 1)
    return floors;
  uint32_t min = std::numeric_limits<uint32_t>::max();
  uint32_t result = 0;
  for(uint32_t i = 1; i <= floors; ++i) {
    result = std::max(recursive_drop(eggs-1, i-1), 
                      recursive_drop(eggs, floors-i));
    if(result < min)
      min = result;
  }
  return min + 1;
}

const table_type& prepare_table(table_type& table, uint32_t eggs, 
                                                   uint32_t floors) {
  table.resize(eggs+1);
  for(size_t i = 0; i < eggs+1; ++i) {
    table[i].resize(floors+1);
  }
  for(size_t i = 0; i <= eggs; ++i) {
    for(size_t j = 0; j <= floors; ++j) {
      table[i][j] = 0;
    }
  }
  for(size_t i = 0; i < eggs+1; ++i) {
    table[i][0] = 0;
    table[i][1] = 1;
  }
  for(size_t j = 1; j < floors+1; ++j) {
    table[1][j] = j;
  }
  uint32_t tmp;
  for(size_t i = 2; i <= eggs; ++i) {
    for(size_t j = 2; j <= floors; ++j) {
      table[i][j] = std::numeric_limits<uint32_t>::max();
      for(size_t k = 1; k <= j; ++k) {
        tmp = std::max(table[i-1][k-1], table[i][j-k]) + 1;
        if(tmp < table[i][j])
          table[i][j] = tmp;
      }
    }
  }
  return table;
}

void print_table(const table_type& t) {
  size_t floors_size = t[0].size();
  for(size_t i = 0; i < t.size(); ++i) {
    for(size_t j = 0; j < floors_size; ++j) {
      std::cout << t[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

int main(int argc, const char* argv[]) {
  auto start = std::clock();
  std::ifstream ifs(argv[1]);
  std::string line;
  std::stringstream ss;
  table_type t;
  uint32_t eggs, floors;
  prepare_table(t, EGGS_MAX, FLOORS_MAX);
  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line.empty())
      continue;
    ss.clear();
    ss.str(line);
    ss >> eggs >> floors;
    std::cout << t[eggs][floors <= FLOORS_MAX ? floors : FLOORS_MAX] << std::endl;
  }
  std::cout << "Elapsed: " << (double)(std::clock() - start)/CLOCKS_PER_SEC 
            << "s" << std::endl;
}