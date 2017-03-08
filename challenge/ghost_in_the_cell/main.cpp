/// Code envoyé sur le site codingame.com (copié collé de tout le projet)

#include <memory>
#include <iostream>
#include <limits>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>

#define PLUSIEURS_ACTIONS true
#define BOMBES            true
#define INCREASE          true

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Usine.h
 */

enum Proprietaire {
    MOI = 1,
    NEUTRE = 0,
    ADVERSAIRE = -1,

    AUTRES_QUE_MOI = 2,
};

class Usine;
/*
 * Permet de définir un lien entre 2 usines séparées par une certaine distance (qui correspond au nbr de tours de jeu nécessaires d'un usine à l'autre)
 */
class Edge {
public:
    Edge(const shared_ptr<Usine> origine, const shared_ptr<Usine> arrivee, int distance) {
        _origine  = origine;
        _arrivee  = arrivee;
        _distance = distance;
    }

    // Permet de connaître le nombre de cyborgs qu'aura l'usine adverse : cyborgs + 2 * tours
    inline int getNbrCyborgsOrigine() const;
    inline int getNbrCyborgsArrivee() const;

    inline Proprietaire getPropOrigine() const;
    inline Proprietaire getPropArrivee() const;

    inline int getProductionOrigine() const;
    inline int getProductionArrivee() const;

    inline shared_ptr<Usine> getUsineArrivee() const;

    inline int getDistance() const;

    inline int getIDAdverse() const;

    inline friend ostream& operator<<(ostream& os, const Edge& edge) {
        os << " - Distance : " << edge._distance << " tours";
        return os;
    }

private:
    shared_ptr<Usine> _origine;
    shared_ptr<Usine> _arrivee;
    int _distance;
};

/**
 * Définit une usine depuis son ID
 */
class Usine : public std::enable_shared_from_this<Usine> {
public:
    /**
     * Initialisation
     */
    Usine(int id) {
        _id = id;
    }

    void addEdge(const shared_ptr<Usine> another, int dist);

    /**
     * Accesseurs
     */
    inline int getID() const { return _id; }

    inline int  getNbrCyborgs() const { return _nbr_cyborgs; }
    inline void setNbrCyborgs(int nbr) { _nbr_cyborgs = nbr; }

    // from 0 to 3
    inline int  getProduction() const { return _production; }
    inline void setProduction(int p) { _production = p; }

    inline int  getTempsAvantProduction() const { return _tempsAvantProduction; }
    inline void setTempsAvantProduction(int tps) { _tempsAvantProduction = tps; }

    inline Proprietaire getProprietaire() const { return _proprietaire; }
    inline void         setProprietaire(Proprietaire p) { _proprietaire = p; }

    int getDistance(shared_ptr<Usine> usine);

    // Liste de toutes les usines à prendre chez les autres
    vector<pair<int, int>> getUsinesAPrendre(int &cyborgs_restants); // TODO : améliorer

    friend ostream& operator<<(ostream& os, const shared_ptr<Usine> &usine) {
        /* os << "Production de " << usine->_production << " cyborgs par tour\nTemps avant que l'usine ne reproduise : "
            << usine->_tempsAvantProduction << "\n" << usine->_nbr_cyborgs << " en stock\nPropriétaire : "
            << getName(usine->_proprietaire) << "\nListe des usines :\n";*/
        for(int i = 0 ; i < usine->_edges.size() ; i++) {
            os << i << usine->_edges[i] << '\n';
        }
        os << '\n';
        return os;
    }

private:
    int _id;
    int _nbr_cyborgs;
    int _production;
    // 0 : tout va bien ; 5 : on s'est pris une bombe puis ça décroît
    int _tempsAvantProduction;
    Proprietaire _proprietaire = NEUTRE;
    // ensemble des liens que l'usine a avec d'autres
    vector<Edge> _edges;

    int getNbrCyborgsPourAttaquerAdversaire(const Edge &edge) const;
};



/**
 * Usine.cpp
 */
inline int Edge::getNbrCyborgsOrigine() const { return _origine->getNbrCyborgs(); }
inline int Edge::getNbrCyborgsArrivee() const { return _arrivee->getNbrCyborgs(); }
inline int Edge::getIDAdverse()  const { return _arrivee->getID(); }
inline Proprietaire Edge::getPropOrigine() const { return _origine->getProprietaire(); }
inline Proprietaire Edge::getPropArrivee() const { return _arrivee->getProprietaire(); }
inline int Edge::getProductionOrigine() const { return _origine->getProduction(); }
inline int Edge::getProductionArrivee() const { return _arrivee->getProduction(); }
inline int Edge::getDistance() const { return _distance; }
inline shared_ptr<Usine> Edge::getUsineArrivee() const { return _arrivee; }

void Usine::addEdge(const shared_ptr<Usine> another, int dist) {
    // la classe Usine ne possède pas sa propre instanciation par des shared_ptr (c'est Graph qui l'a)
    // du coup on récupère la valeur du ptr grace à une dépendance circulaire ; c'est Graph qui la possède et on l'obtient depuis 'this'
    Edge edge(shared_from_this(), another, dist);
    _edges.push_back(edge);
}

