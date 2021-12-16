#include "utils.h"
#include <algorithm>
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

struct Vertex {
  Vertex* prev;
  size_t cost;
  std::pair<int, int> location;
  Vertex(std::pair<int, int> loc) {
    cost = UINT_MAX;
    prev = nullptr;
    location = loc;
  }
};

class vertexComp {
public:
  bool operator()(Vertex* a, Vertex* b) {
    return b->cost < a->cost;
  }
};


Vertex* getVertex(int i, int j, std::map<std::pair<int, int>, Vertex*>& visited) {
  Vertex* v = nullptr;

  if (visited.find({i, j}) != visited.end()) {
    v = visited.find({i, j})->second;
  } else {
    v = new Vertex({i, j});
    visited.insert({{i, j}, v});
  }

  return v;
}


void setCost(Vertex* src, Vertex* dst, const std::vector<short*>& riskMap) {

  if (dst->prev == src)
    return;

  size_t additionalCost = riskMap[dst->location.first][dst->location.second];

  if (src->cost + additionalCost < dst->cost) {
    dst->cost = src->cost + additionalCost;
    dst->prev = src;
  }
}

void setCost(Vertex* src, Vertex* dst, const std::vector<short*>& riskMap, size_t len) {
  if (dst->prev == src)
    return;

  size_t calc = (riskMap[dst->location.first%riskMap.size()][dst->location.second%len] 
                          + dst->location.first/riskMap.size() + dst->location.second/len);
  size_t additionalCost = (calc)%10 + ((calc > 9)? 1 : 0);

  if (src->cost + additionalCost < dst->cost) {
    dst->cost = src->cost + additionalCost;
    dst->prev = src;
  }
}

size_t getCost(Vertex* end) {

  return 0;
}

int main() {
  std::ifstream file("15/data.txt");
  size_t len;
  std::vector<short*> riskMap = parseBlock(file, len);
  std::map<std::pair<int, int>, Vertex*> visited;
  std::priority_queue<Vertex*, std::vector<Vertex*>, vertexComp> q;

  Vertex* initial = new Vertex({0, 0});
  initial->cost = 0;
  q.push(initial);

  Vertex* v;
  Vertex* adj;

  auto update = [&](Vertex* v, Vertex* adj) { 
      if (adj->cost == UINT_MAX) {
        setCost(v, adj, riskMap);
        q.push(adj);
      } else {
        setCost(v, adj, riskMap);
      }
  };

  while (q.size() > 0) {
    v = q.top();
    q.pop();
    
    //Check adjacent
    if ((size_t)v->location.first < riskMap.size() - 1) {
      adj = getVertex(v->location.first + 1, v->location.second, visited);
      update(v, adj);
    }

    if (v->location.first > 0) {
      adj = getVertex(v->location.first - 1, v->location.second, visited);
      update(v, adj);
    }

    if ((size_t)v->location.second < len - 1) {
      adj = getVertex(v->location.first, v->location.second + 1, visited);
      update(v, adj);
    }

    if (v->location.second > 0) {
      adj = getVertex(v->location.first, v->location.second - 1, visited);
      update(v, adj);
    }
  }

  Vertex* end = visited.find({riskMap.size() - 1, len - 1})->second;

  std::cout << "1. " << end->cost << std::endl;

  for (auto& p : visited) {
    delete p.second;
  }

  visited.clear();


  //Part 2
  size_t maxRow = riskMap.size()*5;
  size_t maxCol = len*5;

  initial = new Vertex({0, 0});
  initial->cost = 0;
  q.push(initial);

  auto update2 = [&](Vertex* v, Vertex* adj) { 
      if (adj->cost == UINT_MAX) {
        setCost(v, adj, riskMap, len);
        q.push(adj);
      } else {
        setCost(v, adj, riskMap, len);
      }
  };

  while (q.size() > 0) {
    v = q.top();
    q.pop();
    
    //Check adjacent
    if ((size_t)v->location.first < maxRow - 1) {
      adj = getVertex(v->location.first + 1, v->location.second, visited);
      update2(v, adj);
    }

    if (v->location.first > 0) {
      adj = getVertex(v->location.first - 1, v->location.second, visited);
      update2(v, adj);
    }

    if ((size_t)v->location.second < maxCol - 1) {
      adj = getVertex(v->location.first, v->location.second + 1, visited);
      update2(v, adj);
    }

    if (v->location.second > 0) {
      adj = getVertex(v->location.first, v->location.second - 1, visited);
      update2(v, adj);
    }
  }

  end = visited.find({maxRow - 1, maxCol - 1})->second;

  std::cout << "2. " << end->cost << std::endl;

  return 0;
}