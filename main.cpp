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
    int numbers[5][5] = {};
    bool matched[5][5] = {};
    bool isBingo = false;
    void print()
    {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
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

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (!matched[i][j] && val == numbers[i][j]) {
                    matched[i][j] = true;
                }
                colfound = matched[0][j] && matched[1][j] && matched[2][j] && matched[3][j] && matched[4][j];
                if (colfound)
                    break;
            }
            rowfound = matched[i][0] && matched[i][1] && matched[i][2] && matched[i][3] && matched[i][4]; 
            if (rowfound)
                break;
        }

        isBingo = rowfound || colfound;

        return isBingo;
    }

    int sum()
    {
        int total = 0;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (!matched[i][j]) {
                    total += numbers[i][j];
                }
            }
        }

        return total;
    }
};


std::vector<int> parseCSV(std::string line)
{
    std::istringstream in(line);
    std::vector<int> nums;
    int num = 0;
    char c;
    while (in >> num) {
        nums.push_back(num);
        in >> c;
        std::cout << num << ", ";
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
        for (int cnt = 0; cnt < 5; cnt++) {
            row >> b->numbers[i - 1 - start][cnt];
        }
        //std::cout << std::endl;
    }

    //b->print();
    return b;
}

int main()
{
    std::ifstream file("4/data.txt");

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

    std::cout << splits[0] << " "  << splits[1] << " " << splits[2] 
            << " :: " << splits.size() << std::endl << std::endl;

    for (auto s = 0u; s < splits.size() - 1; s++) {
        boards.push_back(parseBoard(lines, splits[s], splits[s+1]));
    }
    //end board
    boards.push_back(parseBoard(lines, splits[splits.size() - 1], lines.size()));

    bool bingoCalled = false;
    for (int num : drawnNums) {
        for (Board* b : boards) {
            if (b->isBingo) continue;
            bool bingo = b->check(num);
            if (bingo) {
                std::cout << "Bingo on " << num << ", value: ";
                std::cout << num*b->sum() << std::endl;
                bingoCalled = true;
            }
        }

        //if (bingoCalled) break;
    }

    for (Board*& b : boards) {
        delete b;
        b = nullptr;
    }
}