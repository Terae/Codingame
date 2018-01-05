#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

using namespace std;

class Personne;
// Permet de définir un lien entre 2 personnes
class Edge {
public:
    Edge(const shared_ptr<Personne> origine, const shared_ptr<Personne> arrivee) {
        _origine  = origine;
        _arrivee  = arrivee;
    }

    shared_ptr<Personne> getSuivant() { return  _arrivee; }

private:
    shared_ptr<Personne> _origine;
    shared_ptr<Personne> _arrivee;
};

/**
 * Définit une personne depuis son numéro
 */
class Personne : public std::enable_shared_from_this<Personne> {
public:
    Personne(int id) { _id = id; }

    void addEdge(const shared_ptr<Personne> another) {
        _edges.push_back(Edge(shared_from_this(), another));
        another->pasPremier();
    }

    inline int getID() const { return _id; }
    inline void pasPremier() { _estEnTete = false; }
    inline bool estPremier() { return _estEnTete; }

    vector<shared_ptr<Personne>> getPersonnesSuivantes() {
        vector<shared_ptr<Personne>> result;
        for(auto edge : _edges)
            result.push_back(edge.getSuivant());
        return result;
    }

private:
    int _id;
    bool _estEnTete = true;

    vector<Edge> _edges;
};

class Graph {
public:
    Graph() {}

    void addEdge(int origine, int arrivee) {
        ajouterPersonne(origine);
        ajouterPersonne(arrivee);
        _personnes[origine]->addEdge(_personnes[arrivee]);
    }

    vector<shared_ptr<Personne>> getListePremiers() {
        vector<shared_ptr<Personne>> result;
        for(auto it = _personnes.begin() ; it != _personnes.end() ; ++it)
            if(it->second->estPremier())
                result.push_back(it->second);
        return result;
    }

    int getLongueurMax(shared_ptr<Personne> personne) {
        if(personne == nullptr)
            return 0;
        int max = 0;
        for(auto suivant : personne->getPersonnesSuivantes()) {
            int dist = 1 + getLongueurMax(suivant);
            if(dist >= max)
                max = dist;
        }
        return max;
    }

private:
    map<int, shared_ptr<Personne>> _personnes;

    void ajouterPersonne(int id) {
        map<int, shared_ptr<Personne>>::iterator it;
        it = _personnes.find(id);
        if(it == _personnes.end())
            _personnes[id] = make_shared<Personne>(id);
    }
};

int main()
{
    int n; // the number of relationships of influence
    cin >> n; cin.ignore();
    Graph graph;
    for (int i = 0; i < n; i++) {
        int x; // a relationship of influence between two people (x influences y)
        int y;
        cin >> x >> y; cin.ignore();
        graph.addEdge(x, y);
    }

    int distanceMax = 0;
    for(auto personne : graph.getListePremiers()) {
        int dist = graph.getLongueurMax(personne);
        if(dist >= distanceMax)
            distanceMax = dist;
    }

    cout << 1 + distanceMax << endl;
}