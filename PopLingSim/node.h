#ifndef defNode
#define deNode

#include <vector>
#include <string>

class node
 {

    /// METHODES ///

    public:

    node() ; // Constructeur par défault
    node(std::vector<double> uParametreMutation, std::vector<double> uParametreEmprunt) ; // Constructeur évolué
    void mutation() ;
    void empruntLocal(node) ;
    void empruntGlobal(std::vector<node>, int) ;
    void empruntMassif(node, double) ;
    void miseAJour() ;
    void nouvelArbre(std::string) ;
    void nouvelleTailleRandom(int) ;
    void nouvelleTaille(int) ;
    void nouveauTauxMutation(std::vector<double> mutationsL) ;

    std::vector<int> getData() ;
    int getDataU(int) ;
    int getTailleBranche() ;
    std::string getSousArbre() ;
    std::vector<double> getTauxMutation() ;
    double getTauxMutationMoyen() ;
    std::vector<double> getTauxEmprunt() ;
    double getTauxEmpruntMoyen() ;
    /* A faire :
    void ecriture() ;
    */


    /// ATTRIBUTS ///

    private:

    // node **brother ; // Liste de pointeurs vers les noeuds frères
    int nbSites ;
    int tailleBranche ;
	std::string sousArbre ; // Sous-arbre donc node est la branche mère. Egalement identité du noeud.
    std::vector<double> parametreMutation ; // Paramètre de mutation
    std::vector<double> parametreEmprunt ; // Paramètre d'emprunt
    std::vector<int> data ; // Valeurs des n sites
    std::vector<double> tauxMutation ; // Taux de mutation de chacun des n sites
	std::vector<double> tauxEmprunt ; // Taux d'emprunt de chacun des n sites

} ;

#endif
