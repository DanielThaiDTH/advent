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


enum ParseState {
  LEFT,
  RIGHT
};

struct SnailNum {
  bool leftIsValue = true;
  bool rightIsValue = true;
  short left;
  short right;
  SnailNum* leftNum;
  SnailNum* rightNum;
  SnailNum* parent;
  short height = 0;

  SnailNum(SnailNum* parent = nullptr) {
    this->parent = parent;
  }
  
  void print() {
    std::cout << "[";
    if (!leftIsValue) {
      leftNum->print();
    } else {
      std::cout << left;
    }
    std::cout << ",";
    if (!rightIsValue) {
      rightNum->print();
    } else {
      std::cout << right;
    }
    std::cout << "]";
  }

  short calcHeight() {
    short h = 1;
    short h1 = (leftIsValue)? 0 : leftNum->calcHeight();
    short h2 = (rightIsValue)? 0 : rightNum->calcHeight();

    height = h + max(h1, h2);
    return height;
  }

  size_t mag() {
    size_t l_val = (leftIsValue)? 3*left : 3*leftNum->mag();
    size_t r_val = (rightIsValue)? 2*right : 2*rightNum->mag();

    return l_val + r_val;
  }
};

SnailNum* deepCopy(const SnailNum* src) {
  SnailNum* copy = new SnailNum();

  copy->leftIsValue = src->leftIsValue;
  copy->rightIsValue = src->rightIsValue;
  copy->left = src->left;
  copy->right = src->right;
  if (!copy->leftIsValue) {
    copy->leftNum = deepCopy(src->leftNum);
    copy->leftNum->parent = copy;
  }
  if (!copy->rightIsValue) {
    copy->rightNum = deepCopy(src->rightNum);
    copy->rightNum->parent = copy;
  }

  return copy;
}

bool split(SnailNum* root) {
  std::set<SnailNum*> visited;
  SnailNum* curr;
  bool splitted = false;
  int depth = 1;

  curr = root;

  while (!splitted && curr) {
    visited.insert(curr);
    if (!curr->leftIsValue && visited.find(curr->leftNum) == visited.end()) {
      curr = curr->leftNum;
      depth++;
    } else if (curr->leftIsValue && curr->left >= 10) {
      curr->leftIsValue = false;
      curr->leftNum = new SnailNum(curr);
      curr->leftNum->left = curr->left/2;
      curr->leftNum->right = (curr->left+1)/2;
      curr->left = 0;
      visited.insert(curr->leftNum);
      splitted = true;
    } else if (!curr->rightIsValue && visited.find(curr->rightNum) == visited.end()) {
      curr = curr->rightNum;
      depth++;
    }else if (curr->rightIsValue && curr->right >= 10) {
      curr->rightIsValue = false;
      curr->rightNum = new SnailNum(curr);
      curr->rightNum->left = curr->right/2;
      curr->rightNum->right = (curr->right+1)/2;
      curr->right = 0;
      visited.insert(curr->rightNum);
      splitted = true;
    } else {
      curr = curr->parent;
      depth--;
    }
  }


  return splitted;
}

bool splittable(SnailNum* root) {
  std::set<SnailNum*> visited;
  SnailNum* curr;
  bool found = false;

  curr = root;

  while (curr && !found) {
    visited.insert(curr);
    if (!curr->leftIsValue && visited.find(curr->leftNum) == visited.end()) {
      curr = curr->leftNum;
    } else if (!curr->rightIsValue && visited.find(curr->rightNum) == visited.end()) {
      curr = curr->rightNum;
    } else if (curr->leftIsValue && curr->left >= 10) {
      found = true;
    } else if (curr->rightIsValue && curr->right >= 10) {
      found = true;
    } else {
      curr = curr->parent;
    }
  }

  return found;
}

