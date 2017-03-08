#include <iostream>
using namespace std;
int main()
{
    int N;
    cin >> N; cin.ignore();
    string ref="99:99:99";
    for (int i = 0; i < N; i++) {
        string t;
        cin >> t;
        unsigned long long T = (t[0]*10 + t[1]) * 60 * 60 + (t[3]*10 + t[4]) * 60 + (t[6]*10 + t[7]);
        unsigned long long REF = (ref[0]*10 + ref[1]) * 60 * 60 + (ref[3]*10 + ref[4]) * 60 + (ref[6]*10 + ref[7]);
        if(REF > T)ref = t;
    }
    cout << ref << endl;
}