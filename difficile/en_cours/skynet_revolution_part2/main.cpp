#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include <cassert>

using namespace std;

template <class T> class Matrice {
protected:
    vector<std::vector<T> > data;
public:
    Matrice(){}

    Matrice(size_t nb_lig, size_t nb_col, T init = 0){
        for(size_t lig = 0 ; lig < nb_lig ; ++lig){
            vector<T> tmp;
            tmp.reserve(nb_col);
            for(size_t col = 0 ; col < nb_col ; ++col){
                tmp.push_back(init);
            }
            data.push_back(tmp);
        }
    }

    ~Matrice(){}

    /// Nombre de lignes
    inline size_t size1() const{
        return data.size();
    }

    /// Nombre de colonnes
    inline size_t size2() const{
        if (data.size() == 0) return 0;
        return data[0].size();
    }

    /**
     * \brief Accesseur vers un element de la matrice
     * \param lig son numero de ligne
     * \param col son numero de colonne
     * \return la valeur stockee
     */
    inline T get(size_t lig, size_t col) const{
        assert(lig<size1());
        assert(col<size2());
        return data[lig][col];
    }

    /**
     * \brief Accesseur vers un element de la matrice
     * \param lig son numero de ligne
     * \param col son numero de colonne
     * \param val la valeur a stocker
     */
    inline void set(size_t lig, size_t col, T val){
        assert(lig<size1());
        assert(col<size2());
        data[lig][col]=val;
    }

    Matrice<T> pow(const Matrice<T> & m, size_t exp) const {
        assert(m.size1() == m.size2());
        assert(exp > 0);
        if(exp == 1)
            return m;
        return pow(m, exp - 1) * m;
    }

    void set_identite() {
        assert(size1() == size2());
        for(size_t lig = 0 ; lig < size1() ; ++lig) {
            for(size_t col = 0 ; col < size2() ; ++col) {
                if(lig == col)
                    set(lig, col, 1);
                else
                    set(lig, col, 0);
            }
        }
    }
};

template <class T>
ostream& operator<<(ostream& os, const Matrice<T> & m){
    for(size_t lig = 0 ; lig < m.size1() ; ++lig){
        for(size_t col = 0 ; col < m.size2() ; ++col){
            os << m.get(lig, col) << " ";
        }
        os << std::endl;
    }
    return os;
}

template <class T>
Matrice<T> operator+(const Matrice<T> & m1, const Matrice<T> & m2){
    assert(m1.size1() == m1.size1());
    assert(m1.size2() == m1.size2());
    size_t nb_lig = m1.size1();
    size_t nb_col = m1.size2();
    Matrice<T> sum(nb_lig, nb_col);
    for(size_t lig = 0 ; lig < nb_lig ; ++lig){
        for(size_t col = 0 ; col < nb_col ; ++col){
            sum.set(lig, col, m1.get(lig, col) + m2.get(lig, col));
        }
    }
    return sum;
}

template <class T>
Matrice<T> operator*(const Matrice<T> & m1, const Matrice<T> & m2){
    assert(m1.size2() == m2.size1());
    size_t nb_lig = m1.size1();
    size_t nb_col = m2.size2();
    Matrice<T> prod(nb_lig, nb_col);
    for(size_t lig = 0 ; lig < nb_lig ; ++lig){
        for(size_t col = 0 ; col < nb_col ; ++col){
            //calcul du terme (lig,col)
            T tmp = 0;
            for(size_t i = 0 ; i < m1.size2() ; ++i){
                tmp+=m1.get(lig, i) * m2.get(i, col);
            }
            prod.set(lig, col, tmp);
        }
    }
    return prod;
}

int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();

    Matrice<bool> matrix(N, N, false);

    for (int i = 0; i < L; i++) {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();
        matrix.set(N1, N2, 1);
        matrix.set(N2, N1, 1);
    }
    
    vector<int> passerelles;
    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
        passerelles.push_back(EI);
    }

    bool lien1Point2PasserellesRestant = true;
    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();

        int passerelleMenacee = -1;
        int point;

        for(int pass : passerelles) {
            /// Il existe un arc direct entre une passerelle et le virus
            if (matrix.get(SI, pass)) {
                passerelleMenacee = pass;
                point = SI;
                break;
            }
        }

        // On cherche le point le plus dangereux (celui qui a 2 portes de sorties le plus proche)
        if(passerelleMenacee == -1) {
            map<int, vector<int>> pointsReliesAPasserelles;
            for (int y = 0; y < N; y++) {
                vector<int> tmp;
                for (int pass : passerelles)
                    if (matrix.get(y, pass))
                        tmp.push_back(pass);
                pointsReliesAPasserelles[y] = tmp;
            }

            /// On regarde s'il reste des points reliés à 2 passerelles
            if (lien1Point2PasserellesRestant) {
                bool existe = false;
                map<int, vector<int>>::iterator it;
                for (it = pointsReliesAPasserelles.begin(); it != pointsReliesAPasserelles.end(); ++it) {
                    if (it->second.size() >= 2) {
                        existe = true;
                        break;
                    }
                }

                if (!existe)
                    lien1Point2PasserellesRestant = false;
            }

            bool trouve = false;
            Matrice<bool> mat_puissances = matrix;
            while (!trouve) {
                // On augmente la puissance de la matrice puis on regarde chaque passerelle
                // Si on a M⁵ et M[1][6] = true, alors il existe un lien 1-6 de distance d'au moins 5
                mat_puissances = mat_puissances * matrix;

                map<int, vector<int>>::iterator it;
                for (it = pointsReliesAPasserelles.begin(); it != pointsReliesAPasserelles.end(); ++it) {
                    // On cherche en priorité les points reliés à 2 passerelles les plus proches
                    if (lien1Point2PasserellesRestant && it->second.size() < 2 || it->second.empty())
                        continue;

                    /// On a trouvé le point le plus proche avec le bon nombre de passerelles reliées
                    if (mat_puissances.get(SI, it->first)) {
                        passerelleMenacee = it->second[0]; // Une passerelle
                        point = it->first;                 // Point le plus proche du virus
                        trouve = true;
                        break;
                    }
                }
            }
        }
        
        // On mets à jour le graphe (suppression d'un arc)
        matrix.set(passerelleMenacee, point, false);
        matrix.set(point, passerelleMenacee, false);

        cout << passerelleMenacee << ' ' << point << endl;
    }
}
