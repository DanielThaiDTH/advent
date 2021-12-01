#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>

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

enum class ChangeState { DESC, ASC };

int main()
{
    std::vector<const char*> list = readFileList("1/data.txt");
    int old_sum = INT_MAX;
    ChangeState state = ChangeState::DESC;
    int count = 0;

    for (int i = 0; i < list.size() - 2; i++) {
        int depth_sum = std::atoi(list[i]) + std::atoi(list[i+1]) + std::atoi(list[i+2]);

        if (depth_sum > old_sum) {
            state = ChangeState::ASC;
        } else {
            state = ChangeState::DESC;
        }

        if (state == ChangeState::ASC) {
            ++count;
        }

        old_sum = depth_sum;
    }

    std::cout << count;
}