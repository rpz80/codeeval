#include <fstream>
#include <regex>
#include <cstdio>
#include <string>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cctype>

namespace bn {

  const size_t max_dist = 10000;

  auto points_re = std::regex(R"points(\((\d+),\s*(\d+)\);)points");
	auto stops_re = std::regex(R"stops(R\d+=\[(.+)\].*)stops");

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

		    for (size_t j = nodes_.size() * i; j < nodes_.size() * (i+1); ++j) {
			    std::printf("%3d", matrix_[j]);
		    }
		    std::printf("\n");
	    }
    }

    void connect_nodes() {
	    matrix_.resize(nodes_.size()*nodes_.size());
	    
      for (uint32_t i = 0; i < nodes_.size(); ++i) {
		    nodes_[i].index = i;
		    for (uint32_t j = 0; j < nodes_.size(); ++j) {
			    if (i == j) {
				    // *g.matrix.at(i, j) = -1
				    if (j > 0 && nodes_[i-1].route == nodes_[i].route) {
					    at(i, j-1) = 7;
				    }
				    if (j < nodes_.size()-1 && nodes_[i+1].route == nodes_[i].route) {
					    at(i, j+1) = 7;
				    }
			    } else if (nodes_[i].label == nodes_[j].label) {
				    at(i, j) = 12;
			    }
		    }
	    }
    }

    void reset() {
	    for (node& n: nodes_) {
		    n.dist = max_dist;
		    n.visited = false;
	    }
    }

    std::vector<node*> find_start_nodes(uint32_t start) {
	    std::vector<node*> result;
	    for (auto& v : nodes_) {
		    if (v.label == start) {
			    result.push_back(&v);
		    }
	    }
	    return result;
    }

    node* find_min_node() {
	    node* retNode = nullptr;

	    for (auto& v : nodes_) {
		    if (v.label != max_dist && !v.visited) {
			    retNode = &v;
			    break;
		    }
	    }

	    if (retNode == nullptr) {
		    return retNode;
	    }

	    for (uint32_t i = 0; i < nodes_.size(); ++i) {
		    if (nodes_[i].dist < retNode->dist && !nodes_[i].visited) {
			    retNode = &nodes_[i];
		    }
	    }

	    return retNode;
    }

    bool find_and_relax_siblings(node* n, uint32_t stop) {
	    // var i uint = 0
	    uint32_t j = nodes_.size() * n->index;
	    for (; j < nodes_.size()*(n->index+1); ++j) {
		    if (matrix_[j] != 0 && matrix_[j] + n->dist <nodes_[j-nodes_.size()*n->index].dist) {
			    nodes_[j-nodes_.size()*n->index].dist = matrix_[j] + n->dist;
		    }
	    }
	    n->visited = true;

	    if (n->label == stop) {
		    return true;
	    }
	    return false;
    }

    void fillResults(uint32_t stopLabel, std::vector<uint32_t>* results) {
	    for (auto& v : nodes_) {
		    if (v.label == stopLabel) {
			    results->push_back(v.dist);
		    }
	    }
    }

  void findShortestPath(uint32_t start, uint32_t stop) {
	  std::vector<uint32_t> results;
	  auto startNodes = find_start_nodes(start);

	  if (startNodes.size() == 0) {
		  std::printf("None\n");
		  return;
	  }
	  for (auto node : startNodes) {
		  reset();
		  node->dist = 0;
		  while(true) {
			  auto minNode = find_min_node();
			  // fmt.Println("min node: ", minNode)
			  if (minNode == nullptr) {
				  break;
			  }
			  if (find_and_relax_siblings(minNode, stop)) {
				  break;
			  }
		  }

		  fillResults(stop, &results);
		  // g.print()
	  }

	  if (results.size() == 0) {
		  std::printf("None\n");
		  return;
	  } else {
		  auto minDist = std::min_element(results.cbegin(), results.cend());
      if (minDist == results.cend()) {
        std::printf("None\n");
        return;
      }

      if (*minDist == max_dist) {
			  std::printf("None\n");
		  } else {
			  std::printf("%u\n", *minDist);
		  }
	  }
  }

  void appendRoute(uint8_t route , const std::vector<uint32_t>& stops ) {
	  for (auto v : stops) {
      node n = {v, max_dist, 0, route, false};
		  nodes_.push_back(n);
	  }
  }

  private:
    int8_t& at(uint32_t i, uint32_t j) {
      return matrix_[i*nodes_.size() + j];
    }
  private:
    std::vector<node> nodes_;
    std::vector<int8_t> matrix_;
  };

  void process(const std::string& s) {
	  std::vector<std::string> splts;
    std::stringstream ss(s);
    std::string tmp;

    while(!ss.eof()) {
      ss >> tmp;
      splts.push_back(tmp);
    }

    std::smatch points_match;
    bool result_match = std::regex_match(splts[0], points_match, points_re);

	  if (!result_match || points_match.size() != 3) {
      std::printf("points regex error\n");
		  return;
	  }

	  uint32_t start = std::stoul(points_match[1].str());
    uint32_t stop = std::stoul(points_match[2].str());

	  graph g;

    std::smatch stops_match;
    std::stringstream stops_ss;

	  for (uint32_t i = 1; i < splts.size(); ++i) {
      result_match = std::regex_match(splts[i], stops_match, stops_re);

      if (!result_match || stops_match.size() != 2) {
        std::printf("stops regex error\n");
		    return;
	    }
      
      stops_ss.clear();
      stops_ss.str(stops_match[1].str());
		  std::vector<uint32_t> stops_int;
      uint32_t tmp_int;

      while (!stops_ss.eof()) {
        char c = stops_ss.get();
        if (std::isdigit(c)) {
          stops_ss.unget();
          stops_ss >> tmp_int;
          stops_int.push_back(tmp_int);
        }
      }

		  g.appendRoute(i, stops_int);
	  }
	  g.connect_nodes();
	  g.findShortestPath(start, stop);
  }
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