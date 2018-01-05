#include <iostream>
#include <string>
#include <limits>

using namespace std;

int main() {
    int N;
    cin >> N; cin.ignore();
    std::cerr << "Il y a " << N << " maisons.\n" << std::endl;
    int positions[N][2];

    int xMin = std::numeric_limits<int>::max();
    int xMax = std::numeric_limits<int>::min();
    int yMin = std::numeric_limits<int>::max();
    int yMax = std::numeric_limits<int>::min();

    for (int i = 0 ; i < N ; ++i) {
        int X;
        int Y;
        cin >> X >> Y; cin.ignore();
        positions[i][0] = X;
        positions[i][1] = Y;

        if(X > xMax)
            xMax = X;
        if(Y > yMax)
            yMax = Y;
        if(X < xMin)
            xMin = X;
        if(Y < yMin)
            yMin = Y;
        //std::cerr << X << ' ' << Y << std::endl;
    }
    //std::cerr << std::endl;

    std::cerr << "xMin = " << xMin << " ; xMax = " << xMax
              << "\nyMin = " << yMin << " ; yMax = " << yMax << '\n' << std::endl;

    int distYMin = std::numeric_limits<int>::max();
    while(yMax - yMin > 1) {
        int newY = (yMax - yMin) / 2;

        int distY = 0;
        for(int i = 0 ; i < N ; ++i) {
            distY += abs(positions[i][1] - newY);
            if(distY > distYMin)
                break;
        }
        if(distY < distYMin) {
            distYMin = distY;
            yMin += (yMin + newY) / 2;
        }
    }
    /*for(int y = yMin ; y <= yMax ; y++) {
        int distY = 0;
        for(int i = 0 ; i < N ; ++i) {
            distY += abs(positions[i][1] - y);
            if(distY > distYMin)
                break;
        }
        if(distY < distYMin)
            distYMin = distY;
    }*/

    cout << xMax - xMin + distYMin << endl;
}