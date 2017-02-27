#include <iostream>
#include <deque>
#include <vector>

#define NBR_TESTS 9

#ifdef _CLION_
#include <fstream>
#endif

/**
 * Déterminer le gagnant d'une bataille (jeu de cartes déterministe)
 */

using namespace std;

enum Couleur {
    C = 'C', // trèfle
    D = 'D', // carreau
    H = 'H', // coeur
    S = 'S'  // pique
};

struct Carte {
    int numero;
    Couleur couleur;
};

Carte getCarte(string name) {
    Carte carte;
    if(name[0] - '0' < 10) { // de 2 à 9
        carte.numero = name[0] - '0';
        if(carte.numero == 1)
            carte.numero = 10;
    }
    else { // J, Q, K, A
        char tete = name[0];
        if(tete == 'J')
            carte.numero = 11;
        else if(tete == 'Q')
            carte.numero = 12;
        else if(tete == 'K')
            carte.numero = 13;
        else if(tete == 'A')
            carte.numero = 14;
    }
    
    carte.couleur = (Couleur)name[name.length() - 1];
    
    return carte;
}

string cardName(Carte carte) {
    string result = "";
    if(carte.numero < 10)
        result = " " + to_string(carte.numero);
    else if(carte.numero == 10)
        result = to_string(carte.numero);
    else if(carte.numero == 11)
        result = " J";
    else if(carte.numero == 12)
        result = " Q";
    else if(carte.numero == 13)
        result = " K";
    else
        result = " A";
        
    //result += carte.couleur;
    
    return result;
}


int main(int argc, char **argv) {
#ifdef _CLION_
    bool testsOk = true;
    for(int test = 1 ; test <= NBR_TESTS ; test++) {
        string path(argv[0]);
        ifstream TestFile(path.substr(0, path.find_last_of("/")) + "/Test" + to_string(test) + ".txt", ios::in);
        if (!TestFile) return 1;
        cin.rdbuf(TestFile.rdbuf());
#endif
        /////////////////////////////
        // Récupération des cartes //
        /////////////////////////////
        int n; // the number of cards for player 1
        cin >> n;
        cin.ignore();
        deque<Carte> joueur1;
        cerr << "Le joueur 1 a " << n << " cartes : " << endl;
        for (int i = 0; i < n; i++) {
            string cardp1; // the n cards of player 1
            cin >> cardp1;
            cin.ignore();
            joueur1.push_back(getCarte(cardp1));
            cerr << cardName(joueur1.back()) << ' ';
        }
        cerr << '\n' << endl;

        int m; // the number of cards for player 2
        cin >> m;
        cin.ignore();
        deque<Carte> joueur2;
        cerr << "Le joueur 2 a " << m << " cartes : " << endl;
        for (int i = 0; i < m; i++) {
            string cardp2; // the m cards of player 2
            cin >> cardp2;
            cin.ignore();
            joueur2.push_back(getCarte(cardp2));
            cerr << cardName(joueur2.back()) << ' ';
        }
        cerr << "\n" << endl;

        ////////////////////////////
        // Lancement de la partie //
        ////////////////////////////
        int nbrTours = 0;
        // pair<joueur1? , Carte>
        vector<pair<bool, Carte>> defausse;
        bool joueur1Gagne;
        string answer = "";
        while (answer == "") {
            // Un joueur n'a plus de carte
            if (joueur1.empty()) {
                answer = "2 " + to_string(nbrTours);
            } else if (joueur2.empty()) {
                answer = "1 " + to_string(nbrTours);
            }

                // BATAILLE!
            else if (joueur1.front().numero == joueur2.front().numero) {
                // joueurs ex aequo (pas assez de cartes pour une bataille)
                if (joueur1.size() < 4 || joueur2.size() < 4) {
                    answer = "PAT";
                } else {
                    // On défausse 3 cartes
                    for (int i = 0 ; i < 4 ; i++) {
                        defausse.push_back(make_pair(true, joueur1.front()));
                        defausse.push_back(make_pair(false, joueur2.front()));
                        joueur1.pop_front();
                        joueur2.pop_front();
                    }
                }

            } else { // Qui gagne?
                if (joueur1.front().numero < joueur2.front().numero) { // joueur 2 gagne
                    joueur1Gagne = false;
                    defausse.push_back(make_pair(false, joueur2.front()));
                    defausse.push_back(make_pair(true, joueur1.front()));
                } else {
                    joueur1Gagne = true;
                    defausse.push_back(make_pair(true, joueur1.front()));
                    defausse.push_back(make_pair(false, joueur2.front()));
                }
                joueur1.pop_front();
                joueur2.pop_front();

                // On ajoute les cartes dans le bon ordre
                if (joueur1Gagne) {
                    for (auto carte : defausse)
                        if (carte.first)  // cartes du joueur 1
                            joueur1.push_back(carte.second);

                    for (auto carte : defausse)
                        if (!carte.first) // cartes du joueur 2
                            joueur1.push_back(carte.second);
                } else {
                    for (auto carte : defausse)
                        if (!carte.first) // cartes du joueur 2
                            joueur2.push_back(carte.second);

                    for (auto carte : defausse)
                        if (carte.first)  // cartes du joueur 1
                            joueur2.push_back(carte.second);
                }
                defausse.clear();
                nbrTours++;
            }
            cerr << "\n----- Tour " << nbrTours << " -----\n* Joueur 1 (" << joueur1.size() << " cartes) :" << endl;
            for (auto carte : joueur1)
                cerr << cardName(carte) << ' ';
            cerr << "\n* Joueur 2 (" << joueur2.size() << " cartes) :" << endl;
            for (auto carte : joueur2)
                cerr << cardName(carte) << ' ';
            cerr << "\n* Defausse (" << defausse.size() << " cartes) :" << endl;
            if (!defausse.empty()) {
                cerr << "joueur 1 :" << endl;
                for (auto carte : defausse)
                    if (carte.first)
                        cerr << cardName(carte.second) << ' ';
                cerr << "\njoueur 2 :" << endl;
                for (auto carte : defausse)
                    if (!carte.first)
                        cerr << cardName(carte.second) << ' ';
                cerr << endl;
            }
        }

        cout << answer << endl;

#ifdef _CLION_
        string goodAnswer;
        getline(cin, goodAnswer);
        if(goodAnswer != answer) {
            testsOk = false;
            cerr << "\n\n Trouvé : " << answer << endl;
            cerr << "Attendu : " << goodAnswer << endl;
        }
    }

    if(testsOk)
        cerr << "Tous les tests sont bons! =)" << endl;
    else
        cerr << "WARNING !! Un test n'est pas bon =(" << endl;
#endif
}