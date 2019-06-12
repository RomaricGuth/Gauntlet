#ifndef MANCHE2_H
#define MANCHE2_H
#include "Manche.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

/** 
\class Manche2
@brief
Classe Manche2 qui hérite de la classe Manches pour permettre l'utilisation de paramètres globaux
à toutes les manches. \n
Elle gère : \n
Une génération aléatoire de code ascii; \n
La vérification de la touche appuyée par le joueur;\n
La victoire et la défaite de la manche.
*/ 
class Manche2 : public Manches
 {

 	private : 

 		int touche[15];
		string toucheStr[15];
 		int nbJuste;
 		
 	public:

 		Manche2();

		void testReussie(Jeu &j);
 		void init();
		string getTouche()const;
		int getNbJuste();
		void verifTouche(int code);
		int testRegression();
 }; 
 #endif