int Usine::getDistance(shared_ptr<Usine> usine) {
    for(auto edge : _edges)
        if(edge.getUsineArrivee() == usine)
            return edge.getDistance();
    return numeric_limits<int>::max();
}

vector<pair<int, int>> Usine::getUsinesAPrendre(int &cyborgs_restants) {
    //chrono::system_clock::time_point begin = std::chrono::system_clock::now();

    vector<pair<int, int>> result;
    // On attaque les positions de l'adversaire ...
    for (auto edge : _edges) {
        int nbr = getNbrCyborgsPourAttaquerAdversaire(edge);
        // On a assez de cyborgs
        if (edge.getPropArrivee() == ADVERSAIRE && nbr < cyborgs_restants) {
            cyborgs_restants -= nbr + 1;
            result.push_back(make_pair(edge.getIDAdverse(), nbr + 1));
        }
    }
    // ... puis celles des zones neutres avec ce qu'il reste
    for (auto edge : _edges) {
        int nbr = getNbrCyborgsPourAttaquerAdversaire(edge);
        // On a assez de cyborgs
        if (edge.getPropArrivee() == NEUTRE && nbr < cyborgs_restants) {
            cyborgs_restants -= nbr + 1;
            result.push_back(make_pair(edge.getIDAdverse(), nbr + 1));
        }
    }
    //cerr << "Usine::getUsinesAPrendre\n";
    //cerr << "dans getUsinesAPrendre : temps d'exécution : " << chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - begin).count() << " microS\n";
    return result;
}

