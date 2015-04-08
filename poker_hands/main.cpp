#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <map>
#include <ctime>

struct card 
{
  uint8_t val;
  uint8_t mas;
};

typedef std::vector<card> hand_type;

enum comb_type 
{
  NONE            = 1,
  PAIR            = 2,
  TWO_PAIRS       = 4,
  THREE           = 8,
  STRAIGHT        = 16,
  FLUSH           = 32,
  FULL_HOUSE      = 64,
  FOUR            = 128,
  STRAIGHT_FLUSH  = 256,
  ROYAL_FLUSH     = 512
};

class hand 
{
public:
  hand(hand_type& h) : hand_(h), comb_(0) 
  {
    std::sort(hand_.begin(), hand_.end(), 
              [](const card& l, const card& r)
              {
                return l.val < r.val;
              });
    get_comb();
  }
private:
  void get_comb() 
  {
    comb_ = 0;
    
    check_for_straight_flush();
    if(comb_ < STRAIGHT_FLUSH)
      check_pairs();
  }

  void check_for_straight_flush() 
  {
    uint8_t m = hand_[0].mas;
    uint8_t v = hand_[0].val;
    bool straight = true;
    bool flush = true;

    for(size_t i = 1; i < hand_.size(); ++i) 
    {
      if(hand_[i].mas != m)
        flush = false;
      if(hand_[i].val != v+1)
        straight = false;
      m = hand_[i].mas;
      v = hand_[i].val;
    }

    if(straight && flush)
    {
      if(hand_[4].val == 14)
        comb_ = ROYAL_FLUSH;
      else
        comb_ = STRAIGHT_FLUSH;
    }
    else if(flush)
      comb_ = FLUSH;
    else if(straight)
      comb_ = STRAIGHT;
  }

  void check_pairs() 
  {
    for(size_t i = 0; i < hand_.size(); ++i) 
    {
      if(cval_map_.find(hand_[i].val) == cval_map_.end())
        cval_map_.insert(std::make_pair(hand_[i].val, 1));
      else 
        ++cval_map_[hand_[i].val];
    }

    switch(cval_map_.size())
    {
      case 2:
        if(has_same_cards(4))
          comb_ = FOUR;
        else if(has_same_cards(3))
          comb_ = FULL_HOUSE;
        break;
      case 3:
        if(!(comb_ & STRAIGHT) && !(comb_ & FLUSH))
        {
          if(has_same_cards(3))
            comb_ = THREE;
          else if(has_same_cards(2))
            comb_ = TWO_PAIRS;
        }
        break;
      case 4:
        if(!(comb_ & STRAIGHT) && !(comb_ & FLUSH))
          comb_ = PAIR;
        break;
      default:
        if(!(comb_ & STRAIGHT) && !(comb_ & FLUSH))
          comb_ = NONE;
        break;
    }
  }

  bool has_same_cards(uint8_t count)
  {
    for(auto it = cval_map_.begin(); it != cval_map_.end(); ++it)
    {
      if(it->second == count)
        return true;
    }
    return false;
  }

  static void compare_none(const hand& h1, const hand& h2)
  {
    for(size_t i = 4; i >= 0; --i)
    {
      if(h1.hand_[i].val > h2.hand_[i].val)
      {
        std::cout << "left\n";
        return;
      }
      else if(h2.hand_[i].val > h1.hand_[i].val)
      {
        std::cout << "right\n";
        return;
      }
      if(i == 0)
        break;
    }
    std::cout << "none\n";
  }

  static void compare_four_fh(const hand& h1, const hand& h2, 
                              uint8_t firstg)
  {
    uint8_t left4_val, left1_val, right4_val, right1_val;

    for(const auto& p : h1.cval_map_)
      if(p.second == firstg)
        left4_val = p.first;
      else
        left1_val = p.first;

    for(const auto& p : h2.cval_map_)
      if(p.second == firstg)
        right4_val = p.first;
      else
        right1_val = p.first;

      if(left4_val > right4_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right4_val > left4_val)
      {
        std::cout << "right\n";
        return;
      }

      if(left1_val > right1_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right1_val > left1_val)
      {
        std::cout << "right\n";
        return;
      }
      std::cout << "none\n";
  }

