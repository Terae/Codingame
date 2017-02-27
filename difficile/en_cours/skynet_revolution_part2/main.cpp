#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();
    
    bool matrix[N][N];
    for(int i = 0 ; i < N ; i++) {
        for(int j = 0 ; j < N ; j++) {
            matrix[i][j] = false;
        }
    }
    // M(1,2) = M(2,1) = true si le lien entre les points 1 et 2 existe

    for (int i = 0; i < L; i++) {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();
        matrix[N1, N2][N2, N1] = true;
        matrix[N2, N1][N1, N2] = true;
    }
    
    vector<int> passerelles;
    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
        passerelles.push_back(EI);
    }
    
    
    for(int i = 0 ; i < N ; i++) {
        cerr << i << " : ";
        for(int j = 0 ; j < N ; j++)
            cerr << matrix[i][j] << ' ';
        cerr << endl;
    }

    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();
        
        int passerelleMenacee;
        int point;
        
        int nbrPointsAdjacentsMax = 0;
        for(int i = 0 ; i < passerelles.size() ; i++) {
            // Il existe un arc direct entre une passerelle et le virus
            if(matrix[SI][passerelles[i]]) {
                //cerr << "Il y a un arc direct entre l'agent et une passerelle" << endl;
                passerelleMenacee = passerelles[i];
                point = SI;
                break;
            }
            
            // On compte le maximum de points adjacents pour les passerelles pour supprimer un arc de la passerelle la plus découverte
            int nbrPointsAdjacents = 0;
            int pointAdjacent = -1;
            for(int y = 0 ; y < N ; y++) {
                //cerr << "matrice en " << passerelles[i] << ";" << y << " : " << matrix[y][passerelles[i]] << endl;
                if(matrix[y][passerelles[i]]) {
                    nbrPointsAdjacents++;
                    if(pointAdjacent == -1)
                        pointAdjacent = y;
                }
            }
            //cerr << "La passerelle " << passerelles[i] << " a " << nbrPointsAdjacents << " points adjacents (dont le numéro " << pointAdjacent << ")" << endl;
            
            if(nbrPointsAdjacents > nbrPointsAdjacentsMax) {
                nbrPointsAdjacentsMax = nbrPointsAdjacents;
                passerelleMenacee = passerelles[i];
                point = pointAdjacent;
            }
        }
        
        // On mets à jour le graphe (suppression d'un arc)
        matrix[passerelleMenacee][point] = false;
        matrix[point][passerelleMenacee] = false;

        cout << passerelleMenacee << ' ' << point << endl;
    }
}
