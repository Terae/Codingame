#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

using namespace std;

/// Définit une personne depuis son numéro
class Personne {
public:
    Personne(int id) { _id = id; }

    void addEdge(const shared_ptr<Personne> another) {
        _contacts.push_back(another);
    }

    inline int getID() const { return _id; }
    bool estDernier() { return _contacts.size() <= 1; }
    shared_ptr<Personne> getDernierContact() { return _contacts[0]; }

    void supprimerContact(shared_ptr<Personne> personne) {
        _contacts.erase(remove(_contacts.begin(), _contacts.end(), personne), _contacts.end());
    }

    vector<shared_ptr<Personne>> getContacts() { return _contacts; }

private:
    int _id;

    vector<shared_ptr<Personne>> _contacts;
};

class Graph {
public:
    Graph() {}

    void addEdge(int origine, int arrivee) {
        ajouterPersonne(origine);
        ajouterPersonne(arrivee);
        _personnes[origine]->addEdge(_personnes[arrivee]);
        _personnes[arrivee]->addEdge(_personnes[origine]);
    }

    int getNbrMin() {
        int result = 0;
        while(_personnes.size() > 1) {
            // On récupère l'ensemble des personnes en fin de liste
            vector<shared_ptr<Personne>> dernieres;
            for (auto it = _personnes.begin(); it != _personnes.end(); ++it)
                if (it->second->estDernier())
                    dernieres.push_back(it->second);

            // On les supprime
            for (auto personne : dernieres) {
                personne->getDernierContact()->supprimerContact(personne);
                _personnes.erase(personne->getID());
            }
            result++;
        }
        return result;
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
    cout << graph.getNbrMin() << endl;
}