bool explode(SnailNum* root) {
  short depth = 1;
  short carryLeft = -1, carryRight = -1;
  std::set<SnailNum*> visited;
  SnailNum* curr;
  SnailNum* nav;
  SnailNum* prevNav;
  SnailNum* temp;
  bool exploded = false;

  curr = root;
  while (!exploded && curr && depth > 0) {
    visited.insert(curr);
    
    if (!curr->leftIsValue && visited.find(curr->leftNum) == visited.end()) {
      depth++;
      curr = curr->leftNum;
    } else if (!curr->rightIsValue && visited.find(curr->rightNum) == visited.end()) {
      depth++;
      curr = curr->rightNum;
    } else if (curr->rightIsValue && curr->leftIsValue) {
      if (depth > 4) {
        carryRight = curr->right;
        carryLeft = curr->left;
        
        ///Carry left
        nav = curr->parent;
        prevNav = curr;
        while (nav && carryLeft != -1) {
          if (nav->leftIsValue) {
            nav->left += carryLeft;
            carryLeft = -1;  
          } else {
            temp = nav->leftNum;
            if (prevNav != temp) {
              while (!temp->rightIsValue) {
                temp = temp->rightNum;
              }
              temp->right += carryLeft;
              carryLeft = -1;
            }
          }

          prevNav = nav;
          nav = nav->parent;
        }

        ///Carry right
        nav = curr->parent;
        prevNav = curr;
        while (nav && carryRight != -1) {
          if (nav->rightIsValue) {
            nav->right += carryRight;
            carryRight = -1;  
          } else {
            temp = nav->rightNum;
            if (prevNav != temp) {
              while (!temp->leftIsValue) {
                temp = temp->leftNum;
              }
              temp->left += carryRight;
              carryRight = -1;
            }
          }
          prevNav = nav;
          nav = nav->parent;
        }


        nav = curr->parent;
        if (curr == nav->leftNum) {
          nav->leftNum = nullptr;
          nav->left = 0;
          nav->leftIsValue = true;
        } else {
          nav->rightNum = nullptr;
          nav->right = 0;
          nav->rightIsValue = true;
        }
        //delete curr;


        exploded = true;
        curr = nav;
        carryRight = -1;
        carryLeft = -1;
      } else {
        curr = curr->parent;
      }
      depth--;
    } else {
      curr = curr->parent;
      depth--;
    }

  }

  return exploded;
}


SnailNum* add(SnailNum* a, SnailNum* b) {
  SnailNum* sum = new SnailNum();
  bool splitWatch = true;
  bool explodeWatch = true;

  sum->leftIsValue = false;
  sum->rightIsValue = false;

  a->parent = sum;
  b->parent = sum;

  sum->leftNum = a;
  sum->rightNum = b;

  short h = sum->calcHeight();
  explodeWatch = sum->calcHeight() > 4;
  splitWatch = splittable(sum);

  while (explodeWatch || splitWatch) {
    if (explodeWatch && explode(sum)) {
      explodeWatch = sum->calcHeight() > 4;
      splitWatch = splittable(sum);
    } else if (splitWatch && split(sum)) {
      explodeWatch = sum->calcHeight() > 4;
      splitWatch = splittable(sum);
    }
  }

  return sum;
}


std::vector<SnailNum*> parse(std::ifstream& f) {
  SnailNum* activeNum = nullptr;
  SnailNum* rootNum = nullptr;
  std::stack<SnailNum*> currNum;
  std::vector<SnailNum*> nums;
  std::string line;
  ParseState state = LEFT;

  while (std::getline(f, line)) {
    for (const auto& c : line) {
      if (c == '[') {
        if (currNum.size() == 0)
          activeNum = new SnailNum();
        else
          activeNum = new SnailNum(currNum.top());
        if (currNum.size() > 0 && state == LEFT) {
          currNum.top()->leftIsValue = false;
          currNum.top()->leftNum = activeNum;
        } else if (currNum.size() > 0 && state == RIGHT) {
          currNum.top()->rightIsValue = false;
          currNum.top()->rightNum = activeNum;
        }
        currNum.push(activeNum);
        state = LEFT;
      } else if (c == ',') {
        state = RIGHT;
      } else if (c == ']') {
        if (currNum.size() == 1)
          rootNum = currNum.top();
        currNum.pop();
      } else {
        if (state == LEFT) {
          currNum.top()->leftIsValue = true;
          currNum.top()->left = (short)(c - '0');
        } else if (state == RIGHT) {
          currNum.top()->rightIsValue = true;
          currNum.top()->right = (short)(c - '0');
        }
      }
    }
    rootNum->calcHeight();
    nums.push_back(rootNum);
  }
  
  return nums;
}


void clear(SnailNum* root) {
  std::deque<SnailNum*> q;
  q.push_back(root);
  while (q.size() > 0) {
    SnailNum* del = q.front();
    q.pop_front();
    if (!del->leftIsValue)
      q.push_back(del->leftNum);
    if (!del->rightIsValue)
      q.push_back(del->rightNum);
    delete del;
  }
}


int main() {
  std::ifstream file("18/data.txt");
  std::vector<SnailNum*> numbers = parse(file);

  if (numbers.size() > 1) {
    //Part 1

    SnailNum* num = add(numbers[0], numbers[1]);
    for (auto i = 2u; i < numbers.size(); i++) {
      num = add(num, numbers[i]);
    }

    std::cout << num->mag();

    clear(num);


    //Part 2
    // size_t max = 0;
    // size_t mag;
    // SnailNum* temp;

    // for (auto i = 0u; i < numbers.size(); i++) {
    //   for (auto j = 0u; j < numbers.size(); j++) {
    //     if (i != j) {
    //       temp = add(deepCopy(numbers[i]), deepCopy(numbers[j]));
    //       mag = temp->mag();
    //       if (mag > max) {
    //         max = mag;
    //       } 
            
    //     }
    //   }
    // }
    // std::cout << max;

    
    std::cout << std::endl;
  }

  return 0;
}