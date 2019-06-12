#include "Manche.h"
#include "Manche1.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include <string>
using namespace std;

/** 
@brief
Constructeur de la classe Manche 1. 
*/
Manche1::Manche1() {
	ind = 0;
}

/** 
\fn void Manche1::init()
@brief
Initialisation de la Manche 1 : \n
Initialisation globale de la manche; \n
génération aléatoire de nombres pour créer successivement une adition,
une soustraction puis une multiplication, en rengeant l'opération ainsi
que le résultat dans deux tableaux distincts.
*/
void Manche1::init() {
	Manches::init();
	nbJuste = 0;
	res = 0;
	srand(time(NULL)); // initialisation du rand
	double x, y;
	x = rand()%50;
	y = rand()%50;
	resultat[0] = x + y;
	operation[0] = to_string(x) + " + " + to_string(y) + " = ?";
	x = rand()%50;
	y = rand()%50;
	if (x > y) {
		resultat[1] = x - y;
		operation[1] = to_string(x) + " - " + to_string(y) + " = ?";
	}
	else {
		resultat[1] = y - x;
		operation[1] = to_string(y) + " - " + to_string(x) + " = ?";
	}
	x = rand()%10;
	y = rand()%10;
	resultat[2] = x * y;
	operation[2] = to_string(x) + " * " + to_string(y) + " = ?";
	operation[3] = "Additionnez les 3 premiers resultats";
	resultat[3] = resultat[0] + resultat[1] + resultat[2];
}

/** 
\fn void Manche1::testReussie(Jeu &j)
\param Prend un jeu en mode donnée/résultat en paramètre
@brief 
Test si la manche est réussie ou non : \n
Si le temps mis est supérieur à 15 secondes, ou que la manche n'est plus en cours, défaite. \n
Si tous les résultats sont correctes, victoire.
*/
void Manche1::testReussie(Jeu &j) {
	if (enCours == false || timer - j.getTimer() > 15000) {
		Defaite(j);
	}
	else if (nbJuste == 4) {
		Victoire(j);
	}
} 

/** 
\fn string Manche1::getOperation() const 
\return
@brief
Renvoie le tableau de caractère concernant l'opération actuelle pour l'affichage.
*/
string Manche1::getOperation() const {
	return operation[nbJuste];
}

/** 
\fn int Manche1::getResultat() const 
\return
@brief
Renvoie le tableau de caractère concernant le résultat actuelle pour l'affichage.
*/
int Manche1::getResultat() const {
	return resultat[nbJuste];
}

/** 
\fn int Manche1::getRes() const
\return
@brief
Renvoie le résultat de l'opération.
*/
int Manche1::getRes() const {
	return res;
}

/** 
\fn int Manche1::getNbJuste() const
\return
@brief
Renvoie le nombre de bons résultats proposés par le joueur.
*/
int Manche1::getNbJuste() const {
	return nbJuste;
}

/** 
\fn void Manche1::aSaisi(int nb)
\param Prend en paramètre le nombre saisi par le joueur
@brief
Affecte à la variable "res" la valeur saisie par le joueur.
*/
void Manche1::aSaisi(int nb) {
	res *= 10;
	res += nb;
}

/** 
\fn void Manche1::verifRes() 
@brief
Test si le résultat proposé par le joueur est correct :\n
Si oui : on passe à l'opération suivante; \n
Si non : la manche se ferme.
*/
void Manche1::verifRes() {
	cout<<"Vous avez entre "<<res<<endl;
	if (res == resultat[nbJuste])
	{
        res = 0;
		nbJuste++;
		cout<<"Bon resultat"<<endl;
	}
	else {
		enCours = false;
		cout<<"Ce n'est pas le bon resultat"<<endl;
	}	
}

/**
\fn int Manche1::modifRes()
@brief 
Permet la modification du résultat.
*/
int Manche1::modifRes(){
	return res = int(res/10);
}

/** 
\fn int Manche1::testRegression() const
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int Manche1::testRegression() {
	cout<<"Tests sur la manche 1"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	init();
	if(ind != 0 || nbJuste!=0 || res != 0) {
		cout<<"Erreur d'initialisation"<<endl;
		return -1;
	}
	cout<<"Tests des getters/setters"<<endl;
	if (getNbJuste()!=nbJuste || getRes()!=res || 
		getResultat() != resultat[nbJuste] ||
		getOperation() != operation[nbJuste]) {
		cout<<"Erreur de getter"<<endl;
		return -1;
	}
	cout<<"Tests de déroulement de la manche"<<endl;
	aSaisi(8);
	if (res != 8) {
		cout<<"Erreur lors de la saisie d'un chiffre"<<endl;
		return -1;
	}
	aSaisi(0);
	if (res != 80) {
		cout<<"Erreur lors de la saisie d'un chiffre"<<endl;
		return -1;
	}
	if (modifRes() != 8) {
		cout<<"Erreur lors de l'effacement d'un chiffre"<<endl;
		return -1;
	}
	res=-1;
	verifRes();
	Jeu j;
	testReussie(j);
	if (enCours) {
		cout<<"Erreur lors de la verification d'un resultat faux"<<endl;
		return -1;
	}
	init();
	res = resultat[0];
	verifRes();
	if (nbJuste != 1) {
		cout<<"Erreur lors de la verification d'un resultat vrai"<<endl;
		return -1;
	}
	nbJuste = 4;
	testReussie(j);
	if (!reussie) {
		cout<<"Erreur lors de la victoire de la manche"<<endl;
		return -1;
	}
	cout<<"Tests sur la manche 1 tous passés"<<endl;
	return 0;
}