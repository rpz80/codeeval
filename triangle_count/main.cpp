#include <cstdio>
#include <string>
#include <vector>
#include <cstdint>
#include <utility>
#include <sstream>
#include <cctype>
#include <array>
#include <cstring>

#define TRIANGLES_DEBUG

namespace triangles {
    class graph {
    private:
        static const uint32_t reserve_size = 255;

        using edge_type             = std::pair<uint8_t, uint8_t>;
        using edge_vector_type      = std::vector<edge_type>;
        using vertice_array_type    = std::array<uint8_t, reserve_size>;
        
        enum class search_vertice_type {
            first,
            second
        };
        
    public:
        graph() : triangles_count_(0) {
            edges_.reserve(reserve_size);
        }

    public:
        void reset() {
            edges_.clear();
            std::memset(vertices_.data(), 0, reserve_size);
            triangles_count_ = 0;
        }
    
        void add_edge(const uint8_t start, const uint8_t end) {
            edges_.emplace_back(start, end);
        }
        
        void add_vertice(uint8_t v) {
            vertices_[v] = 1;
        }
        
        void print() {
            std::printf("%-10s", "Vertices: ");
            for (size_t i = 0; i < reserve_size; ++i) {
                if (vertices_[i] != 0) {
                    std::printf("%lu ", i);
                }
            }
            std::printf("\n");
            
            std::printf("%-9s", "Edges: ");
            for (const auto& e : edges_) {
                std::printf("(%u, %u) ", e.first, e.second);
            }
            std::printf("\n");
        }
        
        uint32_t count() {
            for (uint8_t i = 0; i < reserve_size; ++i) {
                if (vertices_[i] == 1) {
                    find_triangles_from(i);
                    vertices_[i] = 2;
                }
            }
            return triangles_count_;
        }
        
    private:
        void find_triangles_from(uint8_t v) {
            auto adjacent_outgoing = find_adjacent(v, search_vertice_type::first);
            auto adjacent_incoming = find_adjacent(v, search_vertice_type::second);

#ifdef TRIANGLES_DEBUG
            std::printf("For %u outgoing adjacent are: ", v);
            for (auto& u: adjacent_outgoing) {
                std::printf("%u ", u);
            }
            std::printf("\n");
            
            std::printf("For %u incoming adjacent are: ", v);
            for (auto& u: adjacent_incoming) {
                std::printf("%u ", u);
            }
            std::printf("\n");
#endif
        }

        std::vector<uint8_t> find_adjacent(uint8_t v, search_vertice_type svt) {
            std::vector<uint8_t> ret;
            for (uint8_t i = 0; i < edges_.size(); ++i) {
                switch (svt) {
                    case search_vertice_type::first:
                        if (edges_[i].first == v && vertices_[edges_[i].second] != 2 
                                                 && vertices_[edges_[i].second] != 0) {
                            ret.push_back(edges_[i].second);
                        }
                        break;
                    case search_vertice_type::second:
                        if (edges_[i].second == v && vertices_[edges_[i].first] != 2 
                                                  && vertices_[edges_[i].first] != 0) {
                            ret.push_back(edges_[i].first);
                        }
                        break;
                }
            }
            return ret;
        }
        
    private:
        edge_vector_type    edges_;
        vertice_array_type  vertices_;
        uint32_t            triangles_count_;
    };
    
    void fill_graph(graph* g, const std::string& s) {
        enum class parse_state {
            vcount,
            ecount,
            e1,
            e2
        } ps = parse_state::vcount;

        g->reset();
        std::stringstream ss(s);
        uint8_t tmp;
        
        while (!ss.eof()) {
            int c = ss.get();
            if (std::isdigit(c)) {
                ss.unget();
                ss >> c;
                switch (ps) {
                    case parse_state::vcount:
                        ps = parse_state::ecount;
                        break;
                    case parse_state::ecount:
                        ps = parse_state::e1;
                        break;
                    case parse_state::e1:
                        ps = parse_state::e2;
                        g->add_vertice(c);
                        tmp = c;
                        break;
                    case parse_state::e2:
                        ps = parse_state::e1;
                        g->add_vertice(c);
                        g->add_edge(tmp, c);
                        break;
                }
            }
        }
    }
}

void test() {
    triangles::graph g;
    triangles::fill_graph(&g, "4 5;0 2,0 1,1 2,1 3,2 3");
    g.print();
    std::printf("Triangles_count: %u\n", g.count());
}

int main(int argc, const char* argv[]) {
    test();
}
