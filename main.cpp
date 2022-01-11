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

struct Cuboid {
  int64_t x_1;
  int64_t x_2;
  int64_t y_1;
  int64_t y_2;
  int64_t z_1;
  int64_t z_2;
  uint64_t volume;

  Cuboid(int64_t x1, int64_t x2, int64_t y1, int64_t y2, int64_t z1, int64_t z2) {
    x_1 = x1;
    x_2 = x2;
    y_1 = y1;
    y_2 = y2;
    z_1 = z1;
    z_2 = z2;
    volume = llabs((x2-x1+1)*(y2-y1+1)*(z2-z1+1));
  }
};


struct Action : public Cuboid {
  bool turnOn;

  Action(bool isOn, int64_t x1, int64_t x2, int64_t y1, int64_t y2, int64_t z1, int64_t z2) : 
          Cuboid(x1, x2, y1, y2, z1, z2) {
    turnOn = isOn;
  }
};


struct Intersection : public Cuboid {
  size_t genIdx;
  size_t covIdx;
  bool phantom;
  bool dirty;
  Intersection(size_t idx, bool img, int64_t x1, int64_t x2, int64_t y1, int64_t y2, int64_t z1, int64_t z2) :
                Cuboid(x1, x2, y1, y2, z1, z2) {
    phantom = img;
    genIdx = idx;
    covIdx = 0;
    dirty = false;
  }
};


bool intersection(const Cuboid* a, const Cuboid* b) {
  bool intersect = true;

  intersect = (a->x_1 >= b->x_1 && a->x_1 <= b->x_2) || 
              (a->x_2 >= b->x_1 && a->x_2 <= b->x_2) ||
              (b->x_1 >= a->x_1 && b->x_1 <= a->x_2) ||
              (b->x_2 >= a->x_1 && b->x_2 <= a->x_2);
  intersect = intersect && ((a->y_1 >= b->y_1 && a->y_1 <= b->y_2) ||
                            (a->y_2 >= b->y_1 && a->y_2 <= b->y_2) ||
                            (b->y_1 >= a->y_1 && b->y_1 <= a->y_2) ||
                            (b->y_2 >= a->y_1 && b->y_2 <= a->y_2));
  intersect = intersect && ((a->z_1 >= b->z_1 && a->z_1 <= b->z_2) || 
                            (a->z_2 >= b->z_1 && a->z_2 <= b->z_2) ||
                            (b->z_1 >= a->z_1 && b->z_1 <= a->z_2) ||
                            (b->z_2 >= a->z_1 && b->z_2 <= a->z_2));

  return intersect;
}


Intersection* getIntersection(const Action* a, const Action* b) {
  int64_t x1, x2, y1, y2, z1, z2;

  x1 = max(a->x_1, b->x_1);
  x2 = min(a->x_2, b->x_2); 
  y1 = max(a->y_1, b->y_1);
  y2 = min(a->y_2, b->y_2);
  z1 = max(a->z_1, b->z_1);
  z2 = min(a->z_2, b->z_2);

  return new Intersection(0, !a->turnOn, x1, x2, y1, y2, z1, z2);
}


uint64_t getIntersectionArea(const Intersection* a, const Intersection* b) {
  int64_t x1, x2, y1, y2, z1, z2;

  x1 = max(a->x_1, b->x_1);
  x2 = min(a->x_2, b->x_2); 
  y1 = max(a->y_1, b->y_1);
  y2 = min(a->y_2, b->y_2);
  z1 = max(a->z_1, b->z_1);
  z2 = min(a->z_2, b->z_2);

  return llabs((x2-x1+1)*(y2-y1+1)*(z2-z1+1));
}


std::vector<Action*> parse(std::ifstream& f) {
  std::string line;
  std::string actionType;
  std::vector<Action*> actions;
  bool onAction;
  char c;
  int64_t x1, x2, y1, y2, z1, z2;

  while (std::getline(f, line)) {
    std::istringstream lineStr(line);

    lineStr >> actionType;
    onAction = actionType == "on";
    lineStr >> c;
    lineStr >> c;
    lineStr >> x1;
    lineStr >> c;
    lineStr >> c;
    lineStr >> x2;
    lineStr >> c;
    lineStr >> c;
    lineStr >> c;
    lineStr >> y1;
    lineStr >> c;
    lineStr >> c;
    lineStr >> y2;
    lineStr >> c;
    lineStr >> c;
    lineStr >> c;
    lineStr >> z1;
    lineStr >> c;
    lineStr >> c;
    lineStr >> z2;

    actions.push_back(new Action(onAction, x1, x2, y1, y2, z1, z2));
  }
  
  return actions;
}


