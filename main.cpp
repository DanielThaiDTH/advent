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


struct Node {
  std::vector<Node*> neighbours;
  bool isBig;
  std::string name;
  int visited;
  
  Node(std::string n, bool big) {
    name = n;
    isBig = big;
    visited = 0;
  }

  void addNeighbour(Node* neigh) {
    auto check = [&](Node*& n){ return n == neigh; };
    if (std::none_of(neighbours.begin(), neighbours.end(), check))
      neighbours.push_back(neigh);
  }

  bool operator==(const Node& rhs) {
    return this->name == rhs.name;
  }

  bool operator!=(const Node& rhs) {
    return this->name != rhs.name;
  }
};

void print(const std::vector<Node*> list) {
  for (int i = 0; i < list.size(); i++) {
    std::cout << list[i]->name << " ";
  }

  std::cout << std::endl;
}

void printSet(const std::set<Node*> set) {
  for (auto n : set) {
    std::cout << n->name << ": ";
    for (auto nei : n->neighbours) {
      std::cout << nei->name << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl;
}

Node* contains(const std::set<Node*>& s, Node* n) {
  bool found = false;
  Node* match = nullptr;

  for (auto node : s) {
    found = n->name == node->name;
    if (found) {
      match = node;
      break;
    }
  }

  return match;
}

Node* find(const std::set<Node*>& s, std::string name) {
  Node* found = nullptr;

  for (auto node : s) {
    found = (node->name == name)?node:nullptr;
    if (found) {
      break;
    }
  }

  return found;
}

bool inPath(std::vector<Node*>& path, Node* node) {
  auto check = [&](Node*& n) { return !node->isBig && !(visited < 2) && n->name == node->name; };
  return any_of(path.begin(), path.end(), check);
}

//Returns true if no match
bool visitedPathCheck(const std::vector<std::vector<Node*>>& finished, const std::vector<Node*>& current, Node* cand) {
  bool allNodesMatched = true;
  bool noMatch = true;

  for (const auto& p : finished) {
    allNodesMatched = true;
    if (p.size() == current.size() + 1) {
      for (auto i = 1u; i < p.size() && allNodesMatched; i++) {
        if (i < current.size())
          allNodesMatched = allNodesMatched && p[i] == current[i];
        else
          allNodesMatched = allNodesMatched && p[i] == cand;
      }
    } else {
      allNodesMatched = false;
    }

    noMatch = noMatch && !allNodesMatched;

    if (!noMatch) break;
  }

  return noMatch;
}

bool notIsolated(const Node* curr, const Node* cand) {
  return curr->isBig || (!curr->isBig && (cand->neighbours.size() > 1 || visited < 2));
}

std::set<Node*> parse(std::ifstream& f) {
  std::set<Node*> nodes;
  std::string line;
  size_t pos;
  bool isBig, isBig2;

  while(std::getline(f, line)) {
    isBig = std::islower(line[0]) == 0;
    pos = line.find_first_of('-');
    isBig2 = std::islower(line[pos+1]) == 0;

    Node* n1 = new Node(line.substr(0,pos), isBig);
    Node* n2 = new Node(line.substr(pos+1), isBig2);

    Node* n1check = contains(nodes, n1);
    Node* n2check = contains(nodes, n2);
    if (!n1check) nodes.insert(n1); 
    if (!n2check) nodes.insert(n2); 
    
    if (!n2check && !n1check) {
      n1->addNeighbour(n2);
      n2->addNeighbour(n1);
    } else if (!n2check) {
      n1check->addNeighbour(n2);
      n2->addNeighbour(n1check);
      delete n1;
    } else if (!n1check) {
      n1->addNeighbour(n2check);
      n2check->addNeighbour(n1);
      delete n2;
    } else {
      n1check->addNeighbour(n2check);
      n2check->addNeighbour(n1check);
      delete n1;
      delete n2;
    }
  }

  return nodes;
}

int main() {
  std::ifstream file("12/test.txt");
  std::set<Node*> nodes = parse(file);
  std::vector<std::vector<Node*>> visitedPaths;
  std::vector<Node*> currentPath;
  std::stack<size_t> neighbour_idxs;
  bool foundPath = false;
  size_t count = 0;

  Node* curr = find(nodes, "start");
  printSet(nodes);

  do {
    if (currentPath.size() == 0 || curr != currentPath.back()) {
      currentPath.push_back(curr);
      neighbour_idxs.push(0);
      if (curr->name == "end") {
        visitedPaths.push_back(currentPath);
        count++;
        currentPath.back()->visited--;
        currentPath.pop_back();
        curr = currentPath.back();
      }
    }

    Node* n = curr->neighbours[neighbour_idxs.top()];

    for (Node* n : curr->neighbours) {
      if (!inPath(currentPath, n) && n->name != "start"
          && visitedPathCheck(visitedPaths, currentPath, n) 
          && notIsolated(curr, n) ) {
        curr = n;
        if (!curr->isBig) {
          curr->visited++;
          if (curr->visited >= 2)
            visited = 2;
        }
        foundPath = true;
      }

      if (foundPath) break;
    }

    if (!foundPath) {
      visitedPaths.push_back(currentPath);
      if (!currentPath.back()->isBig) {
        currentPath.back()->visited--;
        if (currentPath.back()->visited == 1)
          visited = 0;
      }
      currentPath.pop_back();
      if (currentPath.size() > 0)
        curr = currentPath.back();
    }

    foundPath = false;
    
  } while(currentPath.size() > 0);

  std::cout << count;

  for (Node* node : nodes) {
    delete node;
  }

  return 0;
}