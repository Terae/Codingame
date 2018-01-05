#include <iostream>
#include <limits>

#define NBR_TESTS 6

#ifdef _CLION_
#include <fstream>
#endif

/**
 * trouver la température la plus proche de 0°C (utilisation de fichiers tests)
 */

using namespace std;

int main(int argc, char **argv) {
#ifdef _CLION_
    bool testsOk = true;
    for(int test = 1 ; test <= NBR_TESTS ; test++) {
        string path(argv[0]);
        ifstream TestFile(path.substr(0, path.find_last_of("/")) + "/Test" + to_string(test) + ".txt", ios::in);
        if (!TestFile) return 1;
        cin.rdbuf(TestFile.rdbuf());
#endif

        int N;
        cin >> N;
        cin.ignore();

        int nbr;
        int min = numeric_limits<int>::max();

        for (int i = 0; i < N; ++i) {
            cin >> nbr;
            cin.ignore();

            if (abs(nbr) < abs(min))
                min = nbr;
            else if (abs(min) == abs(nbr) && min < 0)
                min = nbr;
        }

        int answer = N > 0 ? min : 0;
        cout << answer << endl;

#ifdef _CLION_
        int goodAnswer;
        cin >> goodAnswer; cin.ignore();
        if(goodAnswer != answer) {
            testsOk = false;
            cerr << "\n\n Trouvé : " << answer << endl;
            cerr << "Attendu : " << goodAnswer << endl;
        }
    }

    if(testsOk)
        cerr << "\n\nTous les tests sont bons =)" << endl;
    else
        cerr << "\n\nWARNING : au moins un test n'est pas bon =(" << endl;
#endif
}