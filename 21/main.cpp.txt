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

size_t getDiceVal(size_t diceLevel, size_t pos) {
  size_t inc = (diceLevel*3 + pos)/101;
  size_t value = (diceLevel*3 + pos)%101 + inc;
  return value;
}


std::pair<uint64_t, uint64_t> solvePart2(bool player_1_active, size_t diceLevel, size_t pos_1, size_t pos_2, size_t score_1, size_t score_2) {
  static std::vector<std::tuple<size_t, size_t, size_t>> dices {{1, 1, 1}, {1, 1, 2}, {1, 1, 3},
                                                         /*{1, 2, 1},*/ {1, 2, 2}, {1, 2, 3},
                                                         /*{1, 3, 1}, {1, 3, 2},*/ {1, 3, 3},
                                                         /*{2, 1, 1}, {2, 1, 2}, {2, 1, 3},*/
                                                         /*{2, 2, 1},*/ {2, 2, 2}, {2, 2, 3},
                                                         /*{2, 3, 1}, {2, 3, 2},*/ {2, 3, 3},
                                                         /*{3, 1, 1}, {3, 1, 2}, {3, 1, 3},*/
                                                         /*{3, 2, 1}, {3, 2, 2}, {3, 2, 3},*/
                                                         /*{3, 3, 1}, {3, 3, 2},*/ {3, 3, 3}};
  static std::vector<size_t> dicePermute{ 1, 3, 3, 3, 6, 3, 1, 3, 3, 1 };
  static std::unordered_map<std::string, std::pair<uint64_t, uint64_t>> wins;
  size_t diceValue, d1, d2, d3;
  size_t orig_score_1 = score_1, orig_score_2 = score_2;
  size_t orig_pos_1 = pos_1, orig_pos_2 = pos_2;
  std::pair<uint64_t, uint64_t> winCount = {0 , 0};
  std::string key;
  key.append(1, (player_1_active?'1':'2'));
  key.append(1, '-');
  key.append(1, (char)pos_1 + 48);
  key.append(1, (char)pos_2 + 48);
  key.append(1, (char)score_1 + 48);
  key.append(1, (char)score_2 + 48);

  //std::cout << "Calling with pos " << pos_1 + 1 << " " << pos_2 + 1 << " and score " << score_1 << " " << score_2 << std::endl;

  if (wins.find(key) != wins.end()) {
    return wins[key];
  }


  for (size_t i = 0; i < dices.size(); i++) {
    if (player_1_active) {
      std::tie(d1, d2, d3) = dices[i];
      diceValue = d1 + d2 + d3;
      pos_1 = (orig_pos_1 + diceValue)%10;
      score_1 = orig_score_1 + pos_1 + 1;
      if (score_1 >= 21) {
        winCount.first += dicePermute[i];
      } else {
        std::pair<uint64_t, uint64_t> temp = solvePart2(!player_1_active, diceLevel+1, pos_1, pos_2, score_1, score_2);
        winCount.first += dicePermute[i]*temp.first;
        winCount.second += dicePermute[i]*temp.second;
      }
    } else {
      std::tie(d1, d2, d3) = dices[i];
      diceValue = d1 + d2 + d3; 
      pos_2 = (orig_pos_2 + diceValue)%10;
      score_2 = orig_score_2 + pos_2 + 1;
      if (score_2 >= 21) {
        winCount.second += dicePermute[i];
      } else {
        std::pair<uint64_t, uint64_t> temp = solvePart2(!player_1_active, diceLevel+1, pos_1, pos_2, score_1, score_2);
        winCount.first += dicePermute[i]*temp.first;
        winCount.second += dicePermute[i]*temp.second;
      }
    }
  }

  wins[key] = winCount;

  return winCount;
}


int main() {
  //std::ifstream file("20/data.txt");
  size_t score_1, score_2;
  size_t diceLevel = 0, diceValue = 0;
  score_1 = 0;
  score_2 = 0;
  size_t pos_1 = 6 - 1, pos_2 = 4 - 1;
  bool p1Winner = false;
  size_t rollCount = 0;
  

  while (score_1 < 1000 && score_2 < 1000) {
    diceValue = getDiceVal(diceLevel, 1) + getDiceVal(diceLevel, 2) + getDiceVal(diceLevel, 3);

    pos_1 = (pos_1+diceValue)%10;
    score_1 += pos_1 + 1;
    rollCount++;
    diceLevel++;
    
    if (score_1 >= 1000) {
      p1Winner = true;
      break;
    }
    
    
    diceValue = getDiceVal(diceLevel, 1) + getDiceVal(diceLevel, 2) + getDiceVal(diceLevel, 3);
    pos_2 = (pos_2+diceValue)%10;
    score_2 += pos_2 + 1;
    rollCount++;
    diceLevel++;
  } 

  if (p1Winner) {
    std::cout << "Using player 2 as loser\n";
    std::cout << score_2 << " * " << rollCount*3 << " = " << score_2*rollCount*3;
  } else {
    std::cout << "Using player 1 as loser\n";
    std::cout << score_1 << " * " << rollCount*3 << " = " << score_1*rollCount*3;
  }
  std::cout << std::endl;

  score_1 = 0;
  score_2 = 0;
  pos_1 = 6 - 1;
  pos_2 = 4 - 1;
  diceLevel = 0;

  auto wins = solvePart2(true, diceLevel, pos_1, pos_2, score_1, score_2);

  std::cout << max(wins.first, wins.second);
  
  return 0;
}