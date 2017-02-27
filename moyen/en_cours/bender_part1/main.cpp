#include <iostream>
#include <vector>
#include <algorithm>

#define NBR_TESTS 1

#ifdef _CLION_
#include <fstream>
#endif

using namespace std;

enum Direction {
    SUD = 0,
    EST,
    NORD,
    OUEST
};

Direction getNext(Direction dir, bool inverseur) {
    if(inverseur) return (Direction)(((int)dir - 1) % 4);
    else          return (Direction)(((int)dir + 1) % 4);
}

string to_string(Direction dir) {
    switch(dir) {
        case SUD:
            return "SOUTH";
        case EST:
            return "EAST";
        case NORD:
            return "NORTH";
        case OUEST:
            return "WEST";
    }
}

enum Objet {
    DEPART      = '@',
    CABINE      = '$',
    BIERE       = 'B',
    OBSTACLE    = 'X',
    BORDURE     = '#',
    TELEPORTEUR = 'T',
    INVERSEUR   = 'I',
    MODIF_NORD  = 'N',
    MODIF_SUD   = 'S',
    MODIF_EST   = 'E',
    MODIF_OUEST = 'W',
    VIDE        = ' '
};

struct Position {
    int x = 0;
    int y = 0;

    bool operator==(const Position& pos) const {
        return (this->x == pos.x) && (this->y == pos.y);
    }
};

Position newPosition(Position actuel, Direction dir) {
    switch(dir) {
        case SUD:
            return {actuel.x, actuel.y - 1};
        case EST:
            return {actuel.x + 1, actuel.y};
        case NORD:
            return {actuel.x, actuel.y + 1};
        case OUEST:
            return {actuel.x - 1, actuel.y};
    }
}

// On mémorise tous les états de Bender dans une map (dès qu'on en a deux identiques, on entre dans une LOOP)
struct Etat {
    Position _pos;
    Direction _dir;
    bool _inverseur;
    bool _biere;

    bool operator==(const Etat& etat) const {
        return (this->_pos == etat._pos) && (this->_dir == etat._dir) &&
               (this->_inverseur == etat._inverseur) && (this->_biere == etat._biere);
    }
};

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
        int C;
        cin >> L >> C; cin.ignore();
        Objet carte[C][L];
        vector<Position> teleporteurs;
        Position posBender;
        bool biere = false;
        bool inverseur = false;
        Direction directionActuelle = SUD;
        vector<Etat> etatsPrecedents;

        bool cabineAtteinte = false;

        for (int y = 0 ; y < L ; y++) {
            string row;
            getline(cin, row);
            for(int x = 0 ; x < C ; x++) {
                carte[x][y] = (Objet)row[x];

                switch(carte[x][y]) {
                    case TELEPORTEUR:
                        teleporteurs.push_back({x, y});
                        break;
                    case DEPART:
                        posBender.x = x;
                        posBender.y = y;
                        break;
                    default:
                        break;
                }
            }
        }

        /// SIMULATION ///
        string answer;

        while(!cabineAtteinte) {
            // On vérifie que Bender ne soit pas dans une boucle (ie qu'il a déjà été dans la même configuration)
            bool loop = false;
            Etat etatBender = {posBender, directionActuelle, inverseur, biere};
            for(auto etat : etatsPrecedents) {
                if (etat == etatBender) {
                    loop = true;
                    break;
                }
            }

            if(loop) {
                answer = "LOOP";
                break;
            } else {
                etatsPrecedents.push_back(etatBender);
                Position newPos = newPosition(posBender, directionActuelle);
                // On avance d'un pas
                switch (carte[newPos.x][newPos.y]) {
                    case VIDE: {
                        posBender = newPos;
                        break;}

                    case DEPART: {
                        posBender = newPos;
                        break;}

                    case CABINE: {
                        cabineAtteinte = true;
                        posBender = newPos;
                        break;}

                    case BIERE: {
                        biere = !biere;
                        posBender = newPos;
                        break;}

                    case TELEPORTEUR: {
                        Position teleporteur1 = teleporteurs[0];
                        posBender = (teleporteur1 == posBender ? teleporteurs[1] : teleporteur1);
                        break;}

                    case INVERSEUR: {
                        inverseur = !inverseur;
                        posBender = newPos;
                        break;}

                    case MODIF_OUEST: {
                        directionActuelle = OUEST;
                        posBender = newPos;
                        break;}

                    case MODIF_EST: {
                        directionActuelle = EST;
                        posBender = newPos;
                        break;}

                    case MODIF_NORD: {
                        directionActuelle = NORD;
                        posBender = newPos;
                        break;}

                    case MODIF_SUD: {
                        directionActuelle = SUD;
                        posBender = newPos;
                        break;}

                    case OBSTACLE: {
                        if (biere) {
                            posBender = newPos;
                            carte[newPos.x][newPos.y] = VIDE;
                            // il faudrait check si un état précédent empéchait Bender de passer ici mais
                            // que maintenant qu'il n'y a plus d'obstacle il peut passer mais on pense qu'il est en LOOP
                        } else {
                            // On choisit la direction suivante
                            directionActuelle = (inverseur ? OUEST : SUD);
                            while(carte[newPosition(posBender, directionActuelle).x][newPosition(posBender, directionActuelle).y] == OBSTACLE
                               || carte[newPosition(posBender, directionActuelle).x][newPosition(posBender, directionActuelle).y] == BORDURE) {
                                directionActuelle = getNext(directionActuelle, inverseur);
                            }
                        }
                        break;}

                    case BORDURE: {
                        directionActuelle = (inverseur ? OUEST : SUD);
                        while(carte[newPosition(posBender, directionActuelle).x][newPosition(posBender, directionActuelle).y] == OBSTACLE
                              || carte[newPosition(posBender, directionActuelle).x][newPosition(posBender, directionActuelle).y] == BORDURE) {
                            directionActuelle = getNext(directionActuelle, inverseur);
                        }
                        //newPos = newPosition(posBender, directionActuelle);
                        break;}
                }

                if(carte[newPos.x][newPos.y] != BORDURE && (carte[newPos.x][newPos.y] != OBSTACLE && !biere))
                    answer += to_string(directionActuelle) + "\n";
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