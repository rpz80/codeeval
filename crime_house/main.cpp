#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <array>
#include <stdexcept>
#include <chrono>

namespace ch {
  class crime_analyzer {
    static const int max_size = 2001;
    enum class action_state {
      enter,
      leave,
      unknown
    };
    enum class parse_state {
      actions_number,
      action_type,
      criminal_index
    };
    enum class apply_action_state {
      crime_time,
      done
    };
  public:
    void process(const std::string& line) {
      std::memset(buf_.data(), 0, max_size);
      ss_.clear();
      ss_.str(line);
      action_state as = action_state::unknown;
      parse_state ps = parse_state::actions_number;
      while (!ss_.eof()) {
        int c = ss_.get();
        switch (ps) {
        case parse_state::actions_number:
          if (std::isspace(c)) {
            ps = parse_state::action_type;
          }
          break;
        case parse_state::action_type:
          if (!std::isspace(c) && c != '|') {
            ps = parse_state::criminal_index;
            if (c == 'E') {
              as = action_state::enter;
            } else if (c == 'L') {
              as = action_state::leave;
            } else {
              throw std::logic_error("unexpected action type");
            }
          }
          break;
        case parse_state::criminal_index:
          if (!std::isspace(c)) {
            if (!std::isdigit(c)) {
              throw std::logic_error("expected number >= 0 at criminal index parse state");
            }
            ss_.unget();
            ss_ >> c;
            if (c >= max_size) {
              throw std::logic_error("criminal index should be in [0,2000] range");
            }
            switch (as) {
            case action_state::enter:
            case action_state::leave:
              if (apply_action(c, as) == apply_action_state::crime_time) {
                std::printf("CRIME TIME\n");
                return;
              }
              break;
            }
            ps = parse_state::action_type;
          }
          break;
        }
      }
      std::printf("%u\n", count_criminals());
    }
  private:
    apply_action_state apply_action(int index, action_state as) {
      switch (as) {
      case action_state::enter:
        if (index == 0) {
          ++buf_[index];
        } else {
          if (buf_[index] == 0 || buf_[index] == -1) {
            buf_[index] = 1;
          } else if (buf_[index] == 1) {
            return apply_action_state::crime_time;
          }
        }
        break;
      case action_state::leave:
        if (index == 0) {
          if (buf_[index] > 0) {
            --buf_[index];
          } else {
            for (size_t i = 1; i < max_size; ++i) {
              if (buf_[i] > 0) {
                buf_[i] = 0;
                break;
              }
            }
          }
        } else {
          if (buf_[index] == 1) {
            buf_[index] = 0;
          } else if (buf_[index] == 0) {
            //if (buf_[0] > 0) {
            //  --buf_[0];
            //} else {
              --buf_[index];
            //}
          } else if (buf_[index] == -1) {
            if (buf_[0] > 0) {
              --buf_[0];
            }
            else {
              return apply_action_state::crime_time;
            }
          }
        }
        break;
      }
      return apply_action_state::done;
    }

    uint32_t count_criminals() {
      uint32_t count = 0;
      for (size_t i = 0; i < max_size; ++i) {
        if (buf_[i] > 0) {
          count += buf_[i];
        }
      }
      return count;
    }
  private:
    std::array<int8_t, max_size> buf_;
    std::stringstream ss_;
  };
}

int main(int argc, const char* argv[]) {
  auto start = std::chrono::high_resolution_clock::now();
  if (argc < 2) {
    std::printf("Usage: prg <file-name>\n");
    return 1;
  }
  std::ifstream ifs(argv[1], std::ios_base::binary | std::ios_base::in);
  if (!ifs) {
    std::printf("File open error\n");
    return 1;
  }
  std::string line;
  ch::crime_analyzer ca;
  while (!ifs.eof()) {
    std::getline(ifs, line);
    try {
      ca.process(line);
    } catch(const std::exception& e) {
      std::printf("Exception caught: %s\n", e.what());
      return 1;
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::printf("Elapsed: %.3fms\n", std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
  return 0;
}
