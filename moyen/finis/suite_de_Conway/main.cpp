#include <iostream>
#include <vector>
#include <algorithm>

/**
 * génération de la suite de Conway
 *      1
 *     1 1
 *     2 1
 *   1 2 1 1
 * 1 1 1 2 2 1
 * 3 1 2 2 1 1
 *     ...
 */

using namespace std;

void displayListe(vector<int> list) {
    for(int nbr : list)
        cerr << nbr << ' ';
    cerr << endl;
}

int main() {
    int R;
    cin >> R; cin.ignore();
    int L;
    cin >> L; cin.ignore();
    
    cerr << "Init : R = " << R << endl;
    cerr << "Line : L = " << L << '\n' << endl;

    vector<int> list;
    list.push_back(R);
    for(int l = 0 ; l < L - 1 ; l++) {
        vector<int> newList;

        int count;
        for(int i = 0 ; i < list.size() ; i += count) {
            int nbr = list[i];
            count = 1;
            for(int j = i + 1 ; j < list.size() ; j++) {
                if(nbr == list[j])
                    count++;
                else
                    break;
            }
            newList.push_back(count);
            newList.push_back(nbr);
        }
        list = newList;
        displayListe(list);
    }
    
    string result;
    for(int nbr : list) {
        result += std::to_string(nbr);
        result += ' ';
    }
    result.pop_back();
    
    cout << result << endl;
}
