#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <cstdlib>
#include <climits>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <sstream>
#include <cmath>

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

template<typename T>
T& max(T& a, T& b)
{
    if (a >= b)
        return a;
    else
        return b;
}

static int max_day = 8;

std::map<int, long long> parse(std::ifstream& f)
{
    char c;
    int n;
    std::map<int, long long> fish;

    do {
        f >> n;
        if (fish.find(n) != fish.end()) {
            fish[n] += 1;
        } else {
            fish.insert({n, 1});
        }
    } while (f >> c);

    return fish;
}


void update(std::map<int,long long>& fish)
{
    long long refreshFish;
    long long newFish;

    for (int i = 0; i <= max_day; i++) {
        if (i == 0) {
            newFish = fish[0];
            refreshFish = fish[0];
        } else {
            fish[i-1] = fish[i];
            fish[i] = 0;
        }
    }

    fish[8] = newFish;
    fish[6] += refreshFish;
}

int main()
{
    std::ifstream file("6/data.txt");
    std::map<int, long long> fish = parse(file);
    long long count = 0;

    for (int d = 0; d < 256; d++) {
        update(fish);
    }

    for (auto& f : fish) {
        count += f.second;
    }

    std::cout << count;

    return 0;
}