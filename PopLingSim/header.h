#ifndef mainHeader
#define mainHeader

#include <vector>
#include <string>

// Fonctions du Main
std::vector<double> distributionParametre(double loi, double parametre, int nombre, std::default_random_engine &engine) ;
std::vector<std::string> splitArbre(std::string arbre) ;

// Fonctions de SumStats
std::vector<double> calculProportionsTotales(std::vector< std::vector<int> > tableauDonnees) ;
std::vector<double> calculProportionsSimilarite(std::vector<int> langue1, std::vector<int> langue2) ;
std::vector<double> calculNombreCognatsParSens(std::vector< std::vector<int> > tableauDonnees) ;
std::vector<double> calculStatistique(std::vector< std::vector<int> >) ;

#endif
