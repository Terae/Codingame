#include <iostream>

#define NBR_TESTS 5

#ifdef _CLION_
#include <fstream>
#endif

/**
 * Afficher une ligne de texte en ASCII art (l'alphabet utilisé est en entrée)
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
        // largeur d'une lettre
        int X;
        cin >> X;
        cin.ignore();
        if (X != 20)
            X--;
        // hauteur d'une lettre
        int Y;
        cin >> Y;
        cin.ignore();
        // ligne de texte
        string T;
        getline(cin, T);
        locale loc;
        for (string::size_type i = 0; i < T.length(); i++) {
            T[i] = toupper(T[i], loc);
            if (T[i] > 'Z' || T[i] < 'A')
                T[i] = 'Z' + 1;
            T[i] -= 'A';
        }

        // On récupère les définitions des lettres ascii
        char alphabet[27][X][Y];
        for (int y = 0; y < Y; y++) {
            string ROW;
            getline(cin, ROW);
            int x = 0;
            for (int a = 0; a < 27; a++) {
                for (int i = 0; i < X; i++) {
                    alphabet[a][i][y] = ROW[x];
                    x++;
                }
                if (X != 20)
                    x++;
            }
        }

        // On écrit les mots
        for (int y = 0; y < Y; y++) { // ligne après ligne
            string answer;
            for (string::size_type i = 0; i < T.length(); i++) { // lettre après lettre
                for (int x = 0; x < X; x++) { // caractère après caractère
                    answer += alphabet[(int) T[i]][x][y];
                }
                if (X != 20)
                    answer += " ";
            }
            // On envoie le résultat
            cout << answer << endl;
#ifdef _CLION_
            string goodAnswer;
            getline(cin, goodAnswer);
            if(goodAnswer != answer) {
                testsOk = false;
                cerr << "\n\n Trouvé : " << answer << endl;
                cerr << "Attendu : " << goodAnswer << endl;
            }
#endif
        }
#ifdef _CLION_
    }
    if(testsOk)
        cerr << "Tous les tests sont bons! =)" << endl;
    else
        cerr << "WARNING !! Un test n'est pas bon =(" << endl;
#endif

    return 0;
}