int main() {
  std::ifstream file("22/test.txt");
  std::ifstream file2("22/err.txt");
  std::vector<Action*> actions = parse(file);
  std::map<std::tuple<int64_t, int64_t, int64_t>, bool> cubes;
  std::vector<Intersection*> intersections;
  std::map<size_t, std::vector<Intersection*>> toBeFilled;
  std::map<size_t, std::vector<size_t>> intersectIdxMap;
  std::map<std::pair<size_t, size_t>, Intersection*> intersectionMap;
  std::set<std::pair<size_t, size_t>> seen;
  
  //Part 1
  size_t cnt = 0;
  int64_t minv = -50;
  int64_t maxv = 50;
  int64_t minx, maxx, miny, maxy, minz, maxz;
  for (auto act : actions) {
    if (act->x_1 > 50 || act->x_2 < -50) continue;
    minx = (minv > act->x_1) ? minv : act->x_1;
    maxx = (maxv < act->x_2) ? maxv : act->x_2;
    for (auto x = minx; x <= maxx; x++) {
      if (act->y_1 > 50 || act->y_2 < -50) continue; 
      miny = (minv > act->y_1) ? minv : act->y_1;
      maxy = (maxv < act->y_2) ? maxv : act->y_2;
      for (auto y = miny; y <= maxy; y++) {
        if (act->z_1 > 50 || act->z_2 < -50) continue;
        minz = (minv > act->z_1) ? minv : act->z_1;
        maxz = (maxv < act->z_2) ? maxv : act->z_2;
        for (auto z = minz; z <= maxz; z++) {
          cubes[{x, y, z}] = act->turnOn;
        }
      }
    }
  }

  uint64_t count = 0;
  for (auto& c : cubes) {
    if (c.second) {
      count++;
    }
  }

  std::cout << count << std::endl;
  cubes.clear();

  for (auto act : actions) {
    delete act;
  }
  actions.clear();

  actions = parse(file2);

  //Part 2
  count = 0;
  for (size_t i = 0; i < actions.size(); i++) {
    std::vector<size_t> intersectList;
    std::vector<Intersection*> unfilled;

    std::cout << intersections.size() << "\n";
    if (intersections.size() > 500000) {
      std::cout << "Out of memory error\n";
      for (auto inter : intersections)
        delete inter;
      exit(1);
    }

    for (size_t j = 0; j < i; j++) { 
      if (actions[j]->turnOn && intersection(actions[j], actions[i])) {
        Intersection* temp = getIntersection(actions[j], actions[i]);
        intersections.push_back(temp);
        intersections.back()->genIdx = i;
        intersections.back()->covIdx = j;
        if (actions[i]->turnOn) {
          intersectList.push_back(j);
          intersectionMap.insert({{i, j}, temp});
        } else {
          unfilled.push_back(temp);
        }
      }
    }

    intersectIdxMap.insert({i, intersectList});
    toBeFilled.insert({i, unfilled});
  }

  std::cout << intersections.size() << " at end\n";

  for (auto act : actions) {
    if (act->turnOn)
      count += act->volume;
  }

  std::cout << count << std::endl;

  //uint64_t vol;
  for (size_t i = 0; i < actions.size(); i++) {
    for (auto& j : intersectIdxMap.find(i)->second) {
      Intersection* inter = intersectionMap.find({i, j})->second;
      count -= inter->volume;
      for (auto& innerIdx : intersectIdxMap.find(j)->second) {
        Intersection* cand = intersectionMap.find({j, innerIdx})->second;
        if (intersection(cand, inter)) {
          std::cout << "intersect of " << j << " and " << innerIdx << " intersects with intersect of " << i << " and " << j << std::endl;
          count += getIntersectionArea(cand, inter);
        }
      }

      for (auto& fill : toBeFilled) {
        if (fill.first < i) {
          for (auto& f : fill.second) {
            if (intersection(f, inter)) {

            }
          }
        }
      }
    }
  }

  std::cout << count;

  return 0;
}