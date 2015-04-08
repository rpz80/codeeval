#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <cstdint>

enum pstate_t {
    COUNT,
    CONTENT
};

typedef std::pair<int64_t,int64_t> point_t;

void process(const std::vector<point_t>& points)
{
    double max = std::numeric_limits<double>::max();
    const size_t sz = points.size();
    int64_t tmp;

    for(size_t i = 0; i < sz-1; ++i)
    {
        for(size_t j = i+1; j < sz; ++j)
        {
            tmp = std::pow(points[i].first - points[j].first, 2) + 
                std::pow(points[i].second - points[j].second, 2);
            if(tmp < max)
            {
                 max = tmp;
            }
        }
    }
    std::cout << std::fixed << std::sqrt(max) << std::endl;
}

int main(int argc, const char* argv[]) 
{
    std::ifstream ifs(argv[1]);
    std::string line;
    std::vector<point_t> point_vec;
    point_t tmp;
    std::stringstream ss;

    pstate_t state = COUNT;
    uint32_t count = 0;
    line.reserve(8);
    point_vec.reserve(512);

    std::cout.precision(4);
    while(!ifs.eof())
    {
        std::getline(ifs, line);
        switch(state)
        {
            case COUNT:
                count = std::strtol(line.data(), nullptr, 10);
                if(count == 0)
                    break;
                state = CONTENT;
                break;
            case CONTENT:
                ss.clear();
                ss.str(line);
                ss >> tmp.first >> tmp.second;
                point_vec.push_back(tmp);
                --count;
                if(count == 0)
                {
                    state = COUNT;
                    process(point_vec);
                    point_vec.clear();
                }
                break;
        }
    }
}