#include "Manche.h"
#include "Manche3.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>

using namespace std;

/** 
@brief
Constructeur de la classe, initialise une position tampon aux valeurs de la position du personnage.
*/
Manche3::Manche3(const Position2D &p){
	ind = 2;
	postmp = p;
}

/** 
@brief
destructeur de la classe.
*/
Manche3::~Manche3(){}

/**
\fn void Manche3::init(Jeu &j)
\param Prend en paramètre un Jeu en mode données/résultat 
@brief
Initialisation de la manche, choisi une position libre aléatoire dans l'environnement global.
*/
void Manche3::init(Jeu &j){
	Manches::init();
	srand(time(NULL)); 
	timer = 0;
	do{
		objectif.setX((rand()%j.ter.getDimx()));
		objectif.setY((rand()%j.ter.getDimy()));
	} while(!j.ter.estLibre(objectif.getX(),objectif.getY()));
}

/** 
\fn void Manche3::testReussie(Jeu &j) 
\param Prend en paramètre un Jeu en mode données/résultat
@brief
Test si la manche est réussie ou non : \n
Si le timer dépasse 12 secondes ou que la manche est fermée, défaite; \n
Si la position du personnage est identique à la position de l'objectif, victoire.

*/
void Manche3::testReussie(Jeu &j) {
	if (enCours == false || timer - j.getTimer() > 12000) {
		Defaite(j);
	}
	else if (objectif.getX() == j.perso.pos.getX() && 
			objectif.getY() == j.perso.pos.getY()) {
				
		Victoire(j);

	}
}

/** 
\fn int Manche3::testRegression() const
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int Manche3::testRegression() {
	cout<<"Tests sur la manche 3"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	Jeu j;
	init(j);
	if((postmp.getX()!=j.perso.pos.getX() && 
		postmp.getY()!=j.perso.pos.getY()) ||
	    objectif.getValPos(j.ter) == MUR) {
			cout<<"Erreur d'initialisation"<<endl;
			return -1;
	}
	cout<<"Tests de déroulement de la manche"<<endl;
	objectif.setX(-1);
	testReussie(j);
	if (!enCours) {
		cout<<"Erreur lors de la verification d'un resultat faux"<<endl;
		return -1;
	}
	setTimer(j.getTimer()+50000);
	testReussie(j);
	if (enCours) {
		cout<<"Erreur lors de la defaite de la manche"<<endl;
		return -1;
	}
	init(j);
	objectif = j.perso.pos;
	setTimer(j.getTimer());
	testReussie(j);
	if (!reussie) {
		cout<<"Erreur lors de la victoire de la manche"<<endl;
		return -1;
	}
	cout<<"Tests sur la manche 3 tous passés"<<endl;
	return 0;
}