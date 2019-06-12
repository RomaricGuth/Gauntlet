#ifndef MANCHE4_H
#define MANCHE4_H
#include "Manche.h"
#include "Terrain.h"

using namespace std;

/** 
\struct  Porte
@brief
Structure Porte qui est désigné par :
Une position2D représentant la position de la porte sur la fenêtre;\n
Un entier désignant le numéro de la porte suivante à laquelle la porte est reliée.
*/
struct Porte {
    Position2D pos;
    int numNext;
};

/** 
\class Manche4
@brief
Classe Manche4 qui hérite de la classe Manches pour permettre l'utilisation de paramètres globaux
à toutes les manches. \n
Elle gère : \n
Une génération aléatoire de chiffres correspondants à la redirection de chaque portes; \n
La prise d'une porte par le joueur;\n
La victoire et la défaite de la manche.
*/
class Manche4 : public Manches
 {

 	private: 

 		Porte portes[10];
		
 		
 	public:

		Terrain ter;
        Position2D postmp;

 		Manche4(const Position2D &p);
		~Manche4();

 		void prendrePorte(Personnage &perso) const;
		void testReussie(Jeu &j);
 		void init(Personnage &perso);
		int testRegression();
 }; 
 #endif