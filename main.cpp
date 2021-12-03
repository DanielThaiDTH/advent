#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include <cstdio>

std::vector<const char*> readFileList(std::string filename)
{
	std::ifstream listfile(filename);

    if (!listfile)
        std::cout << "Could not read file\n";

	std::string line;
	std::vector<const char*> lines;

	while (std::getline(listfile, line)) {
		char* copy = new char[line.size() + 1];
		std::strcpy(copy, line.c_str());
		lines.push_back(copy);
	}

	return lines;
}

enum DIRECTION { FOR, UP, DOWN };

struct DirCh {
    DIRECTION dir;
    int mag;
};

DirCh parseLine(const char* line)
{
    char direction[10] = { '\0' };
    int mag;
    DirCh result;

    std::sscanf(line, "%s %d", direction, &mag);

    if (strcmp(direction, "forward") == 0) {
        result.dir = FOR;
    } else if (strcmp(direction, "down") == 0) {
        result.dir = DOWN;
    } else if (strcmp(direction, "up") == 0) {
        result.dir = UP;
    }

    result.mag = mag;

    return result;
}

int main()
{
    std::vector<const char*> list = readFileList("2/data.txt");
    int count;
    int depth = 0;
    int aim = 0;
    int dist = 0;

    for (const char* line : list) {
        DirCh ch = parseLine(line);

        if (ch.dir == FOR) {
            dist += ch.mag;
            depth += aim*ch.mag;
        } else if (ch.dir == DOWN) {
            aim += ch.mag;
        } else {
            aim -= ch.mag;
        }
    }
    
    count = depth*dist;

    std::cout << count;
}