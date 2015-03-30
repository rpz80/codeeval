#include <cstdio>
#include <fstream>
#include <limits>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

void process(const std::string& s, 
             std::vector<int>* buf,
             std::stringstream* ss) {
    buf->clear();
    ss->clear();
    ss->str(s);

    int tmp;
    int max_elem = 0;
    *ss >> tmp;

    while(!ss->eof()) {
        *ss >> tmp;
        buf->push_back(tmp);
        if (tmp > max_elem) {
            max_elem = tmp;
        }
    }

    int32_t min_path = std::numeric_limits<int32_t>::max();

    for (int32_t i = 1; i <= (int32_t)max_elem; ++i) {
        tmp = 0;
        for (int32_t j = 0; j < (int32_t)buf->size(); ++j) {
            tmp += std::abs(i - buf->at(j));
        }
        if (tmp < min_path) {
            min_path = tmp;
        }
    }

    std::printf("%d\n", min_path);
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::fprintf(stderr, "Usage: %s <fname>\n", argv[0]);
        return 1;
    }

    
    std::ifstream ifs(argv[1]);
    if (!ifs) {
        fprintf(stderr, "File open error\n");
        return 1;
    }

    std::string line;
    std::vector<int> buf;
    std::stringstream ss;

    line.reserve(512);
    buf.reserve(128);

    while (!ifs.eof()) {
        std::getline(ifs, line);
        process(line, &buf, &ss);
    }
}
