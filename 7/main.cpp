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

template<typename T>
T min(T a, T b)
{
    if (a <= b)
        return a;
    else
        return b;
}

static int max_day = 8;

std::vector<int> parse(std::ifstream& f)
{
    char c;
    int n;
    std::vector<int> crab_pos;

    do {
        f >> n;
        crab_pos.push_back(n);
    } while (f >> c);

    return crab_pos;
}

std::vector<int> closestTomedianOfArray(std::vector<int>& nums)
{
    double median = 0;
    int closest = -1;
    std::vector<int> candidates;

    if (nums.size() == 1) {
        candidates.push_back(nums[0]);
    } else if (nums.size()%2 == 0 && nums.size() > 0) {
        size_t hi = nums.size() - 1;
        size_t mid = hi/2;
        candidates.push_back(nums[mid]);
        candidates.push_back(nums[mid+1]);
    } else if (nums.size()%2 == 1 && nums.size() > 0) {
        candidates.push_back(nums[(nums.size()-1)/2]);
    }

    return candidates;
}

int findCandidate(std::vector<int>& nums)
{
    int idx = 0;
    int mean = 0;

    if (nums.size() == 1) {
        idx = 0;
    } else if (nums.size()%2 == 0 && nums.size() > 0) {
        size_t hi = nums.size() - 1;
        size_t mid = hi/2;
        idx = mid;
    } else if (nums.size()%2 == 1 && nums.size() > 0) {
        idx = (nums.size()-1)/2;
    }

    return idx;
}


int findMean(std::vector<int>& nums)
{
    int sum = 0;

    for (int n: nums) {
        sum += n;
    }

    double mean = sum/(double)nums.size();

    if (mean - (int)mean > 0.5)
        return (int)mean;
    else
        return (int)mean + 1;
}


int calc(int cand, std::vector<int> crabs)
{
    int fuel = 0;
    for (int c : crabs) {
        int n = std::abs(cand - c);
        fuel += n*(n+1)/2;
    }

    return fuel;
}


int main()
{
    std::ifstream file("7/data.txt");
    std::vector<int> crab_pos = parse(file);
    std::sort(crab_pos.begin(), crab_pos.end());
    
    std::vector<int> cand = closestTomedianOfArray(crab_pos);
    int candIdx = findCandidate(crab_pos);
    int oldIdx = candIdx;

    int res = 0;
    int res2 = INT32_MAX;
    if (cand.size() != 1)
        res = min(calc(cand[0], crab_pos), calc(cand[1], crab_pos));
    else
        res = calc(cand[0], crab_pos);

    std::cout << res << std::endl;

    int fuel_prev = INT32_MAX;
    int fuel_curr = calc(findMean(crab_pos), crab_pos);
    int fuel_med = fuel_curr;
    int step1 = 1, step2 = 1;

    while (fuel_prev > fuel_curr) {
        fuel_prev = fuel_curr;
        fuel_curr = calc(crab_pos[candIdx] + step1, crab_pos);
        step1++;
    }

    res2 = fuel_prev;

    while (fuel_prev > fuel_curr) {
        fuel_prev = fuel_curr;
        fuel_curr = calc(crab_pos[candIdx] - step2, crab_pos);
        step2++;
    }

    std::cout << min(fuel_prev, res2) << ", " << fuel_med;

    return 0;
}