#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * optimiser le prix que paye chacun pour un cadeau
 */

int main()
{
    // Number of Oods
    int N;
    cin >> N; cin.ignore();
    // Present prize
    int Ctot;
    cin >> Ctot; cin.ignore();
    // Budgets
    int B[N];
    
    int tot = 0;
    for (int i = 0; i < N; i++) {
        int b;
        cin >> b; cin.ignore();
        B[i] = b;
        tot += b;
    }
    sort(B, B + N);
    ///////////////////////////////////////////////////////////////
    
    if(tot < Ctot || N == 0)
        cout << "IMPOSSIBLE" << endl;
    else {
        int result[N];
        std::fill_n(result, N, -1);
        bool next = true;
        int nbrOodsLeft = N;
        // On mets de côté tous les Oods qui ne peuvent pas payer la moyenne du prix (on ne garde que les riches)
        int C = Ctot;
        int nbrMax = 0;
        while(next && nbrOodsLeft > 0) {
            next = false;
            int average = C / nbrOodsLeft;
            for(int i = 0 ; i < N ; i++) {
                if(B[i] < average && result[i] == -1) {
                    result[i] = B[i];
                    C -= B[i];
                    nbrOodsLeft--;
                    next = true;
                    nbrMax = i;
                }
            }
        }
        
        // On taxe les riches de la même quantité
        for(int i = 0 ; i < N ; i++) {
            if (result[i] == -1) {
                result[i] = C / nbrOodsLeft;
            }
        }
        
        // On ajuste le prix final en taxant les riches un peu plus
        tot = 0;
        for(int i = 0 ; i < N ; i++) {
            tot += result[i];
        }
        int rest = Ctot - tot;
        int I = N - 1;
        while(rest != 0) {
            if(I == nbrMax)
                I = N - 1;
            result[I] ++;
            I--;
            rest --;
        }
        
        for(int i = 0 ; i < N ; i++)
            cout << result[i] << endl;
    }
}
