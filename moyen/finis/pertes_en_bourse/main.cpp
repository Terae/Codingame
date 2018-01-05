#include <iostream>
#include <vector>
#include <algorithm>

/**
 * trouver la perte maximale en bourse
 */

using namespace std;

int main()
{
    int n;
    cin >> n; cin.ignore();

    vector<int> valeurs(n);
    for (int i = 0; i < n; i++) {
        int v;
        cin >> v; cin.ignore();
        valeurs[i] = v;
        cerr << v << endl;
    }cerr << endl;
     
    int perte_max = 0;
    for(int i = 0 ; i < n ; i++) {
        int a = valeurs[i];
        // on compare par rapport aux suivants ssi la perte peut devenir plus grande que la précédente (croissant)
        if(a > -perte_max) {
            if(i == 0 || a > valeurs[i - 1]) {
               for(int j = i+1; j < n ; j++) {
                   int b = valeurs[j];
                   if(b - a < perte_max) {
                       perte_max = b - a;
                   }
               }
           }
       }
    }
    cout << perte_max << endl;
}
