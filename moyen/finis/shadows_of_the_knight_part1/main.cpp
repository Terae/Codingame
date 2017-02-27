#include <iostream>
#include <vector>
#include <algorithm>

/**
 * Batman cherche une fenêtre dans un tableau 2D pour trouver une bombe (détecteur donnant sa direction)
 */

using namespace std;
 
enum dir {
     U = 0,
     UR,
     R,
     DR,
     D,
     DL,
     L,
     UL
};
  
 dir getDir(string bombDir) {
     if(bombDir == "U")  return U;
     if(bombDir == "UR") return UR;
     if(bombDir == "R")  return R;
     if(bombDir == "DR") return DR;
     if(bombDir == "D")  return D;
     if(bombDir == "DL") return DL;
     if(bombDir == "L")  return L;
     if(bombDir == "UL") return UL;
 }

int main()
{
    int W; // width of the building.
    int H; // height of the building.
    cin >> W >> H; cin.ignore();
    int N; // maximum number of turns before game over.
    cin >> N; cin.ignore();
    int X0;
    int Y0;
    cin >> X0 >> Y0; cin.ignore();
    
    int XMin = 0;
    int YMin = 0;
    int XMax = W;
    int YMax = H;

    // game loop
    while (1) {
        string bombDir; // the direction of the bombs from batman's current location (U, UR, R, DR, D, DL, L or UL)
        cin >> bombDir; cin.ignore();
        dir newDir = getDir(bombDir);
        
        switch(newDir) {
            case U :
                YMax = Y0;
                break;
            case UR:
                XMin = X0;
                YMax = Y0;
                break;
            case R :
                XMin = X0;
                break;
            case DR:
                XMin = X0;
                YMin = Y0;
                break;
            case D :
                YMin = Y0;
                break;
            case DL:
                XMax = X0;
                YMin = Y0;
                break;
            case L :
                XMax = X0;
                break;
            case UL:
                XMax = X0;
                YMax = Y0;
                break;
        }
        
        X0 = (XMax + XMin) / 2;
        Y0 = (YMax + YMin) / 2;

        // the location of the next window Batman should jump to.
        std::cout << X0 << " " << Y0 << std::endl;
    }
}
