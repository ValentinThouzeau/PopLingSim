#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <time.h>
#include <vector>
#include <random>
#include <chrono>

#include "header.h"
#include "node.h"



std::vector<double> distributionParametre(double parametre, double loi, int nombre, std::default_random_engine &engine)
{
    std::uniform_real_distribution<double> randomU(0.0, 1.0) ;
    std::vector<double> distribution ;

    if (loi == 2)
    {
        // Si       X --> Beta(alpha, beta)
        // Alors    E(X) = alpha / (alpha + beta)
        // Et       Mode(X) = (alpha - 1) / (alpha + beta - 2)
        //
        // Si       G1 --> Gamma(alpha, teta) et G2 --> Gamma(beta, teta)
        // Alors    G1 / (G1 + G2) --> Beta(alpha, beta)
        //
        // Soit mu, alpha et beta, X --> Beta(alpha, beta) et E(X) = mu
        // mu  = alpha/ (alpha+beta)  ==>  beta = alpha * (1-mu)/mu

        double alphaL1(50) ;
        double alphaL2(50) ;

        std::gamma_distribution<double> randomL1(alphaL1, 1.0) ;
        std::gamma_distribution<double> randomL2((double) alphaL2*(1 - parametre)/parametre, 1.0) ;

        for (int i = 0; i < nombre; i++)
        {
            distribution.push_back( (double) randomL1(engine) / (randomL1(engine) + randomL2(engine)) ) ;
        }

    }
    else if (loi == 1)
    {
        // Si       G --> Gamma(alpha, teta)
        // Alors    E(G) = alpha*teta
        //
        // Soit mu, alpha et teta, G --> Gamma(alpha, teta) et E(G) = mu
        // mu = alpha*teta  ==>  teta = mu/alpha
        double alphaL = 2.0 ;

        std::gamma_distribution<double> randomL1((double) alphaL, parametre / alphaL) ;


        for (int i = 0; i < nombre; i++)
        {
            double a = 0 ;
            do
            {
                a = (double) randomL1(engine) ;
            } while (a > 0.1) ;

            distribution.push_back(a) ;
        }

    }
    else
    {
        for (int i = 0; i < nombre; i++)
        {
            distribution.push_back(parametre) ;
        }
    }

    return distribution ;
}

std::vector<std::string> splitArbre(std::string arbre)
{

    std::vector<std::string> sousArbres ;
    std::string sousArbre ;
    int nombreParentheses = 0 ;

    for (unsigned int i = 1 ; i < arbre.size() ; i++)
    {
        // Décompte des parenthèses ouvrantes et fermantes
        if (arbre[i] == '(') nombreParentheses++ ;
        else if (arbre[i] == ')') nombreParentheses-- ;

        // Si la virgule est celle départageant deux sous-arbres, ou que l'on atteind la dernière parenthèse fermante
        if (((arbre[i] == ',') && (nombreParentheses == 0))||(nombreParentheses == - 1))
        {
            sousArbres.push_back(sousArbre) ; // On enregistre le sous-arbre dans sousArbres
            sousArbre = "" ; // On réinitialise sousArbre
        }
        else sousArbre = sousArbre + arbre[i] ; // Si aucun sous-arbre n'est délimité, on continue l'enregistrement
    }

    return sousArbres ;
}


std::vector<double> calculProportionSimilarite(std::vector<int> langue1, std::vector<int> langue2)
{
    std::vector<double> proportionsSimilarite ;
    int nombreDonneesManquantes(0) ;

    // Calcul de la proportion de similarités
    proportionsSimilarite.push_back(0) ;
    for (unsigned int i = 0; i < langue1.size(); i++)
    {
        if ((langue1[i] != -1)&&(langue2[i] != -1))
        {
            if (langue1[i] == langue2[i])
            {
                proportionsSimilarite[0] = proportionsSimilarite[0] + 1 ;
            }
        }
        else
        {
            nombreDonneesManquantes = nombreDonneesManquantes + 1 ;
        }

    }
    proportionsSimilarite[0] = (double) proportionsSimilarite[0] / (langue1.size() - nombreDonneesManquantes) ;

    // Calcul de la proportion de similarités d'ordre 2
    proportionsSimilarite.push_back(0) ;
    proportionsSimilarite[1] = pow(proportionsSimilarite[0], 2) ;

    return proportionsSimilarite ;
}


std::vector<double> calculProportionsTotales(std::vector< std::vector<int> > tableauDonnees)
{

    double proportionSimilariteTotal = 0 ;
    double proportionDissimilariteTotal = 0 ;
    int nombreDonneesManquantes(0) ;

    for (unsigned k = 0; k < tableauDonnees[0].size(); k++) // Pour chaque sens
    {
        double proportionSimilarite = 1 ;
        double proportionDissimilarite = 1 ;

        for (unsigned int i = 0; i < tableauDonnees.size(); i++) // Pour la langue i
        {
            for (unsigned int j = i + 1; j < tableauDonnees.size(); j++) // Pour la langue j
            {
                if ((tableauDonnees[i][k] != -1)&&(tableauDonnees[j][k] != -1))
                {
                    if (tableauDonnees[i][k] != tableauDonnees[j][k])
                    {
                        proportionSimilarite = 0 ;
                    }
                    if (tableauDonnees[i][k] == tableauDonnees[j][k])
                    {
                        proportionDissimilarite = 0 ;
                    }
                }
                else
                {
                    nombreDonneesManquantes = nombreDonneesManquantes + 1 ;
                }
            }
        }
        proportionSimilariteTotal = proportionSimilariteTotal + proportionSimilarite ;
        proportionDissimilariteTotal = proportionDissimilariteTotal + proportionDissimilarite ;
    }

    std::vector<double> proportionsTotales ;
    proportionsTotales.push_back((double) proportionSimilariteTotal/(tableauDonnees[0].size() - nombreDonneesManquantes)) ;
    proportionsTotales.push_back((double) proportionDissimilariteTotal/(tableauDonnees[0].size() - nombreDonneesManquantes)) ;

    return proportionsTotales ;
}


