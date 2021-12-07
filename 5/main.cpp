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

struct Line
{
    int x1, y1;
    int x2, y2;
    bool horizontal() const
    {
        return y1 == y2;
    }
    bool vertical() const
    {
        return x1 == x2;
    }
    void reorder()
    {
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
    }
};

static auto h = std::hash<std::string>{};


std::string toPointStr(int x, int y)
{
    return std::to_string(x) + "," + std::to_string(y);
}

Line* parse(const std::string& line)
{
    std::istringstream in(line);
    Line* l = new Line();
    char c;
    
    in >> l->x1;
    in >> c;
    in >> l->y1;
    in >> c;
    in >> c;
    in >> l->x2;
    in >> c;
    in >> l->y2;

    l->reorder();

    return l;
}


int main2()
{
    std::ifstream file("5/big.txt");
    std::string line;
    std::vector<Line*> lines;
    std::vector<Line*> hlines;
    std::vector<Line*> vlines;
    std::vector<Line*> dlines;
    std::unordered_map<std::string, int> grid;
    int count = 0;
    int count1 = 0;

    while (std::getline(file, line)) {
        lines.push_back(parse(line));
    }

    std::copy_if(lines.begin(), lines.end(), std::back_inserter(hlines), [](const Line* l){ return l->horizontal(); });
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(vlines), [](const Line* l){ return l->vertical(); });
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(dlines), [](const Line* l){ return !l->vertical() && !l->horizontal(); });

    auto insertPoint = [&](std::string&& p) {
        if (grid.find(p) != grid.end()) {
            grid[p] += 1;
        } else {
            grid[p] = 1;
        }
    };

    for (const Line* hl : hlines) {
        for (int x = hl->x1; x <= hl->x2; x++) {
            insertPoint(toPointStr(x,hl->y1));
        }
    }

    for (const Line* vl : vlines) {
        if (vl->y1 <= vl->y2) {
            for (int y = vl->y1; y <= vl->y2; y++) {
                insertPoint(toPointStr(vl->x1, y));
            }
        } else {
            for (int y = vl->y2; y <= vl->y1; y++) {
                insertPoint(toPointStr(vl->x1, y));
            }
        }
    }

    for (const Line* dl : dlines) {
        if (dl->x1 <= dl->x2 && dl->y1 <= dl->y2) {
            for (int s = 0; dl->x1+s <= dl->x2; s++) {
                insertPoint(toPointStr(dl->x1+s, dl->y1+s));
            }
        } else if (dl->x1 <= dl->x2 && dl->y1 >= dl->y2) {
            for (int s = 0; dl->x1+s <= dl->x2; s++) {
                insertPoint(toPointStr(dl->x1+s, dl->y1-s));
            }
        }
    }

    for (auto it = grid.begin(); it != grid.end(); ++it) {
        if (it->second > 1)
            count++;
    }

    std::cout << count;

    for (Line* l : lines) {
        delete l;
    }

    return 0;
}


int main()
{
    std::ifstream file("5/big2.txt");
    std::string line;
    std::vector<Line*> lines;
    std::vector<Line*> hlines;
    std::vector<Line*> vlines;
    std::vector<Line*> dlines;
    int xmax = 0;
    int ymax = 0;

    while (std::getline(file, line)) {
        lines.push_back(parse(line));
        xmax = max(xmax, max(lines.back()->x1, lines.back()->x2));
        ymax = max(ymax, max(lines.back()->y1, lines.back()->y2));
    }

    int** grid = new int*[xmax+1];

    for (int i = 0; i <= xmax; i++) {
        grid[i] = new int[ymax+1];
        for (int j = 0; j <= ymax; j++) {
            grid[i][j] = 0;
        }
    }

    std::copy_if(lines.begin(), lines.end(), std::back_inserter(hlines), [](const Line* l){ return l->horizontal(); });
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(vlines), [](const Line* l){ return l->vertical(); });
    std::copy_if(lines.begin(), lines.end(), std::back_inserter(dlines), [](const Line* l){ return !l->vertical() && !l->horizontal(); });

    for (const Line* hl : hlines) {
        for (int x = hl->x1; x <= hl->x2; x++) {
                grid[x][hl->y1] += 1;
        }
    }

    for (const Line* vl : vlines) {
        if (vl->y1 <= vl->y2) {
            for (int y = vl->y1; y <= vl->y2; y++) {
                grid[vl->x1][y] += 1;
            }
        } else {
            for (int y = vl->y2; y <= vl->y1; y++) {
                grid[vl->x1][y] += 1;
            }
        }
    }

    int count = 0;

    for (int i = 0; i <= xmax; i++) {
        for (int j = 0; j <= ymax; j++) {
            if (grid[i][j] > 1)
                count++;
        }
    }

    std::cout << count << std::endl;

    for (const Line* dl : dlines) {
        if (dl->x1 <= dl->x2 && dl->y1 <= dl->y2) {
            for (int s = 0; dl->x1+s <= dl->x2; s++) {
                grid[dl->x1+s][dl->y1+s] += 1;
            }
        } else if (dl->x1 <= dl->x2 && dl->y1 >= dl->y2) {
            for (int s = 0; dl->x1+s <= dl->x2; s++) {
                grid[dl->x1+s][dl->y1-s] += 1;
            }
        }
    }

    count = 0;

    for (int i = 0; i <= xmax; i++) {
        for (int j = 0; j <= ymax; j++) {
            if (grid[i][j] > 1)
                count++;
        }
    }

    std::cout << count;

    for (Line* l : lines) {
        delete l;
    }

    for (int i = 0; i <= xmax; i++) {
        delete[] grid[i];
    }

    delete[] grid;

    return 0;
}