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

enum PacketType {
  SUM = 0,
  PRODUCT = 1,
  MIN = 2,
  MAX = 3,
  LITERAL = 4,
  GT = 5,
  LT = 6,
  EQ = 7
};

std::ostream& operator<<(std::ostream& out, PacketType type) {
  switch (type) {
    case SUM:
      out << "SUM";
      break;
    case PRODUCT:
      out << "PRODUCT";
      break;
    case MIN:
      out << "MIN";
      break;
    case MAX:
      out << "MAX";
      break;
    case LITERAL:
      out << "Literal";
      break;
    case GT:
      out << "Greater than";
      break;
    case LT:
      out << "Less than";
      break;
    case EQ:
      out << "Equal";
      break;
  }

  return out;
}


enum ParseState {
  HEADER,
  VALUE
};


struct Packet {
  short version;
  PacketType type;
  size_t value;
  size_t length;
  std::vector<Packet*> subpackets;
};


std::vector<bool> parse(std::ifstream& f) {
  std::map<char, std::vector<bool>> hexToBool;
  hexToBool.insert({'0', {0, 0, 0, 0}}); 
  hexToBool.insert({'1', {0, 0, 0, 1}});
  hexToBool.insert({'2', {0, 0, 1, 0}});
  hexToBool.insert({'3', {0, 0, 1, 1}});
  hexToBool.insert({'4', {0, 1, 0, 0}});
  hexToBool.insert({'5', {0, 1, 0, 1}}); 
  hexToBool.insert({'6', {0, 1, 1, 0}});
  hexToBool.insert({'7', {0, 1, 1, 1}});
  hexToBool.insert({'8', {1, 0, 0, 0}});
  hexToBool.insert({'9', {1, 0, 0, 1}});
  hexToBool.insert({'A', {1, 0, 1, 0}});
  hexToBool.insert({'B', {1, 0, 1, 1}});
  hexToBool.insert({'C', {1, 1, 0, 0}});
  hexToBool.insert({'D', {1, 1, 0, 1}});
  hexToBool.insert({'E', {1, 1, 1, 0}});
  hexToBool.insert({'F', {1, 1, 1, 1}});

  std::vector<bool> bin;
  std::string line;
  std::getline(f, line);

  for (const auto& c : line) {
    for (bool b : hexToBool.find(c)->second) {
      bin.push_back(b);
    }
  }

  return bin;
}


size_t fromBinary(const std::vector<bool>& bin, size_t& pos, size_t count) {
  size_t num = 0;

  for (auto i = count + pos - 1; i >= pos; i--) {
    num += (size_t)bin[i] << (count + pos - 1 - i);
  }

  pos += count;

  return num;
}


size_t versionSum(Packet* p) {
  size_t val = p->version;

  for (auto sp : p->subpackets) {
    val += versionSum(sp);
  }

  return val;
}


size_t eval(Packet* p) {
  if (p->type == LITERAL) {
    return p->value;
  } else if (p->type == SUM) {
    size_t val = 0;
    for (auto sp : p->subpackets) {
      val += eval(sp);
    }
    return val;
  } else if (p->type == PRODUCT) {
    size_t val = 1;
    for (auto sp : p->subpackets) {
      val *= eval(sp);
    }
    return val;
  } else if (p->type == MIN) {
    size_t min = UINT_MAX;
    size_t subval;
    for (auto sp : p->subpackets) {
      subval = eval(sp);
      if (subval < min)
        min = subval;
    }
    return min;
  } else if (p->type == MAX) {
    size_t max = 0;
    size_t subval;
    for (auto sp : p->subpackets) {
      subval = eval(sp);
      if (subval > max)
        max = subval;
    }
    return max;
  } else if (p->type == GT) {
    return eval(p->subpackets[0]) > eval(p->subpackets[1]);
  } else if (p->type == LT) {
    return eval(p->subpackets[0]) < eval(p->subpackets[1]);
  } else if (p->type == EQ) {
    return eval(p->subpackets[0]) == eval(p->subpackets[1]);
  } else {
    std::cout << "Unknown path reached\n";
    return 0;
  }

}


Packet* parsePacket(const std::vector<bool>& bin, size_t& pos, ParseState state = HEADER) {
  Packet* newPacket = new Packet();
  newPacket->version = ((short)bin[pos] << 2) + ((short)bin[pos+1] << 1) + (short)bin[pos+2];
  // std::cout << "version " << newPacket->version << ", ";
  pos += 3;
  newPacket->type = static_cast<PacketType>(static_cast<short>(bin[pos] << 2) 
                                            + (static_cast<short>(bin[pos+1]) << 1) 
                                            + static_cast<short>(bin[pos+2]));
  pos += 3;

  //std::cout << "type " << newPacket->type << "\n";

  if (newPacket->type == LITERAL) {
    bool cont = bin[pos];
    std::vector<bool> binaryValue;
    pos++;

    while (cont) {
      binaryValue.push_back(bin[pos]);
      binaryValue.push_back(bin[pos+1]);
      binaryValue.push_back(bin[pos+2]);
      binaryValue.push_back(bin[pos+3]);
      pos += 4;

      cont = bin[pos];
      pos++;
    }

    //end
    binaryValue.push_back(bin[pos]);
    binaryValue.push_back(bin[pos+1]);
    binaryValue.push_back(bin[pos+2]);
    binaryValue.push_back(bin[pos+3]);
    pos += 4;

    newPacket->value = 0;
    for (auto i = (int)binaryValue.size() - 1; i >= 0; --i) {
      //std::cout << newPacket->value << " value prev, i = " << i << "\n";
      newPacket->value += (size_t)binaryValue[i] << (binaryValue.size() - 1 - i);
      //std::cout << newPacket->value << " value updated, i = " << i << "\n";
    }
  } else {
    bool isBinSize = !bin[pos];
    pos++;
    if(isBinSize) {
      size_t length = fromBinary(bin, pos, 15);
      size_t origPos = pos;
      while (pos <= origPos + length && (origPos + length - pos) > 9) {
        //std::cout << pos << " " << origPos << " " << origPos + length << "\n";
        newPacket->subpackets.push_back(parsePacket(bin, pos));
      }
    } else {
      size_t packetCount = fromBinary(bin, pos, 11);
      for (auto i = 0u; i < packetCount; i++) {
        newPacket->subpackets.push_back(parsePacket(bin, pos));
      }
    }
  }

  return newPacket;
}

int main() {
  std::ifstream file("16/data.txt");
  std::vector<bool> bin = parse(file);
  size_t pos = 0;

  // for (bool b : bin) {
  //   if (b)
  //     std::cout << "1";
  //   else
  //     std::cout << "0";
  // }
  std::cout << std::endl;

  Packet* p = parsePacket(bin, pos);

  std::cout << "Parsed\n";

  std::cout << versionSum(p) << std::endl;
  std::cout << eval(p);

  return 0;
}