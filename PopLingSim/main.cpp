    #include<iostream>
    #include<cmath>
    #include<fstream>
    #include<stdlib.h>
    #include <time.h>
    #include <ctime>
    #include <vector>
    #include <random>
    #include <chrono>

    #include "node.h"
    #include "header.h"


    ///__________________________///
    ///                          ///
    ///   linguisticModel        ///
    ///   version 2.1            ///
    ///   Valentin Thouzeau      ///
    ///   17-03-2015             ///
    ///   Auckland, New Zealand  ///
    ///__________________________///


    using namespace std;

    int main(int argc, char *argv[])
    {


    ///___ INITIALISATIONS ___///

        // Initialisation des distributions aléatoires
        std::default_random_engine engine ;
        std::uniform_real_distribution<double> randomU(0.0, 1.0) ;


        
// NOTE : MODIFY HERE THE NUMBER OF SIMULATIONS AND THE NUMBER OF COGNATES

        int nombreRepetitions = 200000 ;
        int nombreCognats = 185 ; // Spécifier 0 pour imposer un taux de mutation unique par cognat

// END OF THE NOTE    
        
        
        
        // Suppression des potentiels fichiers préexistants
        remove("outSumStatsL.txt") ;
        remove("outParamL.txt") ;

        // Création d'un flux vers les nouveaux fichiers
        ofstream sumStatsL("outSumStatsL.txt", ios::app) ;
        ofstream paramL("outParamL.txt", ios::app) ;

        for (int i = 0; i < 5; i++)
        {
            paramL << "MUTATION_" << i << "\t";
        }
        paramL << "EMPRUNT\tADMIXTURE\tTEMPS_MIG\tEVENT_0\tEVENT_1" ;
        paramL << endl ;


    ///___ LECTURE DE L'ARBRE ___///

// NOTE : REMEMBER TO MODIFY THE TOPOLOGY OF THE THREE-BRANCHES TREE FILE NAMED rooted_tree.tre 

        string nomArbre = "rooted_tree.tre"; string arbre ;
        ifstream fichierArbre(nomArbre, ios::in);
        if(fichierArbre)  // Si l'ouverture a réussi
        {
            getline(fichierArbre, arbre);
            fichierArbre.close();  // On ferme le fichier
        }
        else cerr << "Le fichier " << nomArbre << " est introuvable" << endl;


    ///___ BOUCLE DE REPETITION ___///

        for (int n = 0; n < nombreRepetitions; n++)
        {


    ///___ PARAMETRES DU MODELE ___///

            // Paramètres linguistiques
            
            

// NOTE : MODIFY HERE THE UPPER BOUNDARY OF THE PRIORS FOR MUTATION, BORROWING AND ADMIXTURE

            double mutationL = 0.01*randomU(engine) ; // Paramètre de description de la mutation
            double emprunt = 0.1*randomU(engine) ; // Paramètre de description de l'emprunt
            double admixture = randomU(engine) ; // Paramètre de description de l'admixture
            
// END OF THE NOTE    
            
            
            
            int temps_mig = 0 ;
            std::vector<double> mutationsL ;
            std::vector<double> emprunts ;
            std::vector<std::vector<double>> vectorMutationsL ; // Vecteur contenant les taux de mutations variables

    ///___ TIRAGE DES TAUX DE MUTATIONS ET EMPRUNTS VARIABLES ___///

            double loi = 1 ; // Code: 1 = Loi Gamma, 2 = Loi Beta tronquée, 0 ou invalide = pas de variation

            // A mettre à l'interieur de la boucle pour changer de taux de mutation pour chaque branche
            mutationsL = distributionParametre(mutationL, loi, nombreCognats, engine) ;

            for (int i = 0; i < 5; i++)
            {
                vectorMutationsL.push_back(mutationsL) ;
            }

            emprunts = distributionParametre(emprunt, loi, nombreCognats, engine) ;


    ///___ TIRAGE DES TEMPS DE DIVERGENCE ___///

            int tempsMax = -1 ; // tempsMax = -1 implique la spécification des temps de divergence au préalable. Si temps != - 1, ne pas renseigner temps!
            vector<int> temps ;

            if (tempsMax == -1)
            {
                temps.push_back(0) ;
                temps.push_back(0) ;

                do {
                    
                    
// NOTE : MODIFY HERE THE UPPER BOUNDARIES OF THE PRIORS FOR THE TIME t_0 and t_1
                    
                    temps[0] = (int) 1 + 1000*randomU(engine) ;
                    temps[1] = (int) 1 + 1000*randomU(engine) ;
                    
// END OF THE NOTE    

                    
                } while (temps[0] <= temps[1]) ;
                tempsMax = temps[0] ;
            }
            
            //temps_mig = (int) (temps[0] - temps[1])*randomU(engine) + temps[1] ;
            temps_mig = temps[1]*randomU(engine) ;

            int itTemps = 1 ; // Itérateur du vecteur temps
            int numeroBranchement = 0 ; // Compteur du nombre de branchement

    ///___ FORMATION DU NODE ANCESTRAL ___///

            vector<node> nodes ;

            node newNode(vectorMutationsL[0], emprunts) ;
            node casseroleNode(vectorMutationsL[0], emprunts) ;

            newNode.nouvelArbre(arbre) ;
            nodes.push_back(newNode) ;


    ///___ BOUCLE EVOLUTIVE ___///

            for (int t = tempsMax; t > 0; t--) // La boucle d'évolution est Foreward, avec t commençant à tempsMax et terminant à 0
            {
                // Mutation
                for (unsigned int i = 0; i < nodes.size(); i++) nodes[i].mutation() ;
                
                
// NOTE : SHAPE HERE THE BORROWING 

    /*
                // Emprunts ponctuels
                if (t == temps[1] - 1)
                {

                    for (int i = 0; i < nombreCognats; i++)
                    {
                        mutationsLbis[i] = mutationsLbis[i] + mutationsL[i] ;
                    }
                    nodes[1].nouveauTauxMutation(mutationsLbis) ;
                }
    */
                // Emprunts le long d'une branche
                if (t < temps[1])
                {
                    nodes[1].empruntLocal(nodes[2]) ;
                    nodes[2].empruntLocal(nodes[1]) ;
                    //nodes[1].empruntLocal(nodes[2]) ;
                    //nodes[2].empruntLocal(nodes[1]) ;
                }

    /*
                //Emprunts le long d'une branche, à partir de t_mig          
                if (t < temps_mig)
                {
                    nodes[0].empruntLocal(nodes[1]) ;
                    nodes[1].empruntLocal(nodes[0]) ;
                }

    */
                
// END OF THE NOTE    
                
                
                /// Divergence
                for (unsigned int i = 0; i < nodes.size(); i++)
                {
                    // Si la taille de branche restante est nulle, on duplique le node
                    if (nodes[i].getTailleBranche() <= 0)
                    {
                        node newNode(nodes[i]) ; // On crée un node fils

                        nodes[i].nouveauTauxMutation(vectorMutationsL[numeroBranchement+1]) ; // On donne un nouveau taux de mutation au noeud père
                        newNode.nouveauTauxMutation(vectorMutationsL[numeroBranchement+2]) ; // On donne un nouveau taux de mutation au noeud fils
                        numeroBranchement = numeroBranchement + 2 ;

                        nodes.push_back(newNode) ; // On le place par défault à la fin du vecteur de nodes

                        vector<string> doubleArbre = splitArbre(nodes[i].getSousArbre()) ; // On sépare l'ancien sous-arbre en deux
                        nodes[i].nouvelArbre(doubleArbre[0]); // On attribue le premier sous-sous-arbre au node père
                        nodes[nodes.size() - 1].nouvelArbre(doubleArbre[1]); // On attribue le second sous-sous-arbre au node fils

                        if (temps.size() + 2 == nodes.size()) // Si le vecteur temps n'a pas été renseigné
                        {
                            nodes[i].nouvelleTailleRandom(t) ; // On calcul la nouvelle taille de branche avant prochain split (ou avant t = 0)
                            nodes[nodes.size() - 1].nouvelleTailleRandom(t) ; // Idem
                            temps.push_back(t) ; // On enregistre comme paramètre le temps de divergence
                        }
                        else // Si le vecteur temps a été renseigné
                        {
                            if (nodes[i].getSousArbre().find(',') != string::npos) // Si le sous-arbre contient une virgule, cad si la branche n'est pas terminale
                            {
                                nodes[i].nouvelleTaille(t - temps[itTemps]) ; // On impose la nouvelle taille de branche avant prochain split
                            }
                            else nodes[i].nouvelleTailleRandom(t) ; // Sinon la branche est terminale, et on contraint donc sa taille
                            if (nodes[nodes.size() - 1].getSousArbre().find(',') != string::npos) // Si le sous-arbre contient une virgule, cad si la branche n'est pas terminale
                            {
                                nodes[nodes.size() - 1].nouvelleTaille(t - temps[itTemps]) ; // On impose la nouvelle taille de branche avant prochain split
                                itTemps++ ;
                            }
                            else nodes[nodes.size() - 1].nouvelleTailleRandom(t) ; // Sinon la branche est terminale, et on contraint donc sa taille
                        }

                        // On réarrange la place des noeuds dans le vecteur pour les faire correspondre au fichier .tre
                        for (unsigned int j = nodes.size() - 1 ; j > i+1; j--)
                        {
                            casseroleNode = nodes[j] ;
                            nodes[j] = nodes[j-1] ;
                            nodes[j-1] = casseroleNode ;
                        }

                    }
                } /// Fin de la boucle de divergence

                // Admixture
    /*
                if (t == temps[1])
                {
                    // Emprunts massifs
                    nodes[1].empruntMassif(nodes[2], admixture) ;
                }*/

                // Incrémentation interne des node (Note: A faire avec modification des taux le cas échéant)
                for (unsigned int i = 0; i < nodes.size(); i++) nodes[i].miseAJour() ;


    ///___ AFFICHAGE DES DONNEES A CHAQUE INSTANT ___///
    /*
                for (unsigned int i = 0; i < nodes.size(); i++)
                {
                    for (unsigned int j = 0; j < nodes[i].getData().size(); j++)
                    {
                        cout << nodes[i].getData()[j] << " ";
                    }
                    cout << endl ;
                }
                cout << endl ;

    */


            } /// Fin de la boucle d'évolution


    ///___ STOCKAGE DES DONNEES AU TEMPS 0 ___///

            vector< vector<int> > tableauDonnees ;
            for (unsigned int i = 0; i < nodes.size(); i++) tableauDonnees.push_back(nodes[i].getData()) ;



    ///___ ECRITURE DES DONNEES DE SORTIE ___///
    /*
            for (unsigned int i = 0; i < tableauDonnees.size(); i++)
            {
                for (unsigned int j = 0; j < tableauDonnees[i].size(); j++)
                {
                    dataL << tableauDonnees[i][j] << " " ;
                }
                dataL << endl ;
            }
            dataL << endl ;
    */


    ///___ ECRITURE DES PARAMETRES DU MODELE ___///

            // Calcul de la moyenne des taux de mutation liguistiques

            for (int i = 0; i < vectorMutationsL.size(); i++)
            {
                mutationL = 0 ;
                for (unsigned int j = 0; j < vectorMutationsL[i].size(); j++)
                {
                    mutationL = mutationL + vectorMutationsL[i][j] ;
                }
                mutationL = mutationL/vectorMutationsL[i].size();
                paramL << mutationL << "\t" ;
            }

            paramL << emprunt << "\t" << admixture << "\t" << temps_mig ; // Paramètres linguistiques

            for (unsigned int i = 0; i < temps.size(); i++)
            {
                paramL << "\t" << temps[i] ;
            }

            paramL << endl ;


    ///___ CALCUL DES STATISTIQUES RESUMEES ___///

            vector<double> sumStats = calculStatistique(tableauDonnees) ;

            for (unsigned int i = 0; i < sumStats.size() ; i++)
            {
                sumStatsL << sumStats[i] << "\t" ;
            }
            sumStatsL << endl ;



    ///___ AFFICHAGE DE LA PROGRESSION ___///

            //if (n % 100 == 0)
            //{
            //    cout << ((double) n / nombreRepetitions)*100 << " %" << endl ;
            //}


        } /// Fin de la boucle de répétition


        return 0 ;
    }
