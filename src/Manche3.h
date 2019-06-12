#ifndef MANCHE3_H
#define MANCHE3_H
#include "Personnage.h"
#include "Terrain.h"
#include "Jeu.h"
#include "Position2D.h"

/** 
\class Manche3
@brief
Classe Manche3 qui hérite de la classe Manches pour permettre l'utilisation de paramètres globaux
à toutes les manches. \n
Elle gère : \n
Une génération aléatoire de position (correspondant à l'objectif à atteindre); \n
La victoire et la défaite de la manche.
*/
class Manche3 : public Manches {

	
	public :	
		Position2D objectif;
		Position2D postmp; // position tampon du perso
		Manche3(const Position2D &p);
		~Manche3();


		void init(Jeu &j);
		void testReussie(Jeu &j);
		int testRegression();

};
#endif 