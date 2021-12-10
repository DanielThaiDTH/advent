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

std::vector<std::string> parse(std::ifstream& f)
{
    std::vector<std::string> lines;
    std::string line;
    std::string token;
    size_t pos;
    std::map<size_t, std::string> unique;
    std::vector<std::string> tokens;
    std::vector<size_t> digits;
    char a_key, b_key, c_key, d_key,e_key, f_key, g_key;
    size_t total = 0;
    size_t mag = 1;
    size_t subtotal = 0;
    bool found = false;

    while(std::getline(f, line)) {
        lines.push_back(line);
        pos = line.find_first_of('|');
        std::istringstream outs(line.substr(pos+1));
        std::istringstream ins(line.substr(0, pos-1));

        //std::cout << line.substr(0, pos - 1) << " | " << line.substr(pos+1) << std::endl;

        while (ins >> token) {
            tokens.push_back(token);
            switch(token.size()) {
                case 2:
                    unique[1] = token;
                    break;
                case 4:
                    unique[4] = token;
                    break;
                case 3:
                    unique[7] = token;
                    break;
                case 7:
                    unique[8] = token;
                    break;
                default:
                    break;
            }
        }

        //find a
        for (auto c : unique[7]) {
            if (unique[8].find(c) != NPOS && unique[1].find(c) == std::string::npos) {
                a_key = c;
                break;
            }
        }

        //find c and f
        for (auto c : unique[1]) {
            for (auto t : tokens) {
                if (t.size() == 6) {
                    if (t.find(c) == std::string::npos) {
                        c_key = c;
                        if (unique[1][0] != c) {
                            f_key = unique[1][0];
                        } else {
                            f_key = unique[1][1];
                        }
                        found = true;
                        break;
                    }
                }
            }

            if(found) break;
        }
        found = false;


        //find b and d
        for (auto c : unique[4]) {
            if (c == c_key || c == f_key)
                continue;
            for (auto t : tokens) {
                if (t.size() == 5 && t.find(f_key) == std::string::npos) {
                    if (t.find(c) != std::string::npos) {
                        d_key = c;
                    } else {
                        b_key = c;
                    }
                }
            }
        }


        //find g
        for (auto t : tokens) {
            //Using 5
            if (t.size() == 5 && t.find(b_key) != std::string::npos && 
                                        t.find(d_key) != std::string::npos && 
                                        t.find(f_key) != std::string::npos) {
                for (auto c : t) {
                    if (c != a_key && c != b_key && c != d_key && c != f_key) {
                        g_key = c;
                        found = true;
                        break;
                    }
                }
            }

            if (found) break;
        }
        found = false;

        for (auto t: tokens) {
            //Using 0
            if (t.size() == 5 && t.find(a_key) != std::string::npos && 
                                t.find(c_key) != std::string::npos && 
                                t.find(d_key) != std::string::npos && 
                                t.find(g_key) != std::string::npos &&
                                t.find(f_key) == std::string::npos) {
                for (auto c : t) {
                    if (c != a_key && c != c_key && c != d_key && c != g_key) {
                        e_key = c;
                        found = true;
                        break;
                    }
                }
            } 

            if(found) break;
        }
        found = false;


        while(outs >> token) {
            switch(token.size()) {
                case 2:
                    digits.push_back(1);
                    break;
                case 4:
                    digits.push_back(4);
                    break;
                case 3:
                    digits.push_back(7);
                    break;
                case 5:
                    if (token.find(e_key) != std::string::npos && token.find(g_key) != std::string::npos) {
                        digits.push_back(2);
                    } else if (token.find(c_key) != std::string::npos && token.find(d_key) != std::string::npos) {
                        digits.push_back(3);
                    } else {
                        digits.push_back(5);
                    }
                    break;
                case 6:
                    if (token.find(c_key) != std::string::npos && token.find(e_key) != std::string::npos && token.find(f_key) != std::string::npos) {
                        digits.push_back(0);
                    } else if (token.find(e_key) != std::string::npos && token.find(f_key) != std::string::npos) {
                        digits.push_back(6);
                    } else {
                        digits.push_back(9);
                    }
                    break;
                case 7:
                    digits.push_back(8);
                    break;
                default:
                    break;
            }
        }


        mag = 1;
        subtotal = 0;
        for (int i = 0; i < digits.size(); i++) {
            for (int n = 0; n < digits.size() - i - 1; n++) {
                mag *= 10;
            }

            subtotal += digits[i]*mag;
            mag = 1;
        }
        //std::cout << subtotal << std::endl;
        //std::cout << a_key << b_key << c_key << d_key << e_key << f_key << g_key << std::endl;

        total += subtotal;
        digits.clear();
        tokens.clear();
    }

    std::cout << total << std::endl;

    return lines;
}

int main()
{
    std::ifstream file("8/data.txt");
    parse(file);

    return 0;
}