#include <iostream>
#include <vector>
#include <algorithm>

#define NBR_TESTS 12

#ifdef _CLION_
#include <fstream>
#endif

/**
 * Effectuer une opération mathématique entre deux nombres Mayas (de 0 à 19)
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
        int L;
        int H;
        cin >> L >> H;
        cin.ignore();

        /// Récupération de l'alphabet
        char maya[20][L][H];
        for (int h = 0; h < H; h++) {
            string numeral;
            cin >> numeral;
            cin.ignore();

            for (int nbr = 0; nbr < 20; nbr++)
                for (int l = 0; l < L; l++)
                    maya[nbr][l][h] = numeral[nbr * L + l];
        }

        /// Affichage de l'alphabet Maya
        /*for(int nbr = 0 ; nbr < 20 ; nbr++) {
            cerr << "Représentation Maya de " << nbr << " :" << endl;
            for(int h = 0 ; h < H ; h++) {
                for(int l = 0 ; l < L ; l++)
                    cerr << maya[nbr][l][h];
                cerr << endl;
            }
            cerr << endl;
        }*/

        /// Récupération de S1
        int S1;
        cin >> S1;
        cin.ignore();
        char nbr1[S1 / L][L][H];
        for (int i = 0; i < S1; i++) {
            string num1Line;
            cin >> num1Line;
            cin.ignore();
            //cerr << num1Line << endl;
            for (int l = 0; l < L; l++)
                nbr1[S1 / L - 1 - i / L][l][i % L] = num1Line[l];
        }//cerr << '\n' << endl;

        /// Récupération de S2
        int S2;
        cin >> S2;
        cin.ignore();
        char nbr2[S2 / L][L][H];
        for (int i = 0; i < S2; i++) {
            string num2Line;
            cin >> num2Line;
            cin.ignore();
            for (int l = 0; l < L; l++)
                nbr2[S2 / L - 1 - i / L][l][i % L] = num2Line[l];
        }

        /// Affichage de S1 et de S2
        /*cerr << "---------- S1 ----------";
        for(int digit = 0 ; digit < S1 / L ; digit++) {
            cerr << "\nDigit " << digit << " :" << endl;
            for(int h = 0 ; h < H ; h++) {
                for(int l = 0 ; l < L ; l++)
                    cerr << nbr1[digit][l][h];
                cerr << endl;
            }
            cerr << endl;
        }

        cerr << "\n\n---------- S2 ----------";
        for(int digit = 0 ; digit < S2 / L ; digit++) {
            cerr << "\nDigit " << digit << " :" << endl;
            for(int h = 0 ; h < H ; h++) {
                for(int l = 0 ; l < L ; l++)
                    cerr << nbr2[digit][l][h];
                cerr << endl;
            }
            cerr << endl;
        }*/

        /// Conversion de S1 en décimal
        unsigned long int nbr1Int = 0;
        for (int digit = 0; digit < S1 / L; digit++) {
            //cerr << "On regarde le digit " << digit + 1 << " du premier nombre S1" << endl;
            // On regarde pour chaque digit à quoi il correspond
            for (int nbr = 0; nbr < 20; nbr++) {
                bool digit1Equal = true;
                //cerr << "\nnbr = " << nbr << endl;
                for (int l = 0; l < L; l++) {
                    for (int h = 0; h < H; h++) {
                        //cerr << maya[nbr][l][h] << ' ' << nbr1[digit][l][h] << "     " << endl;
                        if (maya[nbr][l][h] != nbr1[digit][l][h]) {
                            //cerr << "Le nombre " << nbr << " n'est pas bon : " << maya[nbr][l][h]  << " != " << nbr1[digit][l][h] << endl;
                            digit1Equal = false;
                        }
                    }
                }

                // Si on a trouvé le bon nombre, on l'ajoute au nombre décimal
                if (digit1Equal) {
                    //cerr << "On a trouvé le bon nombre : S1 - digit " << digit << " est égal à " << nbr << endl;
                    nbr1Int += pow(20, digit) * nbr;
                    //cerr << "Le digit  " << digit << "  de S1 vaut  " << nbr << endl;
                    break;
                }
            }
        }

        /// Conversion de S2 en décimal
        unsigned long int nbr2Int = 0;
        for (int digit = 0; digit < S2 / L; digit++) {
            for (int nbr = 0; nbr < 20; nbr++) {
                bool digit2Equal = true;
                for (int l = 0; l < L; l++)
                    for (int h = 0; h < H; h++)
                        if (maya[nbr][l][h] != nbr2[digit][l][h])
                            digit2Equal = false;
                if (digit2Equal) {
                    nbr2Int += pow(20, digit) * nbr;
                    //cerr << "Le digit  " << digit << "  de S2 vaut  " << nbr << endl;
                    break;
                }
            }
        }

        string operation;
        cin >> operation;
        cin.ignore();

        unsigned long int result;
        if (operation == "+")
            result = nbr1Int + nbr2Int;
        else if (operation == "-")
            result = nbr1Int - nbr2Int;
        else if (operation == "*")
            result = nbr1Int * nbr2Int;
        else
            result = nbr1Int / nbr2Int;
        //cerr << "Calcul à effectuer : " << nbr1Int << ' ' << operation << ' ' << nbr2Int << "  =  " << result;

        /// Conversion du résultat en Maya
        vector<int> resultMaya;
        while (result != 0) {
            int valeurDigit = result % 20;
            resultMaya.push_back(valeurDigit);
            result -= valeurDigit;
            result /= 20;
        }
        if (resultMaya.size() == 0)
            resultMaya.push_back(0);

        /*cerr << "  =  ";
        for (int i = 0; i < resultMaya.size(); i++) {
            cerr << " + " << resultMaya[i] << " * 20 ^ " << i;
        }
        cerr << '\n' << endl;*/

        /// Conversion en chiffres Mayas
        for (int digit = resultMaya.size() - 1; digit >= 0; digit--) {
            for (int h = 0; h < H; h++) {
                string answer = "";
                for (int l = 0; l < L; l++)
                    answer += maya[resultMaya[digit]][l][h];
                cout << answer << endl;
#ifdef _CLION_
                string goodAnswer;
                getline(cin, goodAnswer);
                if (goodAnswer != answer) {
                    testsOk = false;
                    cerr << "\n\n Trouvé : " << answer << endl;
                    cerr << "Attendu : " << goodAnswer << endl;
                }
#endif
            }
        }

#ifdef _CLION_
    }
    if(testsOk)
        cerr << "Tous les tests sont bons! =)" << endl;
    else
        cerr << "WARNING !! Un test n'est pas bon =(" << endl;
#endif
}