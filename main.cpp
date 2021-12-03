#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <string>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <algorithm>

static const int BITSIZE = 12;

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

struct Field 
{
    bool val[BITSIZE] = {0};
    void print() 
    {
        for (bool b : val) {
            std::cout << b;
        }
        std::cout << std::endl;
    }
};


Field parseLine(const char* line)
{
    Field result;

    for (int i = 0; i < BITSIZE; i++) {
        result.val[i] = (line[i] == '1');//? 1 : -1;
    }

    return result;
}


int main()
{
    std::vector<std::string> list = readFileList("3/data.txt");
    std::vector<Field> fields;
    std::vector<Field> res;
    std::vector<Field> res2;
    std::vector<Field> temp;
    int sumfield = 0;
    int sumfield2 = 0;
    int co2 = 0;
    int o2 = 0;

    for (std::string line : list) {
        fields.push_back(parseLine(line.c_str()));
    }

    res = fields;
    res2 = fields;

    for (int i = 0; i < BITSIZE; i++) {
        for (const Field& f : res) {
            sumfield += f.val[i]? 1 : -1; 
        }
        for (const Field& f : res2) {
            sumfield2 += f.val[i]? 1 : -1; 
        }
        if (sumfield >= 0) {
            std::cout << "1 more or equal : o2\n";
            if (res.size() > 1) {
                temp = res;
                res.clear();
                std::copy_if(temp.begin(), temp.end(), std::back_inserter(res), 
                                [&](const Field& f) { 
                                    return f.val[i];
                                }
                );
            }
        } else {
            std::cout << "0 more : o2\n";
            if (res.size() > 1) {
                temp = res;
                res.clear();
                std::copy_if(temp.begin(), temp.end(), std::back_inserter(res), 
                                [&](const Field& f) { 
                                    return !f.val[i];
                                }
                );
            }
        }

        if (sumfield2 >= 0) {
            std::cout << "0 less or equal : co2\n";
            if (res2.size() > 1) {
                temp = res2;
                res2.clear();
                std::copy_if(temp.begin(), temp.end(), std::back_inserter(res2), 
                                [&](const Field& f) { 
                                    return !f.val[i];
                                }
                );
            }
        } else {
            std::cout << "1 less : co2\n";
            if (res2.size() > 1) {
                temp = res2;
                res2.clear();
                std::copy_if(temp.begin(), temp.end(), std::back_inserter(res2), 
                                [&](const Field& f) { 
                                    return f.val[i];
                                }
                );
            }
        }
        std::cout << res.size() << " " << res2.size() << std::endl;
        sumfield = 0;
        sumfield2 = 0;
    }

    res[0].print();
    res2[0].print();

    for (int i = BITSIZE - 1; i >= 0; i--) {
        if (res[0].val[i]) {
            o2 += (1 << (BITSIZE - i - 1));
        }
        if (res2[0].val[i]) {
            co2 += (1 << (BITSIZE - i - 1));
        }
    }

    std::cout << o2*co2;
}

// int main()
// {
//     std::vector<std::string> list = readFileList("3/data.txt");
//     Field sum;
//     int gamma = 0;
//     int epi = 0;

//     for (std::string line : list) {
//         Field res = parseLine(line.c_str());
//         for (int i = 0; i < 12; i++) {
//             sum.val[i] += res.val[i];
//         }
//     }
    
//     for (int i = 11; i >= 0; i--) {
//         if (sum.val[i] >= 0) {
//             gamma += (1 << (11-i));
//         } else {
//             epi += (1 << (11-i));
//         }
//     }

//     std::cout << gamma*epi;
// }