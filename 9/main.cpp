#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cmath>

#define NPOS std::string::npos

template<typename T>
void map(std::vector<T>& vec, std::function<bool(const T&)> f)
{
    for (auto it = vec.begin(); it != vec.end();) {
        if (!f(*it)) {
            vec.erase();
        } else {
            ++it;
        }
    }
}

template<typename T>
T& max(T& a, T& b)
{
    if (a >= b)
        return a;
    else
        return b;
}

template<typename T>
T min(T a, T b)
{
    if (a <= b)
        return a;
    else
        return b;
}

static int max_day = 8;

std::vector<int*> readL(std::ifstream& f, size_t& len)
{
    std::vector<int*> data;
    int* nline;
    std::string line;

    std::getline(f, line);
    size_t s = line.size();
    nline = new int[s];
    for (auto i = 0u; i < s; i++) {
        if (line[i] != '\0')
            nline[i] = line[i] - '0';
    }
    data.push_back(nline);
    //std::cout << line << std:: endl;
    
    while(std::getline(f, line)) {
        //std::cout << line << std:: endl;
        nline = new int[s];
        for (auto i = 0u; i < s; i++){
            if (line[i] != '\0')
                nline[i] = line[i] - '0';
        }
        data.push_back(nline);
    }

    len = s;
    return data;
}

struct Basin
{
    size_t i;
    size_t j;
    int val;
};

size_t length = 0;

void calcBasin(const std::vector<int*>& data, size_t i, size_t j, std::vector<std::pair<int,int>>& reached)
{
    //int val = 1;
    if (data[i][j] == 9)
        return;

    reached.push_back(std::pair<int, int>{i, j});

    if (i > 0 && data[i][j] <= data[i-1][j] && 
                std::none_of(reached.begin(), reached.end(), [&](auto& p){ return p == std::pair<int, int>{i-1, j}; })) {
        calcBasin(data, i-1, j, reached);
    }
    
    if (i < data.size() - 1 && data[i][j] <= data[i+1][j] && 
                std::none_of(reached.begin(), reached.end(), [&](auto& p){ return p == std::pair<int, int>{i+1, j}; })) {
        calcBasin(data, i+1, j, reached);
    }

    if (j > 0 && data[i][j] <= data[i][j-1] && 
                std::none_of(reached.begin(), reached.end(), [&](auto& p){ return p == std::pair<int, int>{i, j-1}; })) {
        calcBasin(data, i, j-1, reached);
    }

    if (j < length - 1 && data[i][j] <= data[i][j+1] &&
                std::none_of(reached.begin(), reached.end(), [&](auto& p){ return p == std::pair<int, int>{i, j+1}; })) {
        calcBasin(data, i, j+1, reached);
    }

    //return val;
}

size_t findBasin(std::vector<int*>& data, size_t i, size_t j)
{
    std::vector<std::pair<int,int>> reached;
    calcBasin(data, i, j, reached);
    return reached.size();
}

int main()
{
    std::ifstream file("9/big.txt");
    size_t len;
    std::vector<int*> data = readL(file, len);
    length = len;
    std::vector<Basin> basins;
    bool lowest = true;
    int risk = 0;

    for (auto i = 0u; i < data.size(); i++) {
        for (auto j = 0u; j < len; j++) {
            lowest = true;
            if (data[i][j] == 9)
                lowest = false;

            if (lowest && i > 0) {
                lowest = !(data[i][j] >= data[i-1][j]);
            }
            
            if (lowest && i < data.size() - 1) {
                lowest = !(data[i][j] >= data[i+1][j]);
            }

            if (lowest && j > 0) {
                lowest = !(data[i][j] >= data[i][j-1]);
            }

            if (lowest && j < len - 1) {
                lowest = !(data[i][j] >= data[i][j+1]);
            }

            if (lowest) {
                risk += 1 + data[i][j];
                Basin b;
                b.i = i;
                b.j = j;
                b.val = data[i][j];
                basins.push_back(b);
            }

            //std::cout << data[i][j];
        }
        //std::cout << std::endl;
    }

    std::cout << risk << std::endl << std::endl;

    std::sort(basins.begin(), basins.end(), [](Basin& a, Basin& b){ return a.val < b.val; });
    std::vector<size_t> basinsizes;

    for (Basin& b : basins) {
        //std::cout << "finding on basin " << b.val << std::endl;
        size_t val = findBasin(data, b.i, b.j);
        if (basinsizes.size() < 3) {
            basinsizes.push_back(val);
            if (basinsizes.size() == 3) {
                std::sort(basinsizes.begin(), basinsizes.end(), [](size_t a, size_t b){ return a < b; });
            }
        } else {
            if (val > basinsizes[0] && val < basinsizes[1])
                basinsizes[0] = val;
            else if (val >= basinsizes[1]) {
                basinsizes[0] = val;
                std::sort(basinsizes.begin(), basinsizes.end(), [](size_t a, size_t b){ return a < b; });
            }
        }
    }

    std::sort(basinsizes.begin(), basinsizes.end(), [](size_t a, size_t b){ return a > b; });

    int count = 0;
    size_t product = 1;
    for (auto bs : basinsizes) {
        product *= bs;
        count++;
        if (count == 3) break;
    }

    std::cout << product;

    return 0;
}