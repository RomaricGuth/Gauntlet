#include "Manche.h"
#include <iostream>
#include "Jeu.h"
using namespace std;

/** 
\fn int Manches::getInd() const
\return
@brief
renvoie l'indice de manche.
*/
int Manches::getInd() const{
	return ind;
}

/** 
\fn bool Manches::getReussie() const
\return
@brief
Renvoie un booléen indiquant si la manche a été réussie.
*/
bool Manches::getReussie() const {
	return reussie;
}

/** 
\fn  Manches::getEnCours() const
\return 
@brief
Renvoie un booléen indiquant si la manche est en cours d'exécution.
*/
bool Manches::getEnCours() const {
	return enCours;
}

/** 
\fn unsigned int Manches::getTimer() const
\return
@brief
Renvoie le timer local de la manche.
*/
unsigned int Manches::getTimer() const {
	return timer;
}

/** 
\fn void Manches::setTimer(unsigned int time)
\param prend un temps en milisecondes en paramètre
@brief
Permet la mise à jour du timer local.
*/
void Manches::setTimer(unsigned int time) {
	timer = time;
}

/** 
@brief
constructeur de la classe Manche : initialise le booléen de
test de réussite de la manche à faux.
*/
Manches::Manches(){
	reussie = false;
}

/** 
@brief
Initialise le booléen " en cours " à vrai.
*/
void Manches::init() {
	enCours = true;
}


/** 
@brief
Destructeur de la classe Manche.
*/
Manches::~Manches(){
}


/** 
\fn void Manches::Victoire(Jeu &j)
\param Prend un Jeu en mode données/résultat
@brief
Met à jour les booléens "en cours" et "reussie" en cas de victoire, 
ainsi que le tableau de Manches présent dans la classe Jeu.
*/
void Manches::Victoire(Jeu &j) {
	cout << "Manche réussie, bien joué !!!" << endl;
	enCours = false;
	reussie = true;
	j.MancheReussie(ind);
}

/** 
\fn void Manches::Defaite(Jeu &j)
\param Prend un Jeu en mode données/résultat
@brief
Met à jour les booléens "en cours" en cas de défaite, 
et décrémente le nombre de vies du personnage.
*/
void Manches::Defaite(Jeu &j) {
	enCours = false;
	cout << "Manche perdue, dommage...." << endl;
	j.perso.perdreVie();
}

/** 
\fn void Manches::Lancement() const
@brief 
affiche en console le bon lancement de la manche.
*/
void Manches::Lancement() const {
	cout << "Lancement de la manche " << ind+1 << endl;
}

/** 
\fn int Manches::testRegression() const
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int Manches::testRegression() {
	cout<<"Tests sur le corps de chaque manche"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	init();
	if(!enCours || reussie) {
		cout<<"Erreur d'initialisation"<<endl;
		return -1;
	}
	cout<<"Tests des getters/setters"<<endl;
	setTimer(1000);
	if (timer != 1000 || getTimer()!=1000) {
		cout<<"Erreur sur la gestion du timer"<<endl;
		return -1;
	}
	if (getEnCours()!=enCours || getReussie()!=reussie || getInd()!=ind) {
		cout<<"Erreur de getter"<<endl;
		return -1;
	}	
	cout<<"Tests de déroulement de la manche"<<endl;
	Jeu j;
	int vies = j.perso.getVies();
	Defaite(j);
	if (enCours || reussie || j.perso.getVies() != vies -1) {
		cout<<"Erreur lors de la defaite d'une manche"<<endl;
		return -1;
	}
	int nb = j.getNbReussie();
	Victoire(j);
	if (enCours || !reussie || j.getNbReussie()!=nb+1 || !j.getManche(ind)) {
		cout<<"Erreur lors de la victoire d'une manche"<<endl;
		return -1;
	}
	cout<<"Tests sur le corps des manches tous passés"<<endl;
	return 0;
}