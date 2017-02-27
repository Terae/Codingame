#include <iostream>
#include <algorithm>

/**
 * aller le plus rapidement possible à la bonne coordonnée (N,S/E,W)
 */

using namespace std;

int main()
{
    int LX; // position X initiale de l'éclair
    int LY; // position Y initiale de l'élair
    int TX; // position X de Thor
    int TY; // position Y de Thor
    
    cin >> LX >> LY >> TX >> TY; cin.ignore();

    while (1) {
        int remainingTurns;
        cin >> remainingTurns; cin.ignore();

        string str;
        if (TY>LY){ // trop bas
            str+='N';
            TY--;}
        if (LY>TY){ // trop haut
            str+='S';
            TY++;}
        if(TX>LX){ // trop à droite
            str+='W';
            TX--;}
        if(LX>TX){ // trop à gauche
            str+='E';
            TX++;}

        cout << str << endl;
    }
}
