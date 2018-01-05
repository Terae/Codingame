#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <list>

using namespace std;

map<char, int> points = {{'a', 1},
                         {'b', 3},
                         {'c', 3},
                         {'d', 2},
                         {'e', 1},
                         {'f', 4},
                         {'g', 2},
                         {'h', 4},
                         {'i', 1},
                         {'j', 8},
                         {'k', 5},
                         {'l', 1},
                         {'m', 3},
                         {'n', 1},
                         {'o', 1},
                         {'p', 3},
                         {'q', 10},
                         {'r', 1},
                         {'s', 1},
                         {'t', 1},
                         {'u', 1},
                         {'v', 4},
                         {'w', 4},
                         {'x', 8},
                         {'y', 4},
                         {'z', 10}
};

int value(string s) {
    int i{0};
    for_each(s.cbegin(), s.cend(), [&](char c) { i += points[c]; });
    return i;
}

struct sort_words {
    bool operator() (const string& s1, const string& s2) const {
        return value(s1) < value(s2);
    }
};

int main()
{
    list<string> l;
    int N;
    cin >> N; cin.ignore();
    for (int i = 0; i < N; i++) {
        string W;
        getline(cin, W);
        l.emplace_back(W);
    }
    
    string LETTERS;
    getline(cin, LETTERS);
    
    string better_word;
    int max_points{0};
    for (list<string>::iterator it{l.begin()}; it != l.cend(); ++it) {
        bool possible{true};
        string copy{LETTERS};
        
        for (char c : *it) {
            size_t pos;
            if ((pos = copy.find_first_of(c)) != string::npos)
                copy.erase(next(copy.begin(), pos));
            else {
                possible = false;
                break;
            }
        }
        if (possible) {
            int i{value(*it)};
            if (i > max_points) {
                max_points = i;
                better_word = *it;
            }
        }
    }
    
    cout << better_word << endl;
}
