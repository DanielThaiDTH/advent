#include "utils.h"
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
  for (auto it = vec.begin(); it != vec.end();) {
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

std::vector<short *> parse(std::ifstream &f, size_t &len) {
  std::vector<short *> data;
  short *nline;
  std::string line;

  std::getline(f, line);
  size_t s = line.size();
  nline = new short[s];
  for (auto i = 0u; i < s; i++) {
    if (line[i] != '\0')
      nline[i] = line[i] - '0';
  }
  data.push_back(nline);
  // std::cout << line << std:: endl;

  while (std::getline(f, line)) {
    // std::cout << line << std:: endl;
    nline = new short[s];
    for (auto i = 0u; i < s; i++) {
      if (line[i] != '\0')
        nline[i] = line[i] - '0';
    }
    data.push_back(nline);
  }

  len = s;
  return data;
}

void increaseEnergy(std::vector<short *> &data, size_t len,
                    std::stack<std::pair<int, int>> &coords) {
  for (int i = 0; i < data.size(); i++) {
    for (int j = 0; j < len; j++) {
      ++data[i][j];
      if (data[i][j] == 10)
        coords.push({i, j});
    }
  }
}

void print(const std::vector<short *> &data, size_t len) {
  for (int i = 0; i < data.size(); i++) {
    for (int j = 0; j < len; j++) {
      std::cout << (data[i][j]<10?" ":"") << data[i][j] << ' ';
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;
}


void explode(std::vector<short *> &data, size_t len,
             std::stack<std::pair<int, int>> &coords, size_t &count) {
    int i, j;
    auto addIfNotZero = [](short& d){ 
        if (d != 0) ++d;
     };
  while (coords.size() > 0) {
    count++;
    auto p = coords.top();
    i = p.first;
    j = p.second;
    data[i][j] = 0;
    coords.pop();

    if (i > 0) {
      addIfNotZero(data[i - 1][j]);
      if (data[i - 1][j] == 10)
        coords.push({i - 1, j});

      if (j > 0) {
        addIfNotZero(data[i - 1][j - 1]);
        if (data[i - 1][j - 1] == 10)
          coords.push({i - 1, j - 1});
      }

      if (j < len - 1) {
        addIfNotZero(data[i - 1][j + 1]);
        if (data[i - 1][j + 1] == 10)
          coords.push({i - 1, j + 1});
      }
    }

    if (i < data.size() - 1) {
      addIfNotZero(data[i + 1][j]);
      if (data[i + 1][j] == 10)
        coords.push({i + 1, j});

      if (j > 0) {
        addIfNotZero(data[i + 1][j - 1]);
        if (data[i + 1][j - 1] == 10)
          coords.push({i + 1, j - 1});
      }

      if (j < len - 1) {
        addIfNotZero(data[i + 1][j + 1]);
        if (data[i + 1][j + 1] == 10)
          coords.push({i + 1, j + 1});
      }
    }

    if (j > 0) {
      addIfNotZero(data[i][j - 1]);
      if (data[i][j - 1] == 10)
        coords.push({i, j - 1});
    }

    if (j < len - 1) {
      addIfNotZero(data[i][j + 1]);
      if (data[i][j + 1] == 10)
        coords.push({i, j + 1});
    }
  }
}

void explodeSync(std::vector<short *> &data, size_t len,
             std::stack<std::pair<int, int>> &coords, bool &allExp, size_t& dataSize) {
  int i, j;
  auto addIfNotZero = [](short &d) {
    if (d != 0)
      ++d;
  };

  size_t explodeCount = 0;

  while (coords.size() > 0) {

    auto p = coords.top();
    i = p.first;
    j = p.second;
    data[i][j] = 0;
    coords.pop();
    explodeCount++;

    if (i > 0) {
      addIfNotZero(data[i - 1][j]);
      if (data[i - 1][j] == 10)
        coords.push({i - 1, j});

      if (j > 0) {
        addIfNotZero(data[i - 1][j - 1]);
        if (data[i - 1][j - 1] == 10)
          coords.push({i - 1, j - 1});
      }

      if (j < len - 1) {
        addIfNotZero(data[i - 1][j + 1]);
        if (data[i - 1][j + 1] == 10)
          coords.push({i - 1, j + 1});
      }
    }

    if (i < data.size() - 1) {
      addIfNotZero(data[i + 1][j]);
      if (data[i + 1][j] == 10)
        coords.push({i + 1, j});

      if (j > 0) {
        addIfNotZero(data[i + 1][j - 1]);
        if (data[i + 1][j - 1] == 10)
          coords.push({i + 1, j - 1});
      }

      if (j < len - 1) {
        addIfNotZero(data[i + 1][j + 1]);
        if (data[i + 1][j + 1] == 10)
          coords.push({i + 1, j + 1});
      }
    }

    if (j > 0) {
      addIfNotZero(data[i][j - 1]);
      if (data[i][j - 1] == 10)
        coords.push({i, j - 1});
    }

    if (j < len - 1) {
      addIfNotZero(data[i][j + 1]);
      if (data[i][j + 1] == 10)
        coords.push({i, j + 1});
    }
  }

  if (explodeCount == dataSize) {
    allExp = true;
    print(data, len);
    return;
  }
}

int main() {
  std::ifstream file("11/data.txt");
  size_t len = 0;
  char c;
  size_t explodeCount = 0;
  std::stack<std::pair<int, int>> explodeCoords;
  std::vector<short *> data = parse(file, len);
  size_t dataSize = len*data.size();

  int count = 0;
  std::cout << len << std::endl;
  print(data, len);

//   for (auto i = 0; i < count; i++) {
//     increaseEnergy(data, len, explodeCoords);
//     explode(data, len, explodeCoords, explodeCount);
//     //print(data, len);
//     //std::cin >> c;
//   }

//   std::cout << explodeCount;

    bool allExp = false;
    while (!allExp) {
      increaseEnergy(data, len, explodeCoords);
      explodeSync(data, len, explodeCoords, allExp, dataSize);
      count++;
    }

    std::cout << count;
}