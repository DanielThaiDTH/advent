#include "utils.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <cstdint>

#define NPOS std::string::npos

void printSet(const std::set<std::pair<std::pair<char, char>, char>>& set) {
  for (auto p : set) {
    std::cout << "{ {" << p.first.first << ", " << p.first.second << "}, " << p.second << "}" << std::endl;
  }

  std::cout << std::endl;
}

void printMap(const std::map<std::pair<char, char>, char>& map) {
  for (auto p : map) {
    std::cout << "{ {" << p.first.first << ", " << p.first.second << "}, " << p.second << "}" << std::endl;
  }
  std::cout << std::endl;
}

void printMap(const std::map<std::pair<char, char>, uint64_t>& map) {
  for (auto p : map) {
    std::cout << "{ {" << p.first.first << ", " << p.first.second << "}, " << p.second << "}" << std::endl;
  }
  std::cout << std::endl;
}

void printMap(const std::map<char, uint64_t>& map) {
  for (auto p : map) {
    std::cout << "{" << p.first << ", " << p.second << "}" << std::endl;
  }
  std::cout << std::endl;
}


std::map<std::pair<char, char>, char> parse(std::ifstream& f, std::string& temp, std::map<char, uint64_t>& letters) {
  std::map<std::pair<char, char>, char> rules;
  std::string line;
  char a, b, c;

  std::getline(f, temp);

  std::getline(f, line);

  while(std::getline(f, line)) {
    sscanf_s(line.c_str(), "%c%c %*s %c", &a, 1, &b, 1, &c, 1);
    rules.insert({{a, b}, c});
    letters.insert({c, 0});
  }

  return rules;
}

int main() {
  std::ifstream file("14/data.txt");
  std::string temp;
  std::map<char, uint64_t> letters;
  std::map<std::pair<char, char>, char> rules = parse(file, temp, letters);
  std::map<std::pair<char, char>, uint64_t> pairCount;
  std::map<std::pair<char, char>, uint64_t> nextInc;
  int count = 0;

  for (const auto& p : rules) {
    pairCount.insert({p.first, 0});
    nextInc.insert({p.first, 0});
  }
  
  //printMap(rules);

  //Initialize
  for (auto i = 0u; i < temp.size() - 1; i++) {
    auto pair = std::pair<char, char>{temp.at(i), temp.at(i+1)};
    auto p_cnt = pairCount.find(pair);
    p_cnt->second += 1;

    letters.find(temp.at(i))->second += 1;
    if (i + 2 == temp.size())
      letters.find(temp.at(i+1))->second += 1;
    
    //letters.find(rules.find(pair)->second)->second += 1;
  }

  printMap(letters);
  //count++;

  //Start
  uint64_t cnt;
  char newLetter;
  for (; count < 40; count++) {
    for (auto& pc : pairCount) {
      cnt = pc.second;
      if (cnt > 0) {
        newLetter = rules.find(pc.first)->second;
        nextInc.find({pc.first.first, newLetter})->second += cnt;
        nextInc.find({newLetter, pc.first.second})->second += cnt;
        letters.find(newLetter)->second += cnt;
        pc.second = 0;
      }
    }
    
    for (auto& inc : nextInc) {
      pairCount.find(inc.first)->second += inc.second;
      inc.second = 0;
    }
  }

  //printMap(letters);
  //printMap(pairCount);
  uint64_t min = ULLONG_MAX;
  uint64_t max = 0;

  for (auto& lc : letters) {
    if (lc.second < min) min = lc.second;
    if (lc.second > max) max = lc.second;
  }

  std::cout << max - min;

  return 0;
}