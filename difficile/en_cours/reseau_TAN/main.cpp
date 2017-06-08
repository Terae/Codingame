#include <iostream>
#include <string>
#include <deque>
#include <algorithm>

using namespace std;

int main()
{
    int L; // nombre de places dans le manège
    int C; // nombre de fois que le manège peut fonctionner
    int N; // nombre de groupes
    cin >> L >> C >> N; cin.ignore();

    int groupes[N];
    for (int i = 0; i < N; i++) {
        int Pi;
        cin >> Pi; cin.ignore();
        groupes[i] = Pi;
    }
    
    int ind = 0;
    
    unsigned long int dirhams = 0;
    
    for(int i = 0 ; i < C ; i++) {
        bool manegePlein = false;
        int placesRestantes = L;
        
        int nbrGroupes = 0;
        while(!manegePlein) {
            if(nbrGroupes == N)
                break;
                
            int nbr = groupes[ind];
            if(nbr <= placesRestantes) {
                placesRestantes -= nbr;
                ind = (ind + 1) % N;
                nbrGroupes++;
            } else
                manegePlein = true;
        }
        dirhams += L - placesRestantes;
    }

    cout << dirhams << endl;
}
