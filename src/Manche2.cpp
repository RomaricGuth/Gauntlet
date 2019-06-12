#include "Manche.h"
#include "Manche2.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
using namespace std;

/** 
@brief
Constructeur de la classe Manche2
*/
Manche2::Manche2() {
	ind = 1;
}

/** 
\fn void Manche2::init()
Initialisation de la manche2 :\n
Génère un code ascii aléatoire et l'affecte dans deux tableaux : un gérant la valeur numérique, et l'autre 
la valeur en caractère (pour l'affichage).
*/
void Manche2::init(){
	Manches::init();
	srand(time(NULL)); // initialisation du rand
	nbJuste = 0;
	timer = 0;
	for (int i = 0; i < 15; i++)
	{
		touche[i] = rand()%26 + 97;
		toucheStr[i] = char(touche[i]);
	}
}

/** 
\fn string Manche2::getTouche()const 
\return
@brief
Renvoie le caractère à saisir par le joueur.
*/
string Manche2::getTouche()const {
	return toucheStr[nbJuste];
}

/** 
\fn int Manche2::getNbJuste()
\return
@brief
Renvoie  le nombre de caractère(s) correctement saisie(s) par le joueur.
*/
int Manche2::getNbJuste() {
	return nbJuste;
}

/** 
\fn void Manche2::verifTouche(int touch)
\param Prend en paramètre un entier désignant le code ascii de la touche saisie par le joueur
@brief
Gère la vérification des caractères saisies par le joueur :\n
Si le caractère est correct, on passe au suivant;\n
S'il n'est pas correct, la manche se ferme.
*/
void Manche2::verifTouche(int touch){
	if(touch == touche[nbJuste]){
		nbJuste++;
	}
	else enCours = false;
}

/**
\fn void Manche2::testReussie(Jeu &j)
\param Prend en paramètre un Jeu en mode donnée/résultat
@brief
Test si la manche a été réussie :\n
Si le temps mis est supérieur à 15 secondes ou que la manche est fermée, défaite;\n
Si 15 caractères sont correctement tapés, victoire.
*/
void Manche2::testReussie(Jeu &j) {
	if (enCours == false || timer - j.getTimer() > 15000) {
		Defaite(j);
	}
	else if (nbJuste == 15) {
		Victoire(j);
	}
}

/** 
\fn int Manche2::testRegression() const
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int Manche2::testRegression() {
	cout<<"Tests sur la manche 2"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	init();
	if(ind != 1 || nbJuste!=0) {
		cout<<"Erreur d'initialisation"<<endl;
		return -1;
	}
	for (int i=0;i<15;i++) {
		if (touche[i]<97 || touche[i]>122) {
			cout<<"Erreur d'initialisation"<<endl;
			return -1;
		}
	}
	cout<<"Tests des getters/setters"<<endl;
	if (getNbJuste()!=nbJuste || getTouche() != toucheStr[nbJuste]) {
		cout<<"Erreur de getter"<<endl;
		return -1;
	}
	cout<<"Tests de déroulement de la manche"<<endl;
	verifTouche(0);
	Jeu j;
	testReussie(j);
	if (enCours) {
		cout<<"Erreur lors de la verification d'un resultat faux"<<endl;
		return -1;
	}
	init();
	verifTouche(touche[0]);
	if (nbJuste != 1) {
		cout<<"Erreur lors de la verification d'un resultat vrai"<<endl;
		return -1;
	}
	nbJuste = 15;
	testReussie(j);
	if (!reussie) {
		cout<<"Erreur lors de la victoire de la manche"<<endl;
		return -1;
	}
	cout<<"Tests sur la manche 2 tous passés"<<endl;
	return 0;
}