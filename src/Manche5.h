#ifndef MANCHE5_H
#define MANCHE5_H
#include "Manche.h"
#include "Terrain.h"
#include <vector>
#include <iostream>

using namespace std;


/** 
\struct Balles
@brief
Structure Balles possédant :\n
Un entier désignant son en sens;\n
Un entier désignant le timer de la balle; \n
Une position2D désignant la position de la balle sur la fenêtre.
*/
struct Balles {
        int sens;   
        unsigned int timerB;
        unsigned int vitessePos;
        Position2D posBalle;
};


/** 
\class Manche5
@brief
Classe Manche5 qui hérite de la classe Manches pour permettre l'utilisation de paramètres globaux
à toutes les manches. \n
Elle gère : \n
Une génération aléatoire de chiffres (correspondants à sa position de départ et sa direction); \n
La gestion dynamique du nombre de balles à l'aide d'un tableau dynamique; \n
Le tir et le déplacement en temps réel de chaques balles; \n
La victoire et la défaite de la manche.
*/


class Manche5 : public Manches{

    private:
        
        unsigned int timerC;
        unsigned int cadenceC;
        unsigned int timerV;
        

    public:

    vector<Balles> tabBalles;
    unsigned int vBalle;
    Terrain ter;
    Position2D postmp;

    Manche5(const Position2D &pos);
    ~Manche5();

    unsigned int getTimerC() const;

    void setTimerC(unsigned int t);

    void update();
    void tirerBalle();
    void testReussie(Jeu &j);
    void init(Personnage &p);
    void avancer(Balles &b);
    int testRegression();
};

#endif