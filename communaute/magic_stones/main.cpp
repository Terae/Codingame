#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void displayVector(vector<int> levels) {
    for(int a : levels)
        cerr << a << endl;
    cerr << endl;
}

int main()
{
    int N;
    cin >> N; cin.ignore();
    vector<int> levels;
    vector<int>::iterator it;
    for (int i = 0; i < N; i++) {
        int level;
        cin >> level; cin.ignore();
        levels.push_back(level);
    }
    
    sort(levels.begin(), levels.end());
    
    bool changement;
    while(!changement) {
        changement = false;
        vector<int> newLevels;
        for(int i = 0 ; i < N - 1 ; i++) {
            if(levels[i] != levels[i + 1]) { // unicité
                newLevels.push_back(levels[i]);
            } else {
                newLevels.push_back(levels[i] + 1); // on sauvegarde l'upgrade des 2 nombres identiques
                i++;
                
                changement = true;
            }
        }
        if(!changement)
            break;
        if(levels[N - 2] != levels[N - 1])
            newLevels.push_back(levels[N - 1]);
        
        sort(newLevels.begin(), newLevels.end());
        levels.clear();
        levels = newLevels;
    }
    
    
    it = unique(levels.begin(), levels.end());
    levels.resize(distance(levels.begin(),it));

    cout << levels.size() << endl;
}
