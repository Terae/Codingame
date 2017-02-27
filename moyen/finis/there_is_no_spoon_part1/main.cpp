#include <iostream>
#include <algorithm>

/**
 * Trouver les voisins de droite et d'en bas dans un tableau 2D
 */

using namespace std;

int main() {
    int width; // the number of cells on the X axis
    cin >> width; cin.ignore();
    int height; // the number of cells on the Y axis
    cin >> height; cin.ignore();
    
    bool cellules[width][height];
    for(int i = 0 ; i < width ; i++)
        for(int j = 0 ; j < height ; j++)
            cellules[i][j] = false;
    
    for(int y = 0 ; y < height ; y++) {
        string line; // width characters, each either 0 or .
        getline(cin, line);
        for(int x = 0 ; x < width ; x++)
            cellules[x][y] = line[x] == '0';
    }
    
    for(int y = 0 ; y < height ; y++) {
        for(int x = 0 ; x < width ; x++) {
            // Il y a un noeud : on cherche ses voisins
            if(cellules[x][y]) {
                // Voisin de droite
                int xDroit = -1;
                int yDroit = -1;
                for(int aux = x + 1 ; aux < width ; aux++) {
                    if(cellules[aux][y]) {
                        xDroit = aux;
                        yDroit = y;
                        break;
                    }
                }
                
                // Voisin du bas
                int xBas = -1;
                int yBas = -1;
                for(int aux = y + 1 ; aux < height ; aux++) {
                    if(cellules[x][aux]) {
                        xBas = x;
                        yBas = aux;
                        break;
                    }
                }
                
                // On envoie les données
                cout << x << ' ' << y << ' ' << xDroit << ' ' << yDroit << ' ' << xBas << ' ' << yBas << endl;
            }
        }
    }
}