std::vector<double> calculStatsCognatsParSens(std::vector< std::vector<int> > tableauDonnees)
{
    std::vector<double> statsCognatsParSens ;
    std::vector<double> nombreCognatsParSens ;
    int testDonneeManquante(0) ;

    for (unsigned k = 0; k < tableauDonnees[0].size(); k++) // Pour chaque sens
    {
        testDonneeManquante = 0 ;
        std::vector<int> cognatsParSens ;

        for (unsigned int i = 0; i < tableauDonnees.size(); i++) // Pour les i langues
        {
            if (tableauDonnees[i][k] != -1) cognatsParSens.push_back(tableauDonnees[i][k]) ;
            else testDonneeManquante = 1 ;
        }

        if (testDonneeManquante == 0)
        {
            transform(cognatsParSens.begin(), cognatsParSens.end(), cognatsParSens.begin(), abs) ; // O(n) where n = distance(v.end(), v.begin())
            sort(cognatsParSens.begin(), cognatsParSens.end()); // Average case O(n log n), worst case O(n^2) (usually implemented as quicksort)
            auto uniqueEnd = unique(cognatsParSens.begin(), cognatsParSens.end()) ; // Again n comparisons
            nombreCognatsParSens.push_back(distance(cognatsParSens.begin(), uniqueEnd)) ; // Constant time for random access iterators (like vector's)
        }
    }

    // Moyenne du nombre de cognats par sens
    statsCognatsParSens.push_back(0) ;
    for (unsigned k = 0; k < nombreCognatsParSens.size(); k++) // Pour chaque sens
    {
        statsCognatsParSens[0] = statsCognatsParSens[0] + nombreCognatsParSens[k] ;
    }
    statsCognatsParSens[0] = (double) statsCognatsParSens[0] / nombreCognatsParSens.size() ;


    // Variance du nombre de cognats par sens
    statsCognatsParSens.push_back(0) ;
    for (unsigned k = 0; k < nombreCognatsParSens.size(); k++) // Pour chaque sens
    {
        statsCognatsParSens[1] = statsCognatsParSens[1] + pow(statsCognatsParSens[0] - nombreCognatsParSens[k], 2) ;
    }
    statsCognatsParSens[1] = (double) statsCognatsParSens[1] / nombreCognatsParSens.size() ;

    // Etendue du nombre de cognats par sens
    statsCognatsParSens.push_back(0) ;
    double minCognatsParSens = tableauDonnees.size() , maxCognatsParSens = 0 ;
    for (unsigned k = 0; k < nombreCognatsParSens.size(); k++) // Pour chaque sens
    {
        if (minCognatsParSens > nombreCognatsParSens[k])
        {
            minCognatsParSens = nombreCognatsParSens[k] ;
        }
        if (maxCognatsParSens < nombreCognatsParSens[k])
        {
            maxCognatsParSens = nombreCognatsParSens[k] ;
        }
    }
    statsCognatsParSens[2] = maxCognatsParSens - minCognatsParSens ;

    return statsCognatsParSens ;
}

std::vector<double> calculStatistique(std::vector< std::vector<int> > tableauDonnees)
{

    std::vector<double> tableauSumStats ;

    std::vector<double> proportionsSimilarite ;
    std::vector<double> proportionsTotales ;
    std::vector<double> statsCognatsParSens ;

    for (unsigned int i = 0; i < tableauDonnees.size(); i++)
    {
        for (unsigned int j = i + 1; j < tableauDonnees.size(); j++)
        {
            proportionsSimilarite = calculProportionSimilarite(tableauDonnees[i],tableauDonnees[j]) ;
            tableauSumStats.push_back(proportionsSimilarite[0]) ;  // Nombre de similarités par couple
            tableauSumStats.push_back(proportionsSimilarite[1]) ;  // Nombre de similarités d'ordre 2 par couple
        }
    }

    proportionsTotales = calculProportionsTotales(tableauDonnees) ;
    tableauSumStats.push_back(proportionsTotales[0]) ; // Nombre normalisé de similarités strictes pour toutes les langues
    tableauSumStats.push_back(proportionsTotales[1]) ; // Nombre normalisé de dissimilarités strictes pour toutes les langues

    statsCognatsParSens = calculStatsCognatsParSens(tableauDonnees) ;  // Moyenne, variance, étendue du nombre de cognats par sens
    tableauSumStats.push_back(statsCognatsParSens[0]) ; // Moyenne du nombre de cognats par sens
    tableauSumStats.push_back(statsCognatsParSens[1]) ; // Variance du nombre de cognats par sens
    tableauSumStats.push_back(statsCognatsParSens[2]) ; // Etendue du nombre de cognats par sens

    return tableauSumStats ;
}
