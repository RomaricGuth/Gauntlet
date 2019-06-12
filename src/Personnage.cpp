#include "Personnage.h"
#include <cassert>
#include <iostream>
using namespace std;


/**
\fn int Personnage::getVies() const 
\return Renvoie le nombre de vie(s) du joueur.
 */
int Personnage::getVies() const {
    return vies;
}

/** 
\fn void Personnage::perdreVie()
@brief 
décrémente le nombre de vie(s) du joueur.
*/
void Personnage::perdreVie() {
    vies--;
}

/** 
@brief Constructeur de la classe Personnage, initialise le nombre de vies à 3.
*/
Personnage::Personnage() {
    vies = 3;
}

/** 
@brief Destructeur de la classe personnage.
*/
Personnage::~Personnage() {

}

/** 
\fn int Personnage::testRegression() 
\return Renvoie 0 ou -1 selon le bon fonctionnement de la classe.
*/
int Personnage::testRegression() {
    cout<<endl<<"Tests sur la classe Personnage"<<endl;
    cout<<"Verification de l'initialisation du personnage"<<endl;
    if (vies != 3) return -1;
    cout<<"Verification des getters/setters"<<endl;
    if (getVies() != vies) return -1;
    int tmp = vies;
    perdreVie();
    if (vies != tmp-1) return -1;
    if (pos.testRegression() == -1) {
        cout<<"Erreur lors des tests sur la position"<<endl;
        return -1;
    }
    cout<<endl<<"Tests sur le Personnage tous passes"<<endl;
    return 0;
}