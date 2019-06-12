#include "Manche5.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include "Terrain.h"

/** 
@brief
Constructeur de la classe. Initialise une position tampon à la même position que le personnage.
*/
Manche5::Manche5(const Position2D &pos) : ter(4){
    ind = 4;
    postmp = pos;
}

/** 
\fn void Manche5::init(Personnage &p)
\param Prend un personnage en paramètre en mode données/résultat
@brief
Initialisation de la manche : \n
Initialise la position de départ du personnage; \n
Initialise les valeurs des timers utilisés pour les tirs.
*/
void Manche5::init(Personnage &p){
    Manches::init();
    tabBalles.clear();
    p.pos.setX(ter.getDimx()/2);
    p.pos.setY(ter.getDimy()/2);
    timerC = timer; 
    cadenceC = 3000;  
    vBalle = 5000;
    timerV = timer;
}

/** 
@brief
Destructeur de la classe.
*/
Manche5::~Manche5(){}


/** 
\fn unsigned int Manche5::getTimerC() const
\result
@brief
Renvoie la valeur du timer lié au canon.
*/
unsigned int Manche5::getTimerC() const{
    return timerC;
}

/** 
\fn void Manche5::setTimerC(unsigned int t)
\param Prend un temps en paramètre en milisecondes
@brief
Permet la mise à jour du timer lié au canon.
*/
void Manche5::setTimerC(unsigned int t){
    timerC = t;
}

/** 
\fn void Manche5::update()
@brief
Gère la position des balles en temps réel;\n
Tire une balle provenant d'un canon toutes les "CadenceC" secondes.
*/
void Manche5::update(){
    if(timer - timerC >= cadenceC)tirerBalle();

    for(unsigned int i = 0; i < tabBalles.size(); i++){ 
        avancer(tabBalles[i]);
        if(tabBalles[i].posBalle.getX() <= -1 ||
           tabBalles[i].posBalle.getX() >= ter.getDimx() ||
           tabBalles[i].posBalle.getY() <= -1 ||
           tabBalles[i].posBalle.getY() >= ter.getDimy() ){
               
               tabBalles.erase(tabBalles.begin()+i);
               i--;
        }    
    }

    if(timer - timerV >= 5000) {
        cadenceC -= 500;
        timerV = timer;
    }
}

/** 
\fn void Manche5::avancer(Balles &B)
\param Prend en paramètre une Balle en mode données/résultat
@brief
Gère la position des balles selon leur sens.
*/
void Manche5::avancer(Balles &B){
    switch(B.sens){
        case 0: // depuis haut
 
            if(timer - B.timerB >= vBalle/ter.getDimy()) {
                B.posBalle.setY(B.posBalle.getY()+1);
                B.timerB = timer;
            }
            break;
        case 1:// depuis bas

            if(timer - B.timerB >= vBalle/ter.getDimy()) {
                B.posBalle.setY(B.posBalle.getY()-1);
                B.timerB = timer;
            }
            break;
        case 2: // depuis gauche

            if(timer - B.timerB >= vBalle/ter.getDimx()) {
                B.posBalle.setX(B.posBalle.getX()+1);
                B.timerB = timer;
            }break;
        case 3:// depuis droite  
     
            if(timer - B.timerB >= vBalle/ter.getDimx()) {
                B.posBalle.setX(B.posBalle.getX()-1);
                B.timerB = timer;
            } break;  
    }
}

