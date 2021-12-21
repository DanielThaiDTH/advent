#include "utils.h"
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>
#include <cctype>
#include <cstdint>

#define NPOS std::string::npos

void iteration(std::vector<std::vector<bool>>& map, std::vector<std::vector<bool>>& output, const std::vector<bool>& alg) {
  for (size_t i = 1; i < map.size() - 1; i++) {
    for (size_t j = 1; j < map.size() - 1; j++) {
      size_t num = 0;
      num += map[i+1][j+1] ? 1 : 0;
      num += ((map[i+1][j] ? 1 : 0) << 1);
      num += ((map[i+1][j-1] ? 1 : 0) << 2);
      num += ((map[i][j+1] ? 1 : 0) << 3);
      num += ((map[i][j] ? 1 : 0) << 4);
      num += ((map[i][j-1] ? 1 : 0) << 5);
      num += ((map[i-1][j+1] ? 1 : 0) << 6);
      num += ((map[i-1][j] ? 1 : 0) << 7);
      num += ((map[i-1][j-1] ? 1 : 0) << 8);
      output[i][j] = alg[num];
      //std::cout << i << ", " << j << std::endl;
    }
  }


  for (size_t i = 0; i < map.size(); i++) {
    for (size_t j = 0; j < map.size(); j++) {
      map[i][j] = output[i][j];
    }
  }
}


std::vector<bool> parse(std::ifstream& f, std::vector<std::vector<bool>>& map) {
  std::string line;
  std::vector<bool> alg;
  bool initial = true;
  auto lineparse = [](const std::string& line, std::vector<bool>& bitvec) {
    for (const auto& c : line) {
        if (c == '.')
          bitvec.push_back(false);
        else
          bitvec.push_back(true); 
      }
  };

  while (std::getline(f, line)) {
    if (initial) {
      lineparse(line, alg);
      initial = false;
    } else if (line.size() >= 2) {
      map.push_back(std::vector<bool>());
      lineparse(line, map.back());
    }
  }
  
  return alg;
}


int main() {
  std::ifstream file("20/data.txt");
  std::vector<std::vector<bool>> map;
  std::vector<std::vector<bool>> global_map;
  std::vector<std::vector<bool>> global_copy;
  std::vector<bool> alg = parse(file, map);

  for (int i = 0; i < 500; i++) {
    global_map.push_back(std::vector<bool>());
    global_copy.push_back(std::vector<bool>());
    for(int j = 0; j < 500; j++) {
      global_map.back().push_back(false);
      global_copy.back().push_back(false);
    }
  }

  for (size_t i = 0; i < map.size(); i++) {
    for(size_t j = 0; j < map.back().size(); j++) {
      global_map[i+200][j+200] = map[i][j];
      std::cout << (map[i][j]?'#':'.');
    }
    std::cout << std::endl;
  }

  int count = 1;

  for (; count <= 50; count++) {
    iteration(global_map, global_copy, alg);
  }

  //Pixel Count
  std::cout << "Pixel counting\n";
  size_t pixelCount = 0;
  for (size_t i = 1 + count; i < global_map.size() - 1 - count; i++) {
    for (size_t j = 1 + count; j < global_map.size() - 1 - count; j++) {
      if (global_map[i][j]) pixelCount++;
    }
  }

  std::cout << pixelCount;

  return 0;
}