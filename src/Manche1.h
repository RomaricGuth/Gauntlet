#ifndef MANCHE1_H
#define MANCHE1_H
#include <string>
#include "Manche.h"
using namespace std;

/** 
\class Manche1
@brief
Classe Manche1 qui hérite de la classe Manches pour permettre l'utilisation de paramètres globaux
à toutes les manches. \n
Elle gère : \n
Une génération aléatoire d'opérations mathématiques;\n
La saisie d'une réponse par le joueur;\n
La vérification du résultat proposé par le joueur.
*/
class Manche1 : public Manches {

	private : 

		string operation[4];		
		int resultat[4];
		int res;
		int nbJuste;
	
	public : 

		Manche1();
		void init();
		string getOperation() const;
		int getNbJuste() const;
		int getResultat() const;
		int getRes() const;
		void aSaisi (int nb);
		void verifRes();
		int modifRes();
		void testReussie (Jeu &j);
		int testRegression();

};
#endif
