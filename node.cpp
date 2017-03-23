#include<iostream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include <random>

#include "node.h"

int seed = time(0) ;
std::default_random_engine engine ;
std::uniform_real_distribution<double> randomU(0.0, 1.0) ;
std::uniform_int_distribution<int> randomInt(0, 100000) ;
std::gamma_distribution<double> randomG(2.0,2.0) ;


///___ CONSTRUCTEURS ___///

// Constructeur par défault
node::node() : nbSites(185), tailleBranche(0), sousArbre("Vide"), parametreMutation(0),  parametreEmprunt (0)
{
    // Loi déterminant la répartion des taux de mutation et d'emprunt
    for (int i = 0; i < nbSites; i++)
    {
        data.push_back(0) ;
        tauxMutation.push_back(0) ;
        tauxEmprunt.push_back(0) ;
    }
}

// Constructeur d'un noeud indépendant des autres
node::node(std::vector<double> uParametreMutation, std::vector<double> uParametreEmprunt) : nbSites(185), tailleBranche(0), sousArbre("Vide"), parametreMutation(uParametreMutation), parametreEmprunt(uParametreEmprunt)
{
    // Loi déterminant la répartion des taux de mutation et d'emprunt
    for (int i = 0; i < nbSites; i++)
    {
        data.push_back(randomInt(engine)) ;
    }

    tauxEmprunt = parametreEmprunt ;
    tauxMutation = parametreMutation ;
}



///___ METHODES ___///

void node::mutation()
{
    for (int i = 0 ; i < nbSites; i++)
    {
        if (tauxMutation[i] > randomU(engine))
        {
            data[i] = randomInt(engine) ;
        }
    }
}

void node::empruntLocal(node nodeVoisin)
{
    for (int i = 0; i < nbSites; i++)
    {
       if ((double) tauxEmprunt[i] > randomU(engine))
        {
            data[i] = nodeVoisin.getDataU(i) ;
        }
    }
}

void node::empruntGlobal(std::vector<node> nodes, int numeroNode)
{
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if (i != numeroNode)
        {
            for (int j = 0; j < nbSites; j++)
            {
               if ((double) tauxEmprunt[j]/(nodes.size() - 1) > randomU(engine))
                {
                    data[j] = nodes[i].getDataU(j) ;
                }
            }
        }
    }

}

void node::empruntMassif(node nodeVoisin, double proportionEmprunt)
{
    for (int i = 0; i < nbSites; i++)
    {
       if ((double) proportionEmprunt > randomU(engine))
        {
            data[i] = nodeVoisin.getDataU(i) ;
        }
    }
}

void node::miseAJour()
{
    tailleBranche-- ;
}

void node::nouvelArbre(std::string arbre)
{
    sousArbre = arbre ;
}

void node::nouvelleTailleRandom(int t)
{
    if (sousArbre.find(',') != std::string::npos) // Si la branche contient une virgule, cad si la branche n'est pas terminale
    {
        tailleBranche = (int) t*randomU(engine) ;
    }
    else // Sinon la branche est terminale
    {
        tailleBranche = t ;
    }
}

void node::nouvelleTaille(int t)
{
    tailleBranche = t ;
}

void node::nouveauTauxMutation(std::vector<double> mutationsL)
{
    for (int i = 0; i < nbSites; i++)
    {
        tauxMutation[i] = mutationsL[i] ;
    }
}



///___ METHODES GET ___///

int node::getDataU(int i)
{
    return data[i] ;
}

std::vector<int> node::getData()
{
    return data ;
}

int node::getTailleBranche()
{
    return tailleBranche ;
}

std::vector<double> node::getTauxMutation()
{
    return parametreMutation ;
}

double node::getTauxMutationMoyen()
{
    double tauxMutationMoyen = 0 ;
    for (unsigned int i = 0; i < tauxMutation.size(); i++)
    {
        tauxMutationMoyen = tauxMutationMoyen + tauxMutation[i] ;
    }
    tauxMutationMoyen = (double) tauxMutationMoyen / tauxMutation.size() ;
    return tauxMutationMoyen ;

}

std::vector<double> node::getTauxEmprunt()
{
    return tauxEmprunt ;
}

std::string node::getSousArbre()
{
    return sousArbre ;
}

double node::getTauxEmpruntMoyen()
{
    double tauxEmpruntMoyen = 0 ;
    for (unsigned int i = 0; i < tauxEmprunt.size(); i++)
    {
        tauxEmpruntMoyen = tauxEmpruntMoyen + tauxEmprunt[i] ;
    }
    tauxEmpruntMoyen = (double) tauxEmpruntMoyen / tauxEmprunt.size() ;
    return tauxEmpruntMoyen ;
}
