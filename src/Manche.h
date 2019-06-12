#ifndef MANCHE_H
#define MANCHE_H
#include "Jeu.h"

/**
\class Manches
@brief
Classe Manches qui gère :\n
Le timer local de chaque manche; \n
La victoire et la défaite de chaque manches; \n
L'initialisation générale de chaque manches (booléen "enCours").

*/
class Manches{

	protected :

		int ind;
		bool reussie;
		bool enCours;
		unsigned int timer;

	public :


	int getInd() const;
	bool getReussie() const;
	bool getEnCours() const;
	unsigned int getTimer() const;
	void setTimer(unsigned int time);
	Manches();
	~Manches();

	void init();
	void Victoire(Jeu &j);
	void Defaite(Jeu &j);
	void Lancement() const;
	int testRegression();

};

#endif