#include <fstream>
#include <regex>
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>

namespace bn {

  const size_t max_size = 10000;

  class graph {
  private:
    struct node {
      uint16_t label;
      uint16_t dist;
      uint16_t index;
      uint8_t route;
      bool visited;
    };
  public:
    void print() {
	    for (size_t i = 0; i < nodes_.size(); ++i) {
        const node& n = nodes_[i];
        std::printf("n{%-2u, %-2u, %-2u, %-2u, %-2u}", 
                    n.route, n.label, n.dist, 
                    n.index, n.visited);

		    for (size_t j = matrix_size_ * i; j < matrix_size_ * (i+1); ++j) {
			    std::printf("%3d", matrix_[j]);
		    }
		    std::printf("\n");
	    }
    }

    void connect_nodes() {
    }
  private:
    std::vector<node> nodes_;
    std::vector<int8_t> matrix_;
    uint16_t matrix_size_;
  };
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::printf("Usage: bn <file>\n");
    return 1;
  }

  std::ifstream ifs(argv[1]);
  if (!ifs) {
    std::printf("File open error\n");
    return 1;
  }

  std::string line;
  line.reserve(1024);

  while (!ifs.eof()) {
    std::getline(ifs, line);
    bn::process(line);
  }
}