  static void compare_three(const hand& h1, const hand& h2)
  {
    uint8_t left3_val, left11_val, right3_val, right11_val,
            left12_val, right12_val;

    left3_val = left11_val = right3_val = right11_val
              = left12_val = right12_val = 0;

    for(const auto& p : h1.cval_map_)
      if(p.second == 3)
        left3_val = p.first;
      else if(left11_val == 0)
        left11_val = p.first;
      else if(left12_val == 0)
        left12_val = p.first;

    for(const auto& p : h2.cval_map_)
      if(p.second == 3)
        right3_val = p.first;
      else if(right11_val == 0)
        right11_val = p.first;
      else if(right12_val == 0)
        right12_val = p.first;

      if(left3_val > right3_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right3_val > left3_val)
      {
        std::cout << "right\n";
        return;
      }

      if(left12_val > right12_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right12_val > left12_val)
      {
        std::cout << "right\n";
        return;
      }

      if(left11_val > right11_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right11_val > left11_val)
      {
        std::cout << "right\n";
        return;
      }

      std::cout << "none\n";
  }

  static void compare_two_pairs(const hand& h1, const hand& h2)
  {
    uint8_t left21_val, left22_val, right21_val, right22_val,
            left1_val, right1_val;

    left21_val = left22_val = right21_val = right22_val
              = left1_val = right1_val = 0;

    for(const auto& p : h1.cval_map_)
    {
      if(p.second == 2 && left21_val == 0)
        left21_val = p.first;
      else if(p.second == 2 && left22_val == 0)
        left22_val = p.first;
      else if(p.second == 1)
        left1_val = p.first;
    }

    for(const auto& p : h2.cval_map_)
    {
      if(p.second == 2 && right21_val == 0)
        right21_val = p.first;
      else if(p.second == 2 && right22_val == 0)
        right22_val = p.first;
      else if(p.second == 1)
        right1_val = p.first;
    }

    if(left21_val > left22_val)
    {
      if(left21_val > right21_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right21_val > left21_val)
      {
        std::cout << "right\n";
        return;
      }

      if(left22_val > right22_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right22_val > left22_val)
      {
        std::cout << "right\n";
        return;
      }
    }
    else
    {
      if(left22_val > right22_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right22_val > left22_val)
      {
        std::cout << "right\n";
        return;
      }

      if(left21_val > right21_val)
      {
        std::cout << "left\n";
        return;
      }
      else if(right21_val > left21_val)
      {
        std::cout << "right\n";
        return;
      }
    }

    if(left1_val > right1_val)
    {
      std::cout << "left\n";
      return;
    }
    else if(right1_val > left1_val)
    {
      std::cout << "right\n";
      return;
    }

    std::cout << "none\n";
  }  

  static void compare_pair(const hand& h1, const hand& h2)
  {
    uint8_t left2_val, left11_val, right2_val, right11_val,
            left12_val, right12_val, left13_val, right13_val;

    left2_val = left11_val = right2_val = right11_val
              = left12_val = right12_val = left13_val = right13_val = 0;

    for(const auto& p : h1.cval_map_)
    {
      if(p.second == 2)
        left2_val = p.first;
      else if(left11_val == 0)
        left11_val = p.first;
      else if(left12_val == 0)
        left12_val = p.first;
      else if(left13_val == 0)
        left13_val = p.first;
    }

    for(const auto& p : h2.cval_map_)
    {
      if(p.second == 2)
        right2_val = p.first;
      else if(right11_val == 0)
        right11_val = p.first;
      else if(right12_val == 0)
        right12_val = p.first;
      else if(right13_val == 0)
        right13_val = p.first;
    }


    if(left2_val > right2_val)
    {
      std::cout << "left\n";
      return;
    }
    else if(right2_val > left2_val)
    {
      std::cout << "right\n";
      return;
    }

    if(left13_val > right13_val)
    {
      std::cout << "left\n";
      return;
    }
    else if(right13_val > left13_val)
    {
      std::cout << "right\n";
      return;
    }

    if(left12_val > right12_val)
    {
      std::cout << "left\n";
      return;
    }
    else if(right12_val > left12_val)
    {
      std::cout << "right\n";
      return;
    }

    if(left11_val > right11_val)
    {
      std::cout << "left\n";
      return;
    }
    else if(right11_val > left11_val)
    {
      std::cout << "right\n";
      return;
    }

    std::cout << "none\n";
  }  
public:
  uint32_t comb() const {return comb_;}

