#include "utils.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

#define NPOS std::string::npos

int visited = 0;

std::vector<short *> parseBlock(std::ifstream &f, size_t &len) {
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

void printSet(const std::set<std::pair<int, int>> set) {
  for (auto p : set) {
    std::cout << p.first << ", " << p.second << std::endl;
  }

  std::cout << std::endl;
}

void fold(std::pair<char, int> foldLine, std::set<std::pair<int, int>>& points) {
  std::vector<std::pair<int, int>> foldedPoints;
  std::cout << "Folding on " << foldLine.first << " at " << foldLine.second << std::endl;

  for (auto it = points.begin(); it != points.end();) {
    //std::cout << "testing point (" << it->first << ", " << it->second << ")\n";
    if (foldLine.first == 'x' && it->first > foldLine.second) {
      foldedPoints.push_back({foldLine.second - (it->first - foldLine.second), it->second});
      it = points.erase(it);
    } else if(foldLine.first == 'y' && it->second > foldLine.second) {
      foldedPoints.push_back({it->first, foldLine.second - (it->second - foldLine.second)});
      it = points.erase(it);
    } else {
      ++it;   
    }
  }

  for (auto& p : foldedPoints) {
    points.insert(p);
  }
}

int getMaxX(const std::set<std::pair<int, int>>& points) {
  int max = 0;

  for (const auto& p : points) {
    if (p.first > max) max = p.first;
  }

  return max;
}

int getMaxY(const std::set<std::pair<int, int>>& points) {
  int max = 0;

  for (const auto& p : points) {
    if (p.second > max) max = p.second;
  }

  return max;
}

std::set<std::pair<int, int>> parse(std::ifstream& f, std::vector<std::pair<char, int>>& foldLoc) {
  std::set<std::pair<int, int>> points;
  std::string line;
  bool readingCoords = true;
  int x, y, n;
  char c;

  while(std::getline(f, line)) {
    readingCoords = readingCoords && line.size() >= 3; 
    if (readingCoords) {
      sscanf_s(line.c_str(), "%d%*c%d", &x, &y);
      points.insert({x, y});
    } else if(line.size() > 2) {
      sscanf_s(line.c_str(), "%*s %*s %c%*c%d", &c, 1, &n);
      foldLoc.push_back({c, n});
    }
  }

  return points;
}

int main() {
  std::ifstream file("13/data.txt");
  std::vector<std::pair<char, int>> foldLocation;
  std::set<std::pair<int, int>> points = parse(file, foldLocation);
  bool first = true;
  int maxX, maxY;
  
  for (auto& foldLine : foldLocation) {
    fold(foldLine, points);
    if (first) {
      std::cout << points.size();
      first = false;
    }
  }

  maxX = getMaxX(points);
  maxY = getMaxY(points);

  for (int y = 0; y <= maxY; y++) {
    for (int x = 0; x <= maxX; x++) {
      if (points.find({x, y}) != points.end())
        std::cout << "#";
      else
        std::cout << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}