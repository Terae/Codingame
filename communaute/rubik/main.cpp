#include <iostream>

using namespace std;

int main() {
    int N;
    cin >> N; cin.ignore();
    cout << (N == 1 ? 1 : 2*(N*N + N*(N-2) + (N-2)*(N-2))) << endl;
}