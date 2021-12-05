#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <sstream>

static const int BITSIZE = 12;
static const int FIELDSIZE = 5;
static const int BUNCH = 5;

static_assert(FIELDSIZE%BUNCH == 0);

template<typename T>
void map(std::vector<T>& vec, std::function<bool(const T&)> f)
{
    for (auto it = vec.begin(), it != vec.end();) {
        if (!f(*it)) {
            vec.erase();
        } else {
            ++it;
        }
    }
}

std::vector<std::string> readFileList(std::string filename)
{
	std::ifstream listfile(filename);

    if (!listfile)
        std::cout << "Could not read file\n";

	std::string line;
	std::vector<std::string> lines;

	while (std::getline(listfile, line)) {
		//char* copy = new char[line.size() + 1];
		//std::strcpy(copy, line.c_str());
		lines.push_back(line);
	}

	return lines;
}

enum DIRECTION { FOR, UP, DOWN };

struct Board
{
    int numbers[FIELDSIZE][FIELDSIZE] = {};
    bool matched[FIELDSIZE][FIELDSIZE] = {};
    int colCount[FIELDSIZE] = {};
    int rowCount[FIELDSIZE] = {};
    bool isBingo = false;
    void print()
    {
        for (int i = 0; i < FIELDSIZE; i++) {
            for (int j = 0; j < FIELDSIZE; j++) {
                std::cout << numbers[i][j] << " ";
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }


    bool check(int val)
    {
        bool rowfound = false;
        bool colfound = false;
        
        if (isBingo)
            return isBingo;

        for (int i = 0; i < FIELDSIZE; i++) {
            for (int j = 0; j < FIELDSIZE; j++) {
                if (!matched[i][j] && val == numbers[i][j]) {
                    matched[i][j] = true;
                    colCount[j] += 1;
                    rowCount[i] += 1;
                    colfound = colCount[j] == FIELDSIZE;
                    rowfound = rowCount[i] == FIELDSIZE;
                }
                if (colfound || rowfound)
                    break;
            }
            if (colfound || rowfound)
                break;
        }

        isBingo = rowfound || colfound;

        return isBingo;
    }

    int sum()
    {
        int total = 0;
        for (int i = 0; i < FIELDSIZE; i++) {
            for (int j = 0; j < FIELDSIZE; j++) {
                if (!matched[i][j]) {
                    total += numbers[i][j];
                }
            }
        }

        return total;
    }
};


std::vector<int> parseCSV(const std::string& line)
{
    std::istringstream in(line);
    std::vector<int> nums;
    int num = 0;
    char c;
    while (in >> num) {
        nums.push_back(num);
        in >> c;
    }

    std::cout << std::endl;

    return nums;
}

Board* parseBoard(const std::vector<std::string>& lines, int start, int end)
{
    Board* b = new Board();
    for (int i = start + 1; i < end; i++) {
        std::istringstream row(lines[i]);
        //std::cout << "Building on line: " << row.str() << std::endl;
        for (int cnt = 0; cnt < FIELDSIZE; cnt++) {
            row >> b->numbers[i - 1 - start][cnt];
        }
        //std::cout << std::endl;
    }

    //b->print();
    return b;
}

int main()
{
    std::ifstream file("4/big2.txt");

    std::string drawnStr;
    std::string line;
    std::vector<std::string> lines;
    std::vector<int> splits;
    std::vector<int> drawnNums;
    std::vector<Board*> boards;
    int count = 0;
    
    std::getline(file, drawnStr);
    drawnNums = parseCSV(drawnStr);

    while (std::getline(file, line)) {
        lines.push_back(line);
        if (line.size() <= 2) {
            splits.push_back(count);
        }
        count++;
    }



    for (auto s = 0u; s < splits.size() - 1; s++) {
        boards.push_back(parseBoard(lines, splits[s], splits[s+1]));
    }
    //end board
    boards.push_back(parseBoard(lines, splits[splits.size() - 1], lines.size()));

    bool bingoCalled = false;
    int finalSum = 0;
    int finalNum = 0;
    Board* finalBoard = nullptr;
    for (int num : drawnNums) {
        for (Board* b : boards) {
            if (b->isBingo) continue;
            bool bingo = b->check(num);
            if (bingo) {
                if (!bingoCalled) {
                    std::cout << "Bingo on " << num << ", value: ";
                    std::cout << num*b->sum() << std::endl;
                    bingoCalled = true;
                } else {
                    finalNum = num;
                    finalBoard = b;
                }
            }
        }
    }

    finalSum = finalNum*finalBoard->sum();

    std::cout << "Final Bingo on " << finalNum << ", value: ";
                    std::cout << finalSum << std::endl;

    for (Board*& b : boards) {
        delete b;
        b = nullptr;
    }
}