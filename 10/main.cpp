//#include "utils.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


#define NPOS std::string::npos

template <typename T>
void filter(std::vector<T> &vec, std::function<bool(const T &)> f) {
  for (auto it = vec.begin(), it != vec.end();) {
    if (!f(*it)) {
      vec.erase();
    } else {
      ++it;
    }
  }
}

template <typename T> T &max(T &a, T &b) {
  if (a >= b)
    return a;
  else
    return b;
}

template <typename T> T min(T a, T b) {
  if (a <= b)
    return a;
  else
    return b;
}

bool bracketCheck(const std::string &s, std::map<char, size_t> &score,
                  size_t &points) {
  std::stack<char> bracketStack;
  bool hasExtraEndBracket = false;

  auto validStartBracket = [](char test) {
    return test == '(' || test == '{' || test == '[' || test == '<';
  };
  auto validEndBracket = [](char test) {
    return test == ')' || test == '}' || test == ']' || test == '>';
  };
  auto getEndBracket = [](char key) {
    switch (key) {
    case '(':
      return ')';
    case '{':
      return '}';
    case '[':
      return ']';
    case '<':
      return '>';
    default:
      return '\0';
    }
  };

  for (auto i = 0u; i < s.size() && !hasExtraEndBracket; i++) {
    if (validStartBracket(s.at(i))) {
      bracketStack.push(getEndBracket(s.at(i)));
    } else if (validEndBracket(s.at(i)) && bracketStack.size() > 0 &&
               s.at(i) == bracketStack.top()) {
      bracketStack.pop();
    } else if (validEndBracket(s.at(i))) {
      // std::cout << s.at(i) << std::endl;
      points += score.find(s.at(i))->second;
      hasExtraEndBracket = true;
    }
  }

  if (!hasExtraEndBracket)
    std::cout << bracketStack.size() << std::endl;
  return !hasExtraEndBracket && bracketStack.size() > 0;
}

size_t bracketScore(const std::string &s, std::map<char, size_t> &score) {
  std::stack<char> bracketStack;
  bool hasExtraEndBracket = false;
  size_t points = 0;

  auto validStartBracket = [](char test) {
    return test == '(' || test == '{' || test == '[' || test == '<';
  };
  auto validEndBracket = [](char test) {
    return test == ')' || test == '}' || test == ']' || test == '>';
  };
  auto getEndBracket = [](char key) {
    switch (key) {
    case '(':
      return ')';
    case '{':
      return '}';
    case '[':
      return ']';
    case '<':
      return '>';
    default:
      return '\0';
    }
  };

  for (auto i = 0u; i < s.size(); i++) {
    if (validStartBracket(s.at(i))) {
      bracketStack.push(getEndBracket(s.at(i)));
    } else if (validEndBracket(s.at(i)) && bracketStack.size() > 0 &&
               s.at(i) == bracketStack.top()) {
      bracketStack.pop();
    }
  }

  while (bracketStack.size() > 0) {
    points = points * 5 + score[bracketStack.top()];
    // std::cout << " + " << score[bracketStack.top()] << " = "<< points << " :
    // " << bracketStack.top() << " || ";
    bracketStack.pop();
  }

  // std::cout << std::endl;

  return points;
}

int main() {
  std::ifstream file("10/data.txt");
  std::string line;
  std::map<char, size_t> score;
  std::vector<std::string> lines;
  std::vector<size_t> scores;
  size_t points = 0;
  score.insert({')', 3});
  score.insert({']', 57});
  score.insert({'}', 1197});
  score.insert({'>', 25137});

  while (std::getline(file, line)) {
    if (bracketCheck(line, score, points))
      lines.push_back(line);
  }

  std::cout << points << std::endl << std::endl;
  points = 0;

  score[')'] = 1;
  score[']'] = 2;
  score['}'] = 3;
  score['>'] = 4;

  // std::cout << score['>'] << std::endl;

  for (const auto &l : lines) {
    scores.push_back(bracketScore(l, score));
    // std::cout << scores.back() << std::endl;
  }

  std::sort(scores.begin(), scores.end(),
            [](size_t a, size_t b) { return a < b; });

  for (auto i = 0u; i < scores.size(); i++) {
    std::cout << i + 1 << ": " << scores[i] << std::endl;
  }

  std::cout << scores.size() / 2 << ": " << scores[scores.size() / 2];

  return 0;
}