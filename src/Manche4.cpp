#include "Manche4.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <time.h>
#include "Terrain.h"

using namespace std;



/** 
@brief
Constructeur de la classe. Initialise une position tampon à la position du personnage, et initialise
la position des portes sur la map.
*/
Manche4::Manche4(const Position2D &p) : ter(3) {
    ind = 3;
    postmp = p;
    for (int i=0;i<ter.getDimx();i++) {
        for (int j=0;j<ter.getDimy();j++) {
            if (isdigit(ter.getMap(i,j))) {
                portes[int(ter.getMap(i,j)-'0')].pos.setX(i);
                portes[int(ter.getMap(i,j)-'0')].pos.setY(j);
            }
        }
    }

}

/** 
@brief
Destructueur de la classe.
*/
Manche4::~Manche4(){
}


/** 
\fn void Manche4::prendrePorte(Personnage &perso) const
\param Prend un Personnage en paramètre en mode données/résultat
@brief
Si le joueur prend une porte, il est envoyé à la porte suivante correspondante.
*/
void Manche4::prendrePorte(Personnage &perso) const{
    if (isdigit(perso.pos.getValPos(ter)))
        perso.pos = portes[portes[int(perso.pos.getValPos(ter)-'0')].numNext].pos;
}


/**
\fn void Manche4::testReussie(Jeu &j)
\param Prend un Jeu en paramètre en mode données/résultat
@brief
Test si la manche est réussie ou non :\n
Si la manche est fermée ou que le timer est supérieur à 20 secondes, défaites;\n
Si le personnage atteint la position de la dernière porte, victoire.
 */
void Manche4::testReussie(Jeu &j){
	if (enCours == false || timer - j.getTimer() > 20000) {
		Defaite(j);
	}
	else if (j.perso.pos.getValPos(ter) == '9') {
		Victoire(j);
	}
}


/** 
\fn void Manche4::init(Personnage &perso)
\param Prend un personnage en paramètre en mode données/résultat
@brief
Initialisation de la manche :\n
Donne la position de départ du personnage;\n
Génère aléatoire la redirection de chaque porte en faisant en sorte qu'une seule
porte par étage mène à l'étage inférieur.
*/
void Manche4::init(Personnage &perso){
    Manches::init();
    srand(time(NULL));
    perso.pos.setX(0);
    perso.pos.setY(1);
    for (int etage=0;etage<3;etage++) {
        int pbas = rand()%3;
        for (int p=0;p<3;p++) {
            if (p==pbas) {
                if (etage==2) portes[p+etage*3].numNext = 9;
                else portes[p+etage*3].numNext = rand()%3+(etage+1)*3;
            }
            else {
                int nn;
                do {
                    nn = rand()%((etage+1)*3);
                } while (nn==p+etage*3);
                portes[p+etage*3].numNext = nn;
            }
        }
    }
}

/** 
\fn int Manche4::testRegression() const
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int Manche4::testRegression() {
	cout<<"Tests sur la manche 4"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	Jeu j;
	init(j.perso);
	for (int i=0;i<9;i++) {
        if (portes[i].numNext<0 || portes[i].numNext>9 || 
            portes[i].numNext == i) {
                cout<<"Erreur d'initialisation"<<endl;
                return -1;
        }
	}
	cout<<"Tests de déroulement de la manche"<<endl;
    j.perso.pos = portes[0].pos;
    prendrePorte(j.perso);
    if (!(j.perso.pos == portes[portes[0].numNext].pos)) {
        cout<<"Erreur lors de la prise d'une porte"<<endl;
        return -1;
    }
    setTimer(j.getTimer());
	j.perso.pos = portes[0].pos;
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
    init(j.perso);
    setTimer(j.getTimer());
	j.perso.pos = portes[9].pos;
	testReussie(j);
	if (!reussie) {
		cout<<"Erreur lors de la victoire de la manche"<<endl;
		return -1;
	}
	cout<<"Tests sur la manche 4 tous passés"<<endl;
	return 0;
}