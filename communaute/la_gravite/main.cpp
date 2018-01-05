#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
int main() {
    int width;
    int height;
    cin >> width >> height; cin.ignore();
    vector<vector<char>> colonnes;
    colonnes.reserve(width);
    for (int i = 0; i < height; i++) {
        string line;
        cin >> line; cin.ignore();
        for(int j = 0 ; j < line.size() ; j++) {
            colonnes[j].push_back(line[j]);
        }
    }
    for(int i = 0 ; i < width ; i++) {
        sort(begin(colonnes[i]), end(colonnes[i]));
    }
    for(int i = height - 1 ; i >= 0 ; i--) {
        for(int j = 0 ; j < width ; j++) {
            cout << colonnes[j][i];
        }cout << endl;
    }
}