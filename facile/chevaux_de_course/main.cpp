#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Trouver les deux nombres les plus proches dans une liste
 */

using namespace std;

int main()
{
    int N;
    cin >> N; cin.ignore();
    
    // on récupère toutes les valeurs de puissance
    std::vector<int> tab;
    int Pi;
    for (int i = 0; i < N; i++) {
        cin >> Pi; cin.ignore();
        tab.push_back(Pi);
    }
    
    // on les trie dans l'ordre croissant
    sort(tab.begin(), tab.begin() + N);
    
    int D = numeric_limits<int>::max();
    int temp;
    // on regarde si deux valeurs consécutives sont plus faibles que le minimum
    for (int i = 0 ; i < N-1 ; i++) {
        temp = abs(tab[i] - tab[i+1]);
        if (temp < D) {
            D = temp;
        }        
    }
    
    // on envoie le résultat
    cout << D << endl;
}
