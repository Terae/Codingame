#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int TX;
    int TY;
    cin >> TX >> TY; cin.ignore();

    // game loop
    while (1) {
        int H; // the remaining number of hammer strikes.
        int N; // the number of giants which are still present on the map.
        cin >> H >> N; cin.ignore();
        for (int i = 0; i < N; i++) {
            int X;
            int Y;
            cin >> X >> Y; cin.ignore();
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // The movement or action to be carried out: WAIT STRIKE N NE E SE S SW W or N
        cout << "WAIT" << endl;
    }
}