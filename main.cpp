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

size_t calcDist(const Coord& a, const Coord& b) {
  return abs(a.x - b.x) + abs(a.y - b.y) + abs(a.z - b.z);
}

struct Scanner {
  Rotation rot_x = ROT_0;
  Rotation rot_y = ROT_0;
  Rotation rot_z = ROT_0;
  Coord origin{0,0,0};
  std::vector<Coord> detected;
  std::vector<Coord> transformed;
  std::map<std::pair<size_t, size_t>, size_t> dist;
  std::vector<std::vector<size_t>> distVec;

  void genDists() {

    for (size_t i = 0; i < detected.size(); i++) {

      distVec.push_back(std::vector<size_t>());
      
      for (size_t j = 0; j < detected.size(); j++) {
        if (i == j) {
          dist.insert({{i,j}, 0});
          distVec.back().push_back(0);
          continue;
        }
        
        auto found = dist.find({j, i}); 
        if ( found == dist.end()) {
          size_t distance = calcDist(detected[i], detected[j]);
          dist.insert({ {i, j}, distance });
          distVec.back().push_back(distance);
        } else {
          dist.insert({ {i, j}, found->second });
          distVec.back().push_back(found->second);
        }
      }
      
      std::sort(distVec.back().begin(), distVec.back().end());
    }
  }

  //Transforms the other scanner
  bool pointCompare(Scanner& other, size_t idx_a, size_t idx_b, const RotMatrix& m, std::set<Coord>& matchSet) {
    int x_move, y_move, z_move;
    size_t match = 0;
    std::set<Coord> matchPoints;

    Coord t = m.matrix*other.detected[idx_b];
    x_move = detected[idx_a].x - t.x;
    y_move = detected[idx_a].y - t.y;
    z_move = detected[idx_a].z - t.z;

    match++;
    matchPoints.insert(detected[idx_a]);

    for (size_t i = 0; i < other.detected.size(); i++) {
      if (i != idx_b) {
        t = m.matrix*other.detected[i];
        t.x += x_move;
        t.y += y_move;
        t.z += z_move;
        
        if (std::any_of(detected.begin(), detected.end(), 
                        [&](const auto& p){ return t == p; })) {
          match++;
          matchPoints.insert(t);
        }
      }
    }

    if (match >= 12) {
      other.origin.x = x_move;
      other.origin.y = y_move;
      other.origin.z = z_move;
      other.rot_x = m.rot_x;
      other.rot_y = m.rot_y;
      other.rot_z = m.rot_z;

      for (auto& p : matchPoints) {
        matchSet.insert(p);
      }

      for (size_t i = 0; i < other.detected.size(); i++) {
        other.detected[i] = m.matrix*other.detected[i];
        other.detected[i].x += x_move;
        other.detected[i].y += y_move;
        other.detected[i].z += z_move;
      }
    }

    return match >= 12;
  }
};


bool distMatch(const Scanner& a, const Scanner& b, size_t& idx_a, size_t& idx_b) {
  size_t matchCount = 0;

  for (size_t i = 0; i < a.detected.size() && matchCount < 12; i++) {
    matchCount = 0;
    for (size_t j = 0; j < b.detected.size() && matchCount < 12; j++) {
      matchCount = 0;
      std::vector<size_t> match;
      std::set_intersection(a.distVec[i].begin(), a.distVec[i].end(),
                            b.distVec[j].begin(), b.distVec[j].end(),
                            std::back_inserter(match));
      matchCount = match.size();
      if (matchCount >= 12) {
        idx_a = i;
        idx_b = j;
      }
    }
  }

  return matchCount >= 12;
}


std::vector<Scanner> parse(std::ifstream& f) {
  std::string line;
  std::vector<Scanner> scanners;
  Scanner temp;
  bool initial = true;
  int x, y, z;

  while (std::getline(f, line)) {
    if (line.size() <= 2) continue;
    if (line.at(0) == '-' && line.at(1) == '-' && !initial) {
      scanners.push_back(temp);
      temp = Scanner();
      continue;
    } else if (initial) {
      initial = false;
      continue;
    }
    sscanf_s(line.c_str(), "%d%*c%d%*c%d", &x, &y, &z);
    temp.detected.push_back(Coord{x,y,z});
  }
  scanners.push_back(temp);
  
  return scanners;
}


int main() {
  std::ifstream file("19/data.txt");
  std::vector<Scanner> scanners = parse(file);
  std::vector<RotMatrix> rotations = generateRotations();
  std::set<Coord> beacons;
  std::deque<Scanner*> scannerQueue;
  std::set<const Scanner*> seen; 
  size_t idx_a, idx_b;

  for (auto& s : scanners) {
    s.genDists();
  }

  scannerQueue.push_front(&(scanners[0]));
  seen.insert(&(scanners[0]));

  while (scannerQueue.size() > 0) {
    Scanner* sc = scannerQueue.front();
    scannerQueue.pop_front();
    for (size_t i = 0; i < scanners.size(); i++) {
      if (&(scanners[i]) != sc && seen.find(&(scanners[i])) == seen.end()) {
        if (distMatch(*sc, scanners[i], idx_a, idx_b)) {
          for (const auto& rot : rotations) {
            if (sc->pointCompare(scanners[i], idx_a, idx_b, rot, beacons)) {
              std::cout << std::endl;
              seen.insert(&(scanners[i]));
              scannerQueue.push_front(&(scanners[i]));
              break;
            }
          }
        }
      }
    }
  }

  for (auto& sc : scanners) {
    for (auto& c : sc.detected) {
      beacons.insert(c);
    }

    std::cout << std::endl;
  }

  std::cout << "Scanner locations\n";

  for (const auto& sc : scanners) {
    sc.origin.print();
    std::cout << "\n";
  }

  std::vector<Coord> sortedBeacons;
  for (auto& c : beacons) {
    sortedBeacons.push_back(c);
  }

  std::sort(sortedBeacons.begin(), sortedBeacons.end(), [](auto& a, auto& b){ return a.x < b.x;});
  sortedBeacons.erase(std::unique(sortedBeacons.begin(), sortedBeacons.end()), sortedBeacons.end());

  std::cout << sortedBeacons.size() << std::endl;

  size_t max = 0;

  for (size_t i = 0; i < scanners.size(); i++) {
    for (size_t j = 0; j < scanners.size(); j++) {
      size_t dist = calcDist(scanners[i].origin, scanners[j].origin);
      if (dist > max) max = dist;
    }
  }

  std::cout << max;

  return 0;
}