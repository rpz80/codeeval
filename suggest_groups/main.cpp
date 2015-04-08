#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <cassert>
#include <cstdint>

struct user {
  user() {}
  user(const uint32_t id, const std::string& name) 
    : id(id), name(name) {}
  uint32_t id;
  std::string name;
  std::vector<uint32_t> friends;
};

struct group {
  group(const uint32_t id, const std::string& name) 
    : id(id), name(name) {}
  uint32_t id;
  std::string name;
  std::vector<uint32_t> users;
};

typedef std::vector<user> user_vector_type;
typedef std::vector<group> group_vector_type;

enum parse_state {
  USER,
  FRIENDS,
  GROUPS
};

static uint32_t uid = 0;
static uint32_t gid = 0;

auto user_by_id(uint32_t id, user_vector_type& users) 
                                -> user_vector_type::iterator {
  return std::find_if(users.begin(), users.end(),
            [id](const user& u) {
              return u.id == id;
            });
}

void parse_line(const std::string& line, user_vector_type& users,
                                         group_vector_type& groups) {
  parse_state state = USER;
  std::string tmp;
  tmp.reserve(32);
  uint32_t cur_user_id = 0;
  
  for(size_t i = 0; i < line.size(); ++i) {
    char c = line[i];
    switch(state) {
      case USER:
        if(c != ':') {
          tmp.push_back(c);
        }
        else {
          auto user_it = std::find_if(users.begin(), users.end(),
            [&tmp](const user& u) {
              return u.name == tmp;
            });
          if(user_it == users.end()) {
            users.push_back(user(uid++, tmp));
            cur_user_id = users.back().id;
          }
          else
            cur_user_id = user_it->id;
          state = FRIENDS;
          tmp.clear();
        }
        break;
      case FRIENDS:
        if(std::isalpha(c) || c == ' ') {
          tmp.push_back(c);
        }
        else if(c == ',' || c == ':') {
          auto user_it = std::find_if(users.begin(), users.end(),
            [&tmp](const user& u) {
              return u.name == tmp;
            });
          if(user_it == users.end()) {
            users.push_back(user(uid++, tmp));
            user_by_id(cur_user_id, users)->friends.push_back(users.back().id);
          }
          else {
            user_by_id(cur_user_id, users)->friends.push_back(user_it->id);
          }
          tmp.clear();
          if(c == ':') {
            state = GROUPS;
          }
        }
        break;
      case GROUPS:
        if(std::isalpha(c) || c == ' ') {
          tmp.push_back(c);
        }
        else if(c == ',') {
          auto group_it = std::find_if(groups.begin(), groups.end(),
            [&tmp](const group& g) {
              return g.name == tmp;
            });
          if(group_it == groups.end()) {
            groups.push_back(group(gid++, tmp));
            groups.back().users.push_back(cur_user_id);
          }
          else {
            group_it->users.push_back(cur_user_id);
          }
          tmp.clear();
        }
        break;
      default:
        assert(0);
    }
  }
  if(!tmp.empty()) {
    auto group_it = std::find_if(groups.begin(), groups.end(),
      [&tmp](const group& g) {
        return g.name == tmp;
      });
    if(group_it == groups.end()) {
      groups.push_back(group(gid++, tmp));
      groups.back().users.push_back(cur_user_id);
    }
    else {
      group_it->users.push_back(cur_user_id);
    }
    tmp.clear();
  }
}

void make_suggestions(user_vector_type& users, 
                      group_vector_type& groups) {
  std::vector<uint32_t> suggested_groups;
  std::sort(users.begin(), users.end(),
    [](const user& l, const user& r) {
      return l.name < r.name;
    });

  for(const user& u : users) {
    const size_t friends_size = u.friends.size();
    size_t friend_group_count = 0;
    suggested_groups.clear();

    for(const group& g: groups) {
      auto user_in_group = std::find_if(g.users.begin(), g.users.end(),
                              [&u](uint32_t id) {
                                return u.id == id;
                              });
      if(user_in_group != g.users.end())
        continue;
      for(uint32_t fid : u.friends) {
        if(std::find_if(g.users.begin(), g.users.end(),
                              [fid](uint32_t id) {
                                return fid == id;
                              }) != g.users.end()) {
          ++friend_group_count;
        }
      }
      if((double)friend_group_count >= (double)friends_size/2) {
        suggested_groups.push_back(g.id);
      }
      friend_group_count = 0;
    }
    if(!suggested_groups.empty()) {
      std::sort(suggested_groups.begin(), suggested_groups.end(),
        [&groups](uint32_t l, uint32_t r) {
          auto lit = std::find_if(groups.cbegin(), groups.cend(),
            [&](const group& g) {
              return g.id == l;
            });
          auto rit = std::find_if(groups.cbegin(), groups.cend(),
            [&](const group& g) {
              return g.id == r;
            });
          return lit->name < rit->name;
        });
      std::cout << u.name << ":";
      for(size_t i = 0; i < suggested_groups.size(); ++i) {
        auto git = std::find_if(groups.cbegin(), groups.cend(),
          [&](const group& g) {
            return g.id == suggested_groups[i];
          });
        std::cout << git->name;
        if(i != suggested_groups.size()-1) {
          std::cout << ",";
        }
      }
      std::cout << std::endl;
    }
  }
}

int main(int argc, const char* argv[]) {
  if(argc < 2) {
    std::cout << "Usage: prgm fname\n";
    return -1;
  }
  std::ifstream ifs(argv[1]);
  if(!ifs) {
    std::cout << "File open error\n";
    return -1;
  }
  std::string line;
  user_vector_type users;
  group_vector_type groups;
  
  while(!ifs.eof()) {
    std::getline(ifs, line);
    if(line.empty())
      continue;
    parse_line(line, users, groups);
  }
  make_suggestions(users, groups);
  return 0;
}