/** 
\fn void Manche5::tirerBalle()
@brief
Génère aléatoirement un nombre compris entre 0 et 3 (inclus) désignant le sens de la balle,
et gère sa position de départ en fonction de ce dernier.
*/
void Manche5::tirerBalle(){
    srand(time(NULL));
    timerC = timer;
    Balles B;
    B.timerB = timer;
    B.sens = rand()%4;
    switch(B.sens){
        case 0: // haut
            B.posBalle.setX(ter.getDimx()/2);
            B.posBalle.setY(1);
            B.vitessePos = (float)vBalle/ter.getDimy();
            break;
        case 1: // bas
            B.posBalle.setX(ter.getDimx()/2);
            B.posBalle.setY(ter.getDimy() - 2);
            B.vitessePos = (float)vBalle/ter.getDimy();
            break;
        case 2: // gauche
            B.posBalle.setX(1);
            B.posBalle.setY(ter.getDimy()/2);
            B.vitessePos = (float)vBalle/ter.getDimx();
            break;
        case 3: // droit
            B.posBalle.setX(ter.getDimx() - 2);
            B.posBalle.setY(ter.getDimy()/2);
            B.vitessePos = (float)vBalle/ter.getDimx();
            break;
        default:
            break;
    }
    tabBalles.push_back(B);
}

/** 
\fn void Manche5::testReussie(Jeu &j)
\param Prend en paramètre un Jeu en mode données/résultat
@brief
Test si la manche est réussie ou non : \n
Si la manche est fermée, défaite; \n
Si le joueur survie 20 secondes, victoire.
*/
void Manche5::testReussie(Jeu &j){

    if( timer - j.getTimer() > 20000) Victoire(j);

    else if(enCours == false ) Defaite(j);    

    else{
        for(unsigned int i = 0; i < tabBalles.size() ; i++){
            if(tabBalles[i].posBalle == j.perso.pos) {
                Defaite(j);
                break;
            }
        }
    }
}

/** 
\fn int Manche5::testRegression()
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int Manche5::testRegression() {
	cout<<"Tests sur la manche 5"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	Jeu j;
	init(j.perso);
	if (!tabBalles.empty()) {
			cout<<"Erreur d'initialisation"<<endl;
			return -1;
	}
	cout<<"Tests de déroulement de la manche"<<endl; 
    setTimer(getTimer()+5000);
    update();
    if (tabBalles.size()!=1) {
        cout<<"Erreur lors du tir d'une roquette"<<endl;
        return -1;
    }
    setTimer(tabBalles[0].timerB+5000);
    timerC = timer;
    update();
    cout<<tabBalles[0].posBalle.getX()<<" "<<tabBalles[0].posBalle.getY()<<endl;
    switch (tabBalles[0].sens) {
        case 0: 
            if (!(tabBalles[0].posBalle==Position2D((ter.getDimx()-1)/2, 2))) {
                cout<<"Erreur dans l'avancement des balles"<<endl;
                return -1;
            }
            break;
        case 1: 
            if (!(tabBalles[0].posBalle==Position2D((ter.getDimx()-1)/2, ter.getDimy()-3))) {
                cout<<"Erreur dans l'avancement des balles"<<endl;
                return -1;
            }
            break;
        case 2: 
            if (!(tabBalles[0].posBalle==Position2D(2, (ter.getDimy()-1)/2))) {
                cout<<"Erreur dans l'avancement des balles"<<endl;
                return -1;
            }
            break;
        case 3: 
            if (!(tabBalles[0].posBalle==Position2D(ter.getDimx()-3, (ter.getDimy()-1)/2))) {
                cout<<"Erreur dans l'avancement des balles"<<endl;
                return -1;
            }
            break;
    }
    tabBalles[0].posBalle = j.perso.pos;
    testReussie(j);
	if (enCours) {
		cout<<"Erreur lors de la défaite"<<endl;
		return -1;
	}
    tabBalles[0].posBalle = Position2D(-1,-1);
	update();
    if (!tabBalles.empty()) {
        cout<<"Mauvaise gestion des balles hors map"<<endl;
        return -1;
    }
    init(j.perso);
	setTimer(j.getTimer()+50000);
	testReussie(j);
	if (!reussie) {
		cout<<"Erreur lors de la victoire de la manche"<<endl;
		return -1;
	}
	cout<<"Tests sur la manche 5 tous passés"<<endl;
	return 0;
}