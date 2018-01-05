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
    Position(int X, int Y) { x = X; y = Y; }
    int x = 0;
    int y = 0;

    bool operator==(const Position& pos) const {
        return (this->x == pos.x) && (this->y == pos.y);
    }
};

Position newPosition(Position actuel, Direction dir) {
    switch(dir) {
        case SUD:
            return Position(actuel.x, actuel.y + 1);
        case EST:
            return Position(actuel.x + 1, actuel.y);
        case NORD:
            return Position(actuel.x, actuel.y - 1);
        case OUEST:
            return Position(actuel.x - 1, actuel.y);
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
        Position posBender(0, 0);
        bool biere = false;
        bool inverseur = false;
        Direction directionActuelle = SUD;
        vector<Etat> etatsPrecedents;

        bool cabineAtteinte = false;

        for (int y = 0 ; y < L ; y++) {
            string row;
            getline(cin, row);
            cerr << row;
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
            }cerr << endl;
        }cerr << endl;

        /// SIMULATION ///
        string answer;

        //cerr << "Position initiale de Bender : (" << posBender.x << ", " << posBender.y << ')' << endl;
        while(!cabineAtteinte) {

            //cerr << "Position de Bender : X = " << posBender.x << " ; Y = " << posBender.y << endl;
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
                bool modifDirection = false;
                Direction directionFuture = SUD;

                etatsPrecedents.push_back(etatBender);
                Position newPos = newPosition(posBender, directionActuelle);
                // On avance d'un pas
                switch (carte[newPos.x][newPos.y]) {
                    case VIDE: {
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : VIDE" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;

                        break;}

                    case DEPART: {
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : DEPART" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case CABINE: {
                        cabineAtteinte = true;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : CABINE" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case BIERE: {
                        biere = !biere;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : BIERE" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case TELEPORTEUR: {
                        if(newPos == teleporteurs[0])
                            posBender = teleporteurs[1];
                        else
                            posBender = teleporteurs[0];

                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : TELEPORTEUR ; goto (" << posBender.x << ", " << posBender.y << ')' << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        break;}

                    case INVERSEUR: {
                        inverseur = !inverseur;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : INVERSEUR" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case MODIF_OUEST: {
                        modifDirection = true;
                        directionFuture = OUEST;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : MODIF_OUEST" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case MODIF_EST: {
                        modifDirection = true;
                        directionFuture = EST;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : MODIF_EST" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case MODIF_NORD: {
                        modifDirection = true;
                        directionFuture = NORD;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : MODIF_NORD" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case MODIF_SUD: {
                        modifDirection = true;
                        directionFuture = SUD;
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : MODIF_SUD" << endl;
                        //cerr << "Bender passe de (" << posBender.x << ", " << posBender.y << ") à (" << newPos.x << ", " << newPos.y << ')' << endl;
                        posBender = newPos;
                        break;}

                    case OBSTACLE: {
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : OBSTACLE" << endl;
                        if (biere) {
                            posBender = newPos;
                            carte[newPos.x][newPos.y] = VIDE;
                            etatsPrecedents.clear();
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
                        //cerr << "case (" << newPos.x << ", " << newPos.y << ") : BORDURE" << endl;
                        directionActuelle = (inverseur ? OUEST : SUD);
                        while(carte[newPosition(posBender, directionActuelle).x][newPosition(posBender, directionActuelle).y] == OBSTACLE
                              || carte[newPosition(posBender, directionActuelle).x][newPosition(posBender, directionActuelle).y] == BORDURE) {
                            directionActuelle = getNext(directionActuelle, inverseur);
                        }
                        break;}
                }

                if(carte[newPos.x][newPos.y] != BORDURE && (carte[newPos.x][newPos.y] != OBSTACLE && !biere))
                    answer += to_string(directionActuelle) + "\n";
                else if(carte[newPos.x][newPos.y] != BORDURE && biere)
                    answer += to_string(directionActuelle) + "\n";
                //if(carte[newPos.x][newPos.y] != BORDURE && (carte[newPos.x][newPos.y] != OBSTACLE))
                //    cerr << "CHANGEMENT DE DIRECTION EFFECTUE (" << to_string(directionActuelle) << ")\n" << endl;
                if(modifDirection)
                    directionActuelle = directionFuture;
            }
        }

        cout << answer << endl;

#ifdef _CLION_
        string goodAnswer;
        getline(cin, goodAnswer);
        if(goodAnswer != answer) {
            testsOk = false;
            cerr << "\n\nTrouvé : " << answer << endl;
            cerr << "Attendu : " << goodAnswer << endl;
        }
    }

    if(testsOk)
        cerr << "Tous les tests sont bons! =)" << endl;
    else
        cerr << "WARNING !! Un test n'est pas bon =(" << endl;
#endif
}