#include <iostream>

using namespace std;

int main() {
    int width;
    int height;
    cin >> width >> height; cin.ignore();
    bool init[width+2][height+2];
    for(int x = 0; x < width+2; x++)
        for (int y = 0; y < height+2; y++)
            init[x][y] = false;
    
    for (int y = 1; y < height+1; y++) {
        string line;
        cin >> line; cin.ignore();
        for (int x = 0; x < width; x++)
            init[x+1][y] = line[x] - '0';
    }
    
    for (int y = 1; y < height+1; y++) {
        for (int x = 1; x < width+1; x++) {
            int total = init[x-1][y+1] + init[x][y+1] + init[x+1][y+1]
                      + init[x-1][y]                  + init[x+1][y]
                      + init[x-1][y-1] + init[x][y-1] + init[x+1][y-1];
            // Dead cell
            if(!init[x][y])
                cout << (total == 3);
            // Alive cell
            else
                cout << (total == 2 || total == 3);
        }
        cout << endl;
    }
}