int Usine::getNbrCyborgsPourAttaquerAdversaire(const Edge &edge) const {
    return edge.getNbrCyborgsArrivee() + edge.getDistance() * edge.getProductionArrivee() + 1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Graph.h
 */

class Graph {
public:
    /**
     * Initialisation
     */
    Graph() {}

    // A appeler au tout début après la construction du graphe
    void setNbrUsines(int nbr_usines) {
        for(int id = 0 ; id < nbr_usines ; id++) {
            _usines.push_back(make_shared<Usine>(id));
        }
    }

    void addEdge(int origine, int arrivee, int distance) {
        _usines[origine]->addEdge(_usines[arrivee], distance);
    }

    // Calcule l'éloignement de chaque usine par rapport aux autres
    void init();

    void updateUsine(int id, int prop, int nbr_cyborgs, int production, int tempsAvantProduction, int arg5) {
        _usines[id]->setProprietaire((Proprietaire)prop);
        _usines[id]->setNbrCyborgs(nbr_cyborgs);
        _usines[id]->setProduction(production);
        _usines[id]->setTempsAvantProduction(tempsAvantProduction);
    }

    /**
     * Traitement des données
     */
    int getNbrUsines() const { return (int)_usines.size(); }
    int getNbrCyborgsTotal(Proprietaire prop) const;
    void enleverCyborgs(shared_ptr<Usine> usine, int nbr);

    inline shared_ptr<Usine> getUsine(int id) const { return _usines[id]; }

    // On récupère l'ensemble des usines d'un joueur
    // Ordre de priorité : niveau de prod gros->petit puis nbr de cyborgs peu->bcp
    vector<shared_ptr<Usine>> getListeUsines(Proprietaire prop) const;

    // On récupère l'usine qui possède le plus de cyborgs
    shared_ptr<Usine> getUsineMaxCyborgs(Proprietaire prop = ADVERSAIRE, int productionMin = -1) const;

    // On récupère l'ensemble des usines qui ont une production donnée pour un joueur
    vector<shared_ptr<Usine>> getUsinesSelonProduction(int production, Proprietaire prop = AUTRES_QUE_MOI) const;

    // Si produit = true, alors on récupère les usines qui ne se sont pas prises de bombes
    vector<shared_ptr<Usine>> getUsinesSiProduction(bool produit = true, Proprietaire prop = MOI) const;

    // Renvoie la liste des usines de prop de la plus proche à la plus éloignée de la référence
    vector<shared_ptr<Usine>> getListeUsinesOrdreDistance(shared_ptr<Usine> reference, Proprietaire prop = MOI) const;

    // On récupère l'ensemble de nos usines qu'on peut upgrader avec nbr_cyborgs_restants à la fin
    vector<shared_ptr<Usine>> getUsinesPouvantUpgrader(int productionMaxActuelle = 1, int nbr_cyborgs_restants = 0) const;

    friend ostream& operator<<(ostream& os, Graph& graph) {
        os << "Il y a " << graph._usines.size() << " usines connues.\n";
        for(int i = 0 ;  i < graph._usines.size() ; i++) {
            os << "---- Informations sur l'usine " << i << " ----\n" << graph._usines[i];
        }
        return os;
    }

    void afficherMessageDebug(string message, shared_ptr<Usine> usine) {
        cerr << message << usine->getID() << " : cyborgs = " << usine->getNbrCyborgs() << " ; prod = " << usine->getProduction() << endl;
    }

    void afficherMessageDebug(string message, vector<shared_ptr<Usine>> usines) {
        cerr << message << endl;
        for(auto usine : usines)
            afficherMessageDebug("", usine);

        cerr << '\n' << endl;
    }


private:
    // On recense toutes les usines sur le graphe
    vector<shared_ptr<Usine>> _usines;
    // Permet de connaître la liste de chacune des usines par rapport à une selon leur éloignement
    vector<vector<shared_ptr<Usine>>> _eloignement;
};

struct Troupe {
    Troupe(const Graph& graph, int prop, int id1, int id2, int cyb, int tours) {
        _proprietaire = (Proprietaire)prop;
        _origine = graph.getUsine(id1);
        _arrivee = graph.getUsine(id2);
        _nbr_cyborgs = cyb;
        _nbr_tours_restants = tours;
    }

    Proprietaire _proprietaire;
    shared_ptr<Usine> _origine;
    shared_ptr<Usine> _arrivee;
    int _nbr_cyborgs;
    int _nbr_tours_restants;
};

struct Bombe {
    Bombe(const Graph& graph, int attaquant, int idDepart, int idArrivee, int toursRestants, int arg5) {
        _attaquant = (Proprietaire)attaquant;
        _origine = graph.getUsine(idDepart);
        if(_attaquant == MOI)
            _arrivee = graph.getUsine(idArrivee);
        else
            _arrivee = nullptr;
        _toursRestants = toursRestants;
    }

    Proprietaire _attaquant;
    shared_ptr<Usine> _origine;
    shared_ptr<Usine> _arrivee; // = nullptr si c'est ADVERSAIRE qui l'envoie
    int _toursRestants;         // = -1      si c'est ADVERSAIRE qui l'envoie
};

/**
 * Graph.cpp
 */

void Graph::init() {
    //cerr << "Graph::init\n";
    // Pour chaque usine, on ordonne les autres selon leur distance
    for(auto usine : _usines) {
        vector<shared_ptr<Usine>> v; // représente l'enchaînement des autres usines de la plus près à la plus loin

        for(auto another : _usines)
            if(usine != another)
                v.push_back(another);

        sort(begin(v), end(v), [usine](const shared_ptr<Usine> usine1, const shared_ptr<Usine> usine2)
        { return usine1->getDistance(usine) < usine2->getDistance(usine); });

        _eloignement.push_back(v);
    }
}

int Graph::getNbrCyborgsTotal(Proprietaire prop) const {
    int result = 0;
    auto usines = getListeUsines(prop);
    for(auto usine : usines)
        result += usine->getNbrCyborgs();
    return result;
}

void Graph::enleverCyborgs(shared_ptr<Usine> usine, int nbr) {
    int nbrRestants = usine->getNbrCyborgs() - nbr;
    if(nbrRestants < 0) nbrRestants = 0;
    usine->setNbrCyborgs(nbrRestants);
}

shared_ptr<Usine> Graph::getUsineMaxCyborgs(Proprietaire prop, int productionMin) const {
    //cerr << "Graph::getUsineMaxCyborgs\n";
    int max = 0;
    shared_ptr<Usine> result = nullptr;
    switch(prop) {
        case AUTRES_QUE_MOI:
            for(auto usine : _usines) {
                if(usine->getProprietaire() != MOI && usine->getNbrCyborgs() > max
                   && (productionMin == -1 || usine->getProduction() == productionMin)) {
                    max = usine->getNbrCyborgs();
                    result = usine;
                }
            }
            break;

        default:
            for(auto usine : _usines) {
                if(usine->getProprietaire() == prop && usine->getNbrCyborgs() > max
                   && (productionMin == -1 || usine->getProduction() == productionMin)) {
                    max = usine->getNbrCyborgs();
                    result = usine;
                }
            }
            break;
    }
    return result;
}

vector<shared_ptr<Usine>> Graph::getListeUsines(Proprietaire prop) const {
    //cerr << "Graph::getListeUsines\n";
    vector<shared_ptr<Usine>> result;
    switch(prop) {
        case AUTRES_QUE_MOI:
            for(auto usine : _usines)
                if(usine->getProprietaire() != MOI)
                    result.push_back(usine);
            break;

        default:
            for(auto usine : _usines)
                if(usine->getProprietaire() == prop)
                    result.push_back(usine);
            break;
    }
    // Les usines avec le moins de cyborgs sont à la fin
    sort(begin(result), end(result), [](const shared_ptr<Usine> u1, const shared_ptr<Usine> u2)
    {
        /*// En ayant 1 de prod de moins, c'est bcp plus accessible avec le nbr : on prend!
        if(u1->getProduction() - 1 == u2->getProduction() && u1->getNbrCyborgs() > u2->getNbrCyborgs() / 3)
            return true;*/
        // Si la production est meilleure, c'est plus intéressant
        if(u1->getProduction() < u2->getProduction())
            return true;
        // Si c'est la même production, l'endroit avec le moins de cyborgs est plus intéressant
        if(u1->getProduction() == u2->getProduction())
            return u1->getNbrCyborgs() > u2->getNbrCyborgs();
        // La m est moindre : c'est moins bien
        return false; });
    return result;
}

vector<shared_ptr<Usine>> Graph::getUsinesSelonProduction(int production, Proprietaire prop) const {
    //cerr << "Graph::getUsinesSelonProduction\n";
    vector<shared_ptr<Usine>> result;
    switch(prop) {
        case AUTRES_QUE_MOI:
            for(auto usine : _usines)
                if(usine->getProprietaire() != MOI && usine->getProduction() == production)
                    result.push_back(usine);
            break;

        default:
            for(auto usine : _usines)
                if(usine->getProprietaire() == prop && usine->getProduction() == production)
                    result.push_back(usine);
            break;
    }
    sort(begin(result), end(result), [](const shared_ptr<Usine> u1, const shared_ptr<Usine> u2)
                                     { return u1->getNbrCyborgs() < u2->getNbrCyborgs(); });
    return result;
}

vector<shared_ptr<Usine>> Graph::getUsinesSiProduction(bool produit, Proprietaire prop) const {
    //cerr << "Graph::getUsinesSiProduction\n";
    vector<shared_ptr<Usine>> result;
    switch(prop) {
        case AUTRES_QUE_MOI:
            for(auto usine : _usines)
                if(usine->getProduction() != MOI && ((usine->getTempsAvantProduction() == 0) == produit))
                    result.push_back(usine);
            break;

        default:
            for(auto usine : _usines)
                if(usine->getProduction() == prop && ((usine->getTempsAvantProduction() == 0) == produit))
                    result.push_back(usine);
            break;
    }
    return result;
}

vector<shared_ptr<Usine>> Graph::getListeUsinesOrdreDistance(shared_ptr<Usine> reference, Proprietaire prop) const {
    vector<shared_ptr<Usine>> result;
    for(auto usine : _eloignement[reference->getID()])
        if(usine->getProprietaire() == prop)
            result.push_back(usine);
    //reverse(begin(result), end(result));
    return result;
}

vector<shared_ptr<Usine>> Graph::getUsinesPouvantUpgrader(int productionMaxActuelle, int nbr_cyborgs_restants) const {
    vector<shared_ptr<Usine>> result;
    for(auto usine : getListeUsines(MOI))
        if(usine->getNbrCyborgs() >= 10 + nbr_cyborgs_restants && usine->getProduction() <= productionMaxActuelle)
            result.push_back(usine);
    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Strategie.h
 */

enum Act {
    MOVE,
    SEND_BOMB,
    INC,
    WAIT,

    MSG
};

struct Action {
    Action(shared_ptr<Usine> source, shared_ptr<Usine> cible, int nbr_cyborg) { // MOVE
        _action = MOVE;
        _source = source;
        _cible = cible;
        _nbr_cyborgs = nbr_cyborg;
    }
    Action(shared_ptr<Usine> source, shared_ptr<Usine> cible) { // BOMB
        _action = SEND_BOMB;
        _source = source;
        _cible = cible;
    }
    Action(shared_ptr<Usine> source) { // INC
        _action = INC;
        _source = source;
    }
    Action() { // WAIT
        _action = WAIT;
    }
    Action(string message) { // MSG
        _action = MSG;
        _message = message;
    }

    friend ostream& operator<<(ostream& os, Action& action) {
        switch(action._action) {
            case MOVE:
                cout << "MOVE " << action._source->getID() << ' ' << action._cible->getID() << ' ' << action._nbr_cyborgs;
                break;
            case SEND_BOMB:
                cout << "BOMB " << action._source->getID() << ' ' << action._cible->getID();
                break;
            case INC:
                cout << "INC " << action._source->getID();
                break;
            case WAIT:
                cout << "WAIT";
                break;
            case MSG:
                cout << "MSG " << action._message;
                break;
        }
        return os;
    }

    inline Act getAction() { return _action; }

    inline bool estValide() { return _source->getProprietaire() == MOI; }

private:
    Act _action;
    shared_ptr<Usine> _source = nullptr;
    shared_ptr<Usine> _cible = nullptr;
    int _nbr_cyborgs;
    string _message = "";
};

class Strategie {
public:
    Strategie();

    // Récupère les nouvelles données du jeu
    void update();

    // Si on a planifié une action pour ce tour : on la mets en priorité
    void checkActionsEnAttente();

    // Renvoie true si on a envoyé une bombe sur une usine adverse de prod min avec un certain nombre de cyborgs
    bool envoyerBombe(int productionMinAdversaire = 3, int nbrMinCyborgs = 1);

    // Renvoie true tant qu'on peut faire d'autres actions
    void envoyerNouvellesTroupes();

    void increaseUsines(int nbrTotalCyborgsMin);

    void lancerActions(bool plusieursActions = true, bool bombes = true, bool increase = true);

    void textDebug() {
        _graph.afficherMessageDebug("liste des usines de MOI", _graph.getListeUsines(MOI));
        _graph.afficherMessageDebug("liste des usines de ADVERSAIRE", _graph.getListeUsines(ADVERSAIRE));
        _graph.afficherMessageDebug("liste des usines de NEUTRE", _graph.getListeUsines(NEUTRE));
        _graph.afficherMessageDebug("liste des usines de AUTRES_QUE_MOI", _graph.getListeUsines(AUTRES_QUE_MOI));

        _graph.afficherMessageDebug("usines de AUTRES_QUE_MOI de prod 1", _graph.getUsinesSelonProduction(1));

        _graph.afficherMessageDebug("usines depuis 1", _graph.getListeUsinesOrdreDistance(_graph.getUsine(1), ADVERSAIRE));

        // TODO : à tester (plus les autres, j'ai pas eu le temps de les marquer)
        _graph.afficherMessageDebug("usines qui peuvent up", _graph.getUsinesPouvantUpgrader());
        _graph.afficherMessageDebug("usines de l'adversaire qui sont arretees", _graph.getUsinesSiProduction(false, ADVERSAIRE));
    }

private:
    int _tourDeJeu = 0;
    int _bombesRestantes = 2;

    chrono::system_clock::time_point start;

    Graph _graph;
    vector<Troupe> _troupes;
    vector<Bombe>  _bombes;

    vector<Action> _actions;
    vector<pair<int, Action>> _actionsEnAttente;
    // Permet d'éviter d'attaquer tous les tours la même usine
    map<shared_ptr<Usine>, int> _usinesDejaSousAttaque;


    // Renvoie l'usine sur laquelle on a déjà envoyé une bombe (nullptr si aucune bombe n'est lancée)
    shared_ptr<Usine> getUsineBombeEnvoyee();
    // Permet de savoir si l'adversaire nous envoie une bombe
    int getNbrUsinesMenaceesParBombe();
    inline int getNbrBombesRestantes() { return _bombesRestantes; };
    vector<Troupe> getTroupesAllantSur(shared_ptr<Usine> cible, Proprietaire attaquant);

    void lancerTroupes(shared_ptr<Usine> usineAPrendre);

    // On garde un certain nombre de cyborgs dans chaque usine
    void protegerUsines();

    int getTempsRestant(); // en millisecondes
};

/**
 * Strategie.cpp
 */

Strategie::Strategie() {
    int factoryCount; // the number of factories
    cin >> factoryCount;
    cin.ignore();

    _graph.setNbrUsines(factoryCount);

    int linkCount; // the number of links between factories
    cin >> linkCount;
    cin.ignore();

    for (int i = 0; i < linkCount; i++) {
        int factory1;
        int factory2;
        int distance;
        cin >> factory1 >> factory2 >> distance;
        cin.ignore();

        // Le graphe est bi-directionnel : on crée un lien dans les 2 sens
        _graph.addEdge(factory1, factory2, distance);
        _graph.addEdge(factory2, factory1, distance);
    }

    // On mets à jour les correspondances entre les usines
    _graph.init();
}

void Strategie::update() {
    start = std::chrono::system_clock::now();

    int entityCount; // the number of entities (e.g. factories and troops)
    cin >> entityCount;
    cin.ignore();

    _troupes.clear();
    _bombes.clear();
    _actions.clear();

    for(int i = 0 ; i < entityCount ; i++) {
        int entityId;
        string entityType;
        int arg1;
        int arg2;
        int arg3;
        int arg4;
        int arg5;
        cin >> entityId >> entityType >> arg1 >> arg2 >> arg3 >> arg4 >> arg5;
        cin.ignore();

        if (entityType == "FACTORY") {
            _graph.updateUsine(entityId, arg1, arg2, arg3, arg4, arg5);
        } else if (entityType == "TROOP") {
            _troupes.push_back(Troupe(_graph, arg1, arg2, arg3, arg4, arg5));
        } else if (entityType == "BOMB") {
            _bombes.push_back(Bombe(_graph, arg1, arg2, arg3, arg4, arg5));
        }

        //textDebug();
    }

    //protegerUsines();

    _tourDeJeu += 2;

    // On regarde si des usines perdent leur protection
    map<shared_ptr<Usine>, int>::iterator it;
    for(it = _usinesDejaSousAttaque.begin() ; it != _usinesDejaSousAttaque.end() ; ++it)
        if(it->second < _tourDeJeu)
            _usinesDejaSousAttaque.erase(it);
}

shared_ptr<Usine> Strategie::getUsineBombeEnvoyee() {
    if(_bombesRestantes < 2)
        for(auto bombe : _bombes)
            if(bombe._attaquant == MOI)
                return bombe._arrivee;
    return nullptr;
}

void Strategie::checkActionsEnAttente() {
    sort(_actionsEnAttente.begin(), _actionsEnAttente.end(), [](const pair<int, Action>& a1, const pair<int, Action>& a2)
                                                        { return a1.first > a2.first; });
    for(int i = (int)_actionsEnAttente.size() - 1 ; i >= 0 ; i--) {
        auto act = _actionsEnAttente[i];
        if(act.first <= _tourDeJeu) {
            if(act.second.estValide())
                _actions.push_back(act.second);
            else
                _actionsEnAttente.pop_back();
        } else
            break;
    }
}

bool Strategie::envoyerBombe(int productionMinAdversaire, int nbrMinCyborgs) {
    if(getNbrBombesRestantes() > 0) {
        shared_ptr<Usine> usineAdverse = _graph.getUsineMaxCyborgs(ADVERSAIRE, productionMinAdversaire);
        // Si une usine adverse est intéressante à attaquer, alors let's go!
        if(usineAdverse != nullptr && usineAdverse != getUsineBombeEnvoyee()
           && usineAdverse->getNbrCyborgs() > nbrMinCyborgs)
        {
            // Au cas où d'autres actions doivent être faites
            /*for(auto action : _actions)
                _actionsEnAttente.push_back(make_pair(_tourDeJeu + 2, action));
            _actions.clear();*/

            auto mesUsinesProches = _graph.getListeUsinesOrdreDistance(usineAdverse, MOI);
            if(!mesUsinesProches.empty()) {
                auto monUsineProche = mesUsinesProches[0];
                _actions.push_back(Action(monUsineProche, usineAdverse)); // BOMB
                _bombesRestantes--;

                // On envoie des troupes juste après
                int nbrCyborgs = 2 + usineAdverse->getDistance(monUsineProche) * usineAdverse->getProduction() +
                                 usineAdverse->getNbrCyborgs();
                _actionsEnAttente.push_back(
                        make_pair(_tourDeJeu + 2, Action(monUsineProche, usineAdverse, nbrCyborgs))); // MOVE
                _usinesDejaSousAttaque[usineAdverse] = _tourDeJeu + monUsineProche->getDistance(usineAdverse);// + 2;
            }

            return true;
        }
    }
    return false;
}

void Strategie::envoyerNouvellesTroupes() {
    /*if(_tourDeJeu == 0) {
        auto usines = _graph.getListeUsines(AUTRES_QUE_MOI);
        auto monUsine = _graph.getListeUsines(MOI)[0];
        sort(usines.begin(), usines.end(), [monUsine](const shared_ptr<Usine> u1, const shared_ptr<Usine> u2)
        { if(u1->getProprietaire() == ADVERSAIRE)
            return false;
            return u1->getNbrCyborgs() + (4 - u1->getProprietaire()) * u1->getDistance(monUsine)
               < u2->getNbrCyborgs() + (4 - u2->getProprietaire()) * u2->getDistance(monUsine); });

        for(auto usineAdverse : usines)
            _actions.push_back(Action(monUsine, usineAdverse, usineAdverse->getNbrCyborgs() + 2));
        return;
    }*/
    protegerUsines();

    auto usinesAdverses = _graph.getListeUsines(AUTRES_QUE_MOI);

    /// On trie l'ensemble des usines adverses en fonction de la rentabilité de l'entreprise
    /// Selon le nbr de cyborgs, la production, la distance depuis chacune de mes usines, ...
    sort(begin(usinesAdverses), end(usinesAdverses), [this](const shared_ptr<Usine> u1, const shared_ptr<Usine> u2)
    {
        /// DEBUG ///
        if(u1->getProduction() == u2->getProduction())
            return u1->getNbrCyborgs() < u2->getProduction();
        return u1->getProduction() < u2->getProduction();

        int distanceActuelle1 = 0;

        // On calcule le nombre de cyborgs perdus pour prendre chaque usine
        int rentabiliteUsine1 = 0;//numeric_limits<int>::max();
        int rentabiliteUsine2 = 0;

        bool pris = false;
        // On calcule combien de cyborgs on doit envoyer pour prendre l'usine 1
        int nbrCyborgsEnnemi = u1->getNbrCyborgs();
        vector<shared_ptr<Usine>> mesUsines = _graph.getListeUsinesOrdreDistance(u1, MOI);
        for(auto usine : mesUsines) {
            if(u1->getProprietaire() == ADVERSAIRE) {
                distanceActuelle1 = u1->getDistance(usine) - distanceActuelle1 + 1;
                nbrCyborgsEnnemi += distanceActuelle1 * u1->getProduction();
            }
            // On mets à jour le nombre de cyborgs que possède l'ennemi
            int nbrCyborgsEnvoyes = min(nbrCyborgsEnnemi, max(0, usine->getNbrCyborgs() - nbrCyborgsEnnemi)) + 1;
            if(nbrCyborgsEnvoyes < 0) nbrCyborgsEnvoyes = 0;
            if(nbrCyborgsEnvoyes > usine->getNbrCyborgs()) nbrCyborgsEnvoyes = usine->getNbrCyborgs();
            rentabiliteUsine1 += nbrCyborgsEnvoyes;

            if(nbrCyborgsEnnemi < 0) {
                pris = true;
                break;
            }
        }
        // On ne peut pas prendre l'usine : c'est pas super rentable
        if(!pris)
            rentabiliteUsine1 += 10;

        // On fait de même pour l'usine 2
        int distanceActuelle2 = 0;
        pris = false;
        nbrCyborgsEnnemi = u2->getNbrCyborgs();
        mesUsines = _graph.getListeUsinesOrdreDistance(u2, MOI);
        for(auto usine : mesUsines) {
            if(u2->getProprietaire() == ADVERSAIRE) {
                distanceActuelle2 = u2->getDistance(usine) - distanceActuelle2 + 1;
                nbrCyborgsEnnemi += distanceActuelle2 * u2->getProduction();
            }

            int nbrCyborgsEnvoyes = min(nbrCyborgsEnnemi, max(0, usine->getNbrCyborgs() - nbrCyborgsEnnemi)) + 1;
            if(nbrCyborgsEnvoyes < 0) nbrCyborgsEnvoyes = 0;
            if(nbrCyborgsEnvoyes > usine->getNbrCyborgs()) nbrCyborgsEnvoyes = usine->getNbrCyborgs();
            rentabiliteUsine2 += nbrCyborgsEnvoyes;

            if(nbrCyborgsEnnemi < 0) {
                pris = true;
                break;
            }
        }
        if(!pris)
            rentabiliteUsine2 += 10;

        /// On envoie l'usine qui est la plus rentable à prendre (en fonction de leur production)
        // La plus rentable est à la fin (permet de faire des pop_back()
        if(rentabiliteUsine1 == rentabiliteUsine2)
            return distanceActuelle1 < distanceActuelle2;
        return rentabiliteUsine1 * (4 - u1->getProduction()) < rentabiliteUsine2 * (4 - u2->getProduction()); });


    cerr << "Liste des usines que je peux attaquer :\n";
    for(auto usine : usinesAdverses) {
        cerr << usine->getID() << " : " << usine->getProduction() << " de prod ; " << usine->getNbrCyborgs() << " cyborgs ; propriétaire : " << usine->getProprietaire() << endl;
    } cerr << endl;

    while(getTempsRestant() > 2 && !usinesAdverses.empty()) {
        auto usineConvoitee = usinesAdverses[usinesAdverses.size() - 1];
        // Si on attaque déjà l'usine, on n'envoie pas plus de troupe avant le tourDeJeu décidé
        map<shared_ptr<Usine>, int>::iterator it = _usinesDejaSousAttaque.find(usineConvoitee);
        if (it == _usinesDejaSousAttaque.end()) {
            cerr << "* Tentative d'attaque de l'usine " << usineConvoitee->getID() << " :  " << usineConvoitee->getNbrCyborgs() << " cyborgs.\n";

            int nbrCyborgsEnnemi = usineConvoitee->getNbrCyborgs();
            // On attaque si on a assez de troupes pour
            if(nbrCyborgsEnnemi + 5 < _graph.getNbrCyborgsTotal(MOI)) {
                auto mesUsines = _graph.getListeUsinesOrdreDistance(usineConvoitee, MOI);
                int distanceActuelle = 0;
                bool temp = true;
                for (auto usine : mesUsines) {
                    if (temp) {
                        // On n'attaque plus l'usine avant que les premières troupes n'arrivent
                        _usinesDejaSousAttaque[usineConvoitee] = _tourDeJeu + 2 * usineConvoitee->getDistance(usine);
                        temp = false;
                    }
                    if (usineConvoitee->getProprietaire() == ADVERSAIRE) {
                        distanceActuelle = usineConvoitee->getDistance(usine) - distanceActuelle + 1;
                        nbrCyborgsEnnemi += distanceActuelle * usineConvoitee->getProduction();
                    }
                    cerr << "cyborgs ennemis : " << nbrCyborgsEnnemi << " ; envoyes : ";

                    int nbrCyborgsEnvoyes =
                            min(nbrCyborgsEnnemi, max(0, usine->getNbrCyborgs() - nbrCyborgsEnnemi)) + 2;
                    if (nbrCyborgsEnvoyes < 0) nbrCyborgsEnvoyes = 0;
                    if (nbrCyborgsEnvoyes > usine->getNbrCyborgs()) nbrCyborgsEnvoyes = usine->getNbrCyborgs();
                    cerr << nbrCyborgsEnvoyes << endl;
                    nbrCyborgsEnnemi -= nbrCyborgsEnvoyes;
                    _graph.enleverCyborgs(usine, nbrCyborgsEnvoyes);

                    _actions.push_back(Action(usine, usineConvoitee, nbrCyborgsEnvoyes)); // MOVE

                    if (nbrCyborgsEnnemi < 0) {
                        break;
                    }
                }
            }
        }
        usinesAdverses.pop_back();
    }
}

void Strategie::lancerTroupes(shared_ptr<Usine> usineAPrendre) {
    //int prod = usineAPrendre->getProduction();
    //cerr << "prod = " << prod << endl;
    // il faut MULTIPLICATEUR troupes de plus avant de considérer qu'attaquer est rentable
    const double multiplicateur = 1.0;//(prod == 3 ? 1.0 : (prod == 2 ? 0.9 : (prod == 1 ? 0.75 : 0.65)));
    auto usinesProches = _graph.getListeUsinesOrdreDistance(usineAPrendre, MOI);
    int nbrCyborgsAdverse = usineAPrendre->getNbrCyborgs() + 2;
    bool peutPrendre = false;
    vector<int> nbrCyborgsSuccessifs;
    int distanceActuelle = 0;
    for(int i = 0 ; i < usinesProches.size() ; i++) {
        distanceActuelle = usineAPrendre->getDistance(usinesProches[i]) - distanceActuelle;
        nbrCyborgsAdverse += distanceActuelle * usineAPrendre->getProduction();
        int nbrCyborgsEnvoyes = (int)(multiplicateur * (usinesProches[i]->getNbrCyborgs() - nbrCyborgsAdverse + 1));
        if(nbrCyborgsEnvoyes < 0) nbrCyborgsEnvoyes = 0;
        nbrCyborgsAdverse -= nbrCyborgsEnvoyes;
        nbrCyborgsSuccessifs.push_back(nbrCyborgsEnvoyes);

        if(nbrCyborgsAdverse < 0) {
            peutPrendre = true;
            // On n'attaque plus l'usine avant que les premières troupes n'arrivent
            _usinesDejaSousAttaque[usineAPrendre] = _tourDeJeu + 2 * usineAPrendre->getDistance(usinesProches[0]);
            break;
        }
    }
    // En mettant en commun toutes les usines, on peut prendre une usine
    if(peutPrendre) {
        for(int i = 0 ; i < nbrCyborgsSuccessifs.size() ; i++) {
            _actions.push_back(Action(usinesProches[i], usineAPrendre, nbrCyborgsSuccessifs[i])); // MOVE
            _graph.enleverCyborgs(usinesProches[i], nbrCyborgsSuccessifs[i]);
        }
    }
}

void Strategie::increaseUsines(int nbrTotalCyborgsMin) {
    int nbrTotalCyborgs = _graph.getNbrCyborgsTotal(MOI);
    if(nbrTotalCyborgs > nbrTotalCyborgsMin && nbrTotalCyborgs > 0.8 * _graph.getNbrCyborgsTotal(ADVERSAIRE)) {
        auto usines = _graph.getUsinesPouvantUpgrader(1); // On upgrade des usines de niveau 0 ou 1
        // Pour chaque usine suscpetible de monter en niveau, on vérifie qu'elle ait assez de cyborgs pour qu'on la garde
        for(auto usine : usines) {
            bool autorisationInc = true;
            int nbrCyborgs = usine->getNbrCyborgs() - 10;

            auto troupes = getTroupesAllantSur(usine, ADVERSAIRE);
            // On simule le nombre de cyborgs qu'aurait notre usine en upgradant ; si négatif : on abandonne le INC
            for(auto troupe : troupes) {
                nbrCyborgs += (troupe._nbr_tours_restants - 1) * (usine->getProduction() + 1) - troupe._nbr_cyborgs;
                if(nbrCyborgs <= 0) {
                    autorisationInc = false;
                    break;
                }
            }

            if(autorisationInc) {
                _actions.push_back(Action(usine)); // INC
                _graph.enleverCyborgs(usine, 10);
                nbrTotalCyborgs -= 10;
            }

            if(nbrTotalCyborgs <= nbrTotalCyborgsMin)
                break;
        }
    }
}

void Strategie::protegerUsines() {
    const int NBR_MIN_CYBORGS_USINE_PROD_3 = 6;
    const int NBR_MIN_CYBORGS_USINE_PROD_2 = 3;
    const int NBR_MIN_CYBORGS_USINE_PROD_1 = 2;
    const int NBR_MIN_CYBORGS_USINE_PROD_0 = 0;

    auto usines = _graph.getUsinesSelonProduction(3, MOI);
    for(auto usine : usines)
        _graph.enleverCyborgs(usine, NBR_MIN_CYBORGS_USINE_PROD_3);

    usines = _graph.getUsinesSelonProduction(2, MOI);
    for(auto usine : usines)
        _graph.enleverCyborgs(usine, NBR_MIN_CYBORGS_USINE_PROD_2);

    usines = _graph.getUsinesSelonProduction(1, MOI);
    for(auto usine : usines)
        _graph.enleverCyborgs(usine, NBR_MIN_CYBORGS_USINE_PROD_1);

    usines = _graph.getUsinesSelonProduction(0, MOI);
    for(auto usine : usines)
        _graph.enleverCyborgs(usine, NBR_MIN_CYBORGS_USINE_PROD_0);
}

int Strategie::getNbrUsinesMenaceesParBombe() {
    int nbr = 0;
    for(auto bombe : _bombes)
        if(bombe._attaquant == ADVERSAIRE)
            nbr++;
    return nbr;
}

vector<Troupe> Strategie::getTroupesAllantSur(shared_ptr<Usine> cible, Proprietaire attaquant) {
    vector<Troupe> result;
    for(auto troupe : _troupes)
        if(troupe._arrivee == cible && troupe._proprietaire == attaquant)
            result.push_back(troupe);
    sort(begin(result), end(result), [](const Troupe& t1, const Troupe& t2)
    { return t1._nbr_tours_restants < t2._nbr_tours_restants; });
    return result;
}

void Strategie::lancerActions(bool plusieursActions, bool bombes, bool increase) {
    //sort(begin(_actions), end(_actions));
    //_actions.erase(unique(begin(_actions), end(_actions)), end(_actions));

    if(plusieursActions) {
        cout << "WAIT";

        for(auto action : _actions)
            if((bombes || action.getAction() != SEND_BOMB) && (increase || action.getAction() != INC))
                cout << ";" << action;
    } else {
        for(auto action : _actions) {
            if(action.getAction() == MOVE) {
                cout << action << endl;
                return;
            }
        }
        cout << "WAIT";
    }
    cout << endl;
}

int Strategie::getTempsRestant() {
    return (_tourDeJeu > 0 ? 50 : 1000) - (int)chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


int main() {
    Strategie strat;

    while(true) {
        strat.update();
        strat.checkActionsEnAttente();
        //if(!strat.envoyerBombe(3, 5)) {
            strat.increaseUsines(50);
            strat.envoyerNouvellesTroupes();
        //}
        strat.lancerActions(PLUSIEURS_ACTIONS, BOMBES, INCREASE);
    }
}