  static void compare(const hand& h1, const hand& h2)
  {
    if(h1.comb_ > h2.comb_)
      std::cout << "left\n";
    else if(h2.comb_ > h1.comb_)
      std::cout << "right\n";
    else
    {
      switch(h1.comb_)
      {
        case ROYAL_FLUSH:
          std::cout << "none\n";
          return;
        case STRAIGHT:
        case STRAIGHT_FLUSH:
          if(h1.hand_[0].val > h2.hand_[0].val)
            std::cout << "left\n";
          else if(h2.hand_[0].val > h1.hand_[0].val)
            std::cout << "right\n";
          else
            std::cout << "none\n";
          return;
        case FLUSH:
        case NONE:
          compare_none(h1, h2);
          return;
        case FOUR:
          compare_four_fh(h1, h2, 4);
          return;
        case THREE:
          compare_three(h1, h2);
          return;
        case TWO_PAIRS:
          compare_two_pairs(h1, h2);
          return;   
        case PAIR:
          compare_pair(h1, h2);
          return;        
       case FULL_HOUSE:
          compare_four_fh(h1, h2, 3);
          return;
        default:
          std::cout << "none\n";
      }
    }
  }

private:
  hand_type& hand_;
  uint32_t comb_;
  std::map<uint8_t, uint8_t> cval_map_;
private:
  friend std::ostream& operator << (std::ostream& os, const hand& h);
};

std::istream& operator >> (std::istream& is, card& ca) 
{
  uint8_t c = is.get();

  if(std::isdigit(c)) 
    ca.val = c - 48;
  else 
  {
    switch(c) 
    {
      case 'T': ca.val = 10; break;
      case 'J': ca.val = 11; break;
      case 'Q': ca.val = 12; break;     
      case 'K': ca.val = 13; break;
      case 'A': ca.val = 14; break;
      default: 
        return is;
    }
  }
  c = is.get();
  ca.mas = c;
  is.get();
  return is;
}

void print_comb(const hand& h, std::ostream& os);

std::ostream& operator << (std::ostream& os, const hand& h)
{
  os << "[ ";
  for(const auto& c : h.hand_)
    os << (int)c.val << (char)c.mas << " ";
  os << "]";
  print_comb(h, os);
  return os;
}

void process(hand_type& left, hand_type& right) 
{
  // std::cout << left << right << std::endl;
  hand::compare(left, right);
}

void print_comb(const hand& h, std::ostream& os) 
{
  uint32_t hcomb = h.comb();

  if(hcomb & ROYAL_FLUSH) 
    os << "ROYAL FLUSH ";
  else if(hcomb & STRAIGHT_FLUSH) 
    os << "STRAIGHT FLUSH ";
  else if(hcomb & FOUR)
    os << "FOUR ";
  else if(hcomb & FULL_HOUSE)
    os << "FULL_HOUSE ";
  else if(hcomb & FLUSH)
    os << "FLUSH ";
  else if(hcomb & STRAIGHT)
    os << "STRAIGHT ";
  else if(hcomb & THREE)
    os << "THREE ";
  else if(hcomb & TWO_PAIRS)
    os << "TWO PAIRS ";
  else if(hcomb & PAIR)
    os << "PAIR ";
  else if(hcomb & NONE)
    os << "NONE ";
  os << std::endl;
}

int main(int argc, char const *argv[]) 
{
  std::ifstream ifs{argv[1]};
  std::string line;
  std::stringstream ss;

  hand_type left, right;
  card tmp_card;
  uint8_t counter;

  time_t start = clock();

  while(!ifs.eof()) 
  {
    std::getline(ifs, line);
    if(line.empty())
      continue;
    ss.clear();
    ss.str(line);
    
    left.clear();
    right.clear();
    counter = 0;

    while(counter != 10) 
    {
      ss >> tmp_card;
      if(counter >= 5) 
        right.push_back(tmp_card);
      else 
        left.push_back(tmp_card);
      ++counter;
    }
    process(left, right);
  }
  std::cout << "Elapsed: " 
            << double(clock() - start) / CLOCKS_PER_SEC 
            << std::endl;
  return 0;
}