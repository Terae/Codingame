#include <iostream>
#include <vector>
#include <algorithm>

/**
 * trouver la montagne la plus haute dans une liste de montagnes
 */

using namespace std;

int main() {
    while (1) {
        vector<int> mountains;
        for (int i = 0 ; i < 8 ; i++) {
            int mountainH; // represents the height of one mountain.
            cin >> mountainH; cin.ignore();
            mountains.push_back(mountainH);
        }

        int n;
        int max = 0;
        for(int i = 0 ; i < 8 ; i++) {
            if(mountains[i] > max) {
                n = i;
                max = mountains[i];
            }
        }
        cout << n << endl;
    }
}