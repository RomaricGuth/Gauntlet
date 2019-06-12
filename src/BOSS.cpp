#include "Manche.h"
#include "BOSS.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include<cassert>
#include <stdio.h>
#include <time.h>
#include <string>
using namespace std;

/** 
\fn void BOSS::init(Personnage &perso)
\param Prend un personnage en paramètre en mode données/résultat
@brief
Initialisation de la manche : \n
Initialise la position de départ du personnage; \n
Initialise les valeurs des timers utilisés pour les tirs; \n
Initialise la vie du personnage et du BOSS.
*/
void BOSS::init(Personnage &perso) {
    Manches::init();
    ter.reinit(5);
    tabRoquettes.clear();
    posF.clear();
    bouleF.clear();
    onde.ondeH.clear();
    onde.ondeB.clear();
    onde.ondeG.clear();
    onde.ondeD.clear();
    perso.pos.setX(ter.getDimx()/2);
    perso.pos.setY(4*ter.getDimy()/5);
    boss.setX(14);
    boss.setY(8);
    vieBOSS = 3000;
    viePerso = 200;
    timerR = timer;
    timerF = timer;
    tFeu = timer;
    timerInvincible=timer;
    timerBoss=0;
    cadenceR = 2500;
    vRoquette = 3500;
    vFeu = 3500;
    tmp=perso.pos;
    sens='h';
    initTelep();
    changePos=false;
    invincible=false;
    onde.saut=false;
    onde.onde=false;
    onde.vieDernierSaut=3000;
    onde.debut=timer;
    feu = false;
}

void BOSS::initTelep() {
    srand(time(NULL));
    int telepCentre = rand()%4;
    tabTelep[telepCentre].numNext = 4;
    tabTelep[4].numNext=rand()%4;

    int nn;
    for(int i=0;i<5;i++) {
        if (i != telepCentre) {
            do {
                nn = rand()%5;
            } while (nn==4 || nn==i);
            tabTelep[i].numNext = nn;
        }
    }
}

/** 
\fn bool BOSS::getSens()const
\return Renvoie le sens du personnage.
*/
char BOSS::getSens() const {
    return sens;
}

/** 
\fn void BOSS::setSens(char s)
\param Prend un charactère en paramètres
@brief affect à sens la valeur du charactère passé en paramètre.
*/
void BOSS::setSens(char s) {
    assert(s=='h' || s=='b' || s=='g' || s=='d');
    sens = s;
}

/** 
\fn int BOSS::getViePerso()const
\return Renvoie le nombre de points de vie du personnage.
*/
int BOSS::getViePerso()const{
    return viePerso;
}

/** 
\fn int BOSS::getVieBOSS()const
\return Renvoie le nombre de points de vie du BOSS.
*/
int BOSS::getVieBOSS()const{
    return vieBOSS;
}
/** 
\fn bool BOSS::getFeu()const
\return Renvoie la valeur du booléen 'feu'.
*/
bool BOSS::getFeu()const{
    return feu;
}

/** 
\fn void BOSS::setFeu(bool f)
\param Prend un booléen en paramètres
@brief affect à 'feu' la valeur du booléen passé en paramètre.
*/
void BOSS::setFeu(bool f){
    feu = f;
}
/** 
@brief
Constructeur de la classe. Initialise la position des téléporteurs sur la map.
*/
BOSS::BOSS(Personnage &perso): ter(5){
    ind = 5;
    changePos=false;
    tmpPerso = perso.pos;
    for (int i=0;i<ter.getDimx();i++) {
        for (int j=0;j<ter.getDimy();j++) {
            if (isdigit(ter.getMap(i,j))) {
                tabTelep[int(ter.getMap(i,j)-'0')].pos.setX(i);
                tabTelep[int(ter.getMap(i,j)-'0')].pos.setY(j);
            }
        }
    }
}


/** 
@brief
Destructeur de la classe.
*/
BOSS::~BOSS(){}

/** 
\fn void BOSS::update(Personnage &perso)
\param Prend en paramètre un personnage en mode données/résultat
@brief
Gère la position des roquettes en temps réel ;\n
Tire une roquette provenant d'un canon toutes les "CadenceR" secondes; \n
Test si les roquettes heurtent un mur pour disparaitre et faire disparaitre le mur. \n
Gere la teleportation
*/
void BOSS::update(Personnage &perso){
    if(timer - timerR >= cadenceR)tirerRoquette(perso.pos);

    for(unsigned int i = 0; i < tabRoquettes.size(); i++){ 
        avancerBalle(tabRoquettes[i],vRoquette);
        if(tabRoquettes[i].posBalle.getX() == perso.pos.getX() &&
           tabRoquettes[i].posBalle.getY() == perso.pos.getY()) {
            perdreViePerso(50);
            tabRoquettes.erase(tabRoquettes.begin()+i);
            i--;
        }
        else if(tabRoquettes[i].posBalle.getX() <= -1 ||
                tabRoquettes[i].posBalle.getX() >= ter.getDimx() ||
                tabRoquettes[i].posBalle.getY() <= -1 ||
                tabRoquettes[i].posBalle.getY() >= ter.getDimy() ){

               tabRoquettes.erase(tabRoquettes.begin()+i);
               i--;
        }  
        else if(ter.getMap(tabRoquettes[i].posBalle.getX(),
                tabRoquettes[i].posBalle.getY()) == MUR){

            ter.modifTer(tabRoquettes[i].posBalle.getX(), 
            tabRoquettes[i].posBalle.getY(),CHEMIN);
            tabRoquettes.erase(tabRoquettes.begin()+i);
            i--;
        } 
    }
    for (unsigned int i=0;i<5;i++) {
        if(perso.pos == tabTelep[i].pos) {
            perso.pos = tabTelep[tabTelep[i].numNext].pos; 
            perso.pos.haut(ter);
            if (i==4) initTelep();
        }
    }

    vector<Position2D> positionsBoss;
    positionsBoss.push_back(boss);
    positionsBoss.push_back(Position2D(boss.getX()+1, boss.getY()));
    positionsBoss.push_back(Position2D(boss.getX(), boss.getY()+1));
    positionsBoss.push_back(Position2D(boss.getX()+1, boss.getY()+1));
    if (!onde.saut) {
        for (int i=0;i<4;i++) {
            if (positionsBoss[i] == perso.pos)
                perdreViePerso(100);
        }
    }

    if (onde.saut) {
        if(changePos && timer-onde.debut >2500) {
            int x,y;
            do {
                x=rand()%(ter.getDimx()-1);
                y=rand()%(ter.getDimy()-1);
            } while(ter.getMap(x,y)==VIDE || ter.getMap(x+1,y)==VIDE ||
                    ter.getMap(x,y+1)==VIDE || ter.getMap(x+1,y+1)==VIDE);
            boss.setX(x);
            boss.setY(y);
            changePos = false;
        }
        else if (timer-onde.debut>5000)
            lancerOnde();
    }
    if (onde.onde) {
        if(timer-onde.debut>500) updateOnde();
        for(list<Position2D>::iterator it=onde.ondeH.begin();
        it!=onde.ondeH.end();++it) {
            if(it->getValPos(ter)==MUR)
                ter.modifTer(it->getX(),it->getY(),CHEMIN);
            if(*it==perso.pos) perdreViePerso(100);
        }
        for(list<Position2D>::iterator it=onde.ondeB.begin();
        it!=onde.ondeB.end();++it) {
            if(it->getValPos(ter)==MUR)
                ter.modifTer(it->getX(),it->getY(),CHEMIN);
            if(*it==perso.pos) perdreViePerso(100);
        }
        for(list<Position2D>::iterator it=onde.ondeG.begin();
        it!=onde.ondeG.end();++it) {
            if(it->getValPos(ter)==MUR)
                ter.modifTer(it->getX(),it->getY(),CHEMIN);
            if(*it==perso.pos) perdreViePerso(100);
        }
        for(list<Position2D>::iterator it=onde.ondeD.begin();
        it!=onde.ondeD.end();++it) {
            if(it->getValPos(ter)==MUR)
                ter.modifTer(it->getX(),it->getY(),CHEMIN);
            if(*it==perso.pos) perdreViePerso(100);
        }
    }

    if (invincible && timer-timerInvincible > 2000) invincible=false;

    if(timer - timerF >= 10000) poserFleur();
    else if(timer - timerF >= 5000 && posF.size() > 0){
       ter.modifTer(posF[0].getX(), posF[0].getY(), CHEMIN);    
       posF.erase(posF.begin());
    }

    if(posF.size() > 0){
        if(perso.pos.getX() == posF[0].getX() &&
            perso.pos.getY() == posF[0].getY()){

            setFeu(true);
            tFeu = timer;
            ter.modifTer(posF[0].getX(), posF[0].getY(), CHEMIN);
            posF.erase(posF.begin());
        }
    }

    if(bouleF.size() > 0 ){
        
        for(unsigned int g = 0; g < bouleF.size(); g++){
            avancerBalle(bouleF[g],vFeu);
            if(((bouleF[g].posBalle.getX() - tmp.getX() >= 5 ) ||
               (bouleF[g].posBalle.getX() - tmp.getX() <= -5) ||
               (bouleF[g].posBalle.getY() - tmp.getY() >= 5) ||
               (bouleF[g].posBalle.getY() - tmp.getY() <= -5)) ) {

                bouleF.erase(bouleF.begin()+g);
                g--;

            }

            else if(ter.getMap(bouleF[g].posBalle.getX(),bouleF[g].posBalle.getY())
               == MUR){

               bouleF.erase(bouleF.begin()+g);
               g--;
            }

            else {
                for (int i=0;i<4;i++) {
                    if (positionsBoss[i] == bouleF[g].posBalle) {
                        perdreVieBOSS(10);
                        bouleF.erase(bouleF.begin()+g);
                        g--;
                        break;
                    }
                } 
            }

        } 

        if(timer - tFeu >= 6000){
          setFeu(false);
        }

    }

}


/** 
\fn void BOSS::tirerRoquette(const Position2D &p)
@brief
Génère aléatoirement un nombre compris entre 0 et 3 (inclus) désignant le sens de la roquette,
et gère sa position de départ en fonction de ce dernier.
*/
void BOSS::tirerRoquette(const Position2D &p){
    srand(time(NULL));
    timerR = timer;
    for (int i=0;i<3;i++) {
        Balles B;
        B.timerB = timer;
        B.sens = rand()%4;
        switch(B.sens){
            case 0: // haut
                B.posBalle.setX(p.getX()+(rand()%11 - 5));
                B.posBalle.setY(0);
                B.vitessePos = (float)vRoquette/ter.getDimy();
                break;
            case 1: // bas
                B.posBalle.setX(p.getX()+(rand()%11 - 5));
                B.posBalle.setY(ter.getDimy() - 1);
                B.vitessePos = (float)vRoquette/ter.getDimy();
                break;
            case 2: // gauche
                B.posBalle.setX(0);
                B.posBalle.setY(p.getY()+(rand()%11 - 5));
                B.vitessePos = (float)vRoquette/ter.getDimx();
                break;
            case 3: // droit
                B.posBalle.setX(ter.getDimx() - 1);
                B.posBalle.setY(p.getY()+(rand()%11 - 5));
                B.vitessePos = (float)vRoquette/ter.getDimx();
                break;
            default:
                break;
        }
        tabRoquettes.push_back(B);
    }
}

/** 
\fn void BOSS::avancerBalle(Balles &B)
\param Prend en paramètre une Balle en mode données/résultat
@brief
Gère la position des balles selon leur sens.
*/
void BOSS::avancerBalle(Balles &B,unsigned int v) {
    switch(B.sens){
        case 0: // depuis haut
 
            if(timer - B.timerB >= (unsigned int)(v/ter.getDimy())) {
                B.posBalle.setY(B.posBalle.getY()+1);
                B.timerB = timer;
            }
            break;
        case 1:// depuis bas

            if(timer - B.timerB >=(unsigned int)(v/ter.getDimy())) {
                B.posBalle.setY(B.posBalle.getY()-1);
                B.timerB = timer;
            }
            break;
        case 2: // depuis gauche

            if(timer - B.timerB >= (unsigned int)(v/ter.getDimx())) {
                B.posBalle.setX(B.posBalle.getX()+1);
                B.timerB = timer;
            }
            break;
        case 3:// depuis droite  
     
            if(timer - B.timerB >=(unsigned int)(v/ter.getDimx())) {
                B.posBalle.setX(B.posBalle.getX()-1);
                B.timerB = timer;
            } 
            break;  
    }
}

/** 
\fn void BOSS::construire(const Position2D &pos, int s)
\param Pren en paramètres une position2D en mode données/résultat
designant la position du personnage, et un entier désignant
la direction où sera placé le mur. 

@brief
Gère la construction d'un mur par le joueur sur la map.
*/
void BOSS::construire(const Position2D &pos, int s){
    
    int x,y;

    vector<Position2D> positionsBoss;
    positionsBoss.push_back(boss);
    positionsBoss.push_back(Position2D(boss.getX()+1, boss.getY()));
    positionsBoss.push_back(Position2D(boss.getX(), boss.getY()+1));
    positionsBoss.push_back(Position2D(boss.getX()+1, boss.getY()+1));

    switch(s){
        case 0:
            x=pos.getX();
            y=pos.getY()-1;
            break; 
        case 1:
            x=pos.getX();
            y=pos.getY()+1;
            break;
        case 2:
            x=pos.getX()-1;
            y=pos.getY();
            break;
        case 3:
            x=pos.getX()+1;
            y=pos.getY();
            break;
        default: return;
    }
    bool dansBoss=false;
    for (int i=0;i<4;i++) {
        if (positionsBoss[i].getX()==x && positionsBoss[i].getY()==y) {
            dansBoss = true;
        }
    }
    if (ter.estLibre(x,y) && !dansBoss) {
        ter.modifTer(x,y,MUR);
    }
}

/** 
\fn void BOSS::taper(const Position2D &pos, int s)
\param Pren en paramètres une position2D en mode données/résultat
designant la position du personnage, et un entier désignant
la direction du coup. 

@brief
Donne un coup d'epee dans une direction donnée.
*/
void BOSS::taper(const Position2D &pos, int s){
    bool taper;
    switch(s){
        case 0:
            taper = ((pos.getX()==boss.getX() || pos.getX()==boss.getX()+1)
                && (pos.getY()-1==boss.getY() || pos.getY()-1==boss.getY()+1)); 
            break;
        case 1:
            taper = ((pos.getX()==boss.getX() || pos.getX()==boss.getX()+1)
                && (pos.getY()+1==boss.getY() || pos.getY()+1==boss.getY()+1)); 
            break; 

        case 2:
            taper=(pos.getX()-1==boss.getX() || pos.getX()-1==boss.getX()+1)
                && (pos.getY()==boss.getY() || pos.getY()==boss.getY()+1);
            break;

        case 3:
            taper = ((pos.getX()+1==boss.getX() || pos.getX()+1==boss.getX()+1)
                && (pos.getY()==boss.getY() || pos.getY()==boss.getY()+1));
            break; 
    }
    if (taper) {
        perdreVieBOSS(100);
    }
}

/** 
\fn void BOSS::perdreViePerso(int degats)
\param prends en paramètre les dégats pris par le personnage
@brief
Enlève de la vie au personnage et le fait devenir invincible pendant 2 secondes.
*/
void BOSS::perdreViePerso(int degats) {
    assert(degats>=0);
    if (!invincible) {
        invincible = true;
        timerInvincible = timer;
        viePerso-=degats;
    }
}

/** 
\fn void BOSS::perdreVieBOSS(int degats)
\param prends en paramètre les dégats pris par le boss
@brief
Enlève de la vie au boss et le fait sauter si necessaire
*/
void BOSS::perdreVieBOSS(int degats) {
    assert(degats>=0);
    if (!onde.saut) {
        vieBOSS-=degats;
        timerBoss=timer;
        if (onde.vieDernierSaut-vieBOSS >= 500) sauter();
    }
}

/** 
\fn void BOSS::blinkPerso()
\return renvoie true s'il ne faut pas afficher le perso 
*/
bool BOSS::blinkPerso() {
    return (invincible && timer%2 == 0);
}

/** 
\fn void BOSS::blinkBoss()
\return renvoie true s'il ne faut pas afficher le boss 
*/
bool BOSS::blinkBoss() {
    return (timer-timerBoss<1000 && timer%2 == 0);
}

/** 
\fn void BOSS::sauter()
@brief
Fait sauter le boss pour lancer une onde de choc
*/
void BOSS::sauter() {
    if (!onde.saut && !onde.onde) {
        onde.saut=true;
        onde.vieDernierSaut=vieBOSS;
        onde.debut=timer;
        if (vieBOSS <= 1500) changePos=true;
    }
}

/** 
\fn void BOSS::lancerOnde()
@brief
Demarre l'onde de choc (après l'atterissage du boss)
*/
void BOSS::lancerOnde() {
    onde.saut=false;
    onde.onde=true;
    onde.debut=timer;

    onde.ondeH.push_back(Position2D(boss.getX(),boss.getY()));
    onde.ondeH.push_back(Position2D(boss.getX()+1,boss.getY()));

    onde.ondeB.push_back(Position2D(boss.getX(),boss.getY()+1));
    onde.ondeB.push_back(Position2D(boss.getX()+1,boss.getY()+1));

    onde.ondeG.push_back(Position2D(boss.getX(),boss.getY()));
    onde.ondeG.push_back(Position2D(boss.getX(),boss.getY()+1));

    onde.ondeD.push_back(Position2D(boss.getX()+1,boss.getY()));
    onde.ondeD.push_back(Position2D(boss.getX()+1,boss.getY()+1));
}


/** 
\fn void BOSS::updateOnde()
@brief
Etends l'onde de choc sur les cases appropriées
*/
void BOSS::updateOnde() {
    onde.debut=timer;
    if(onde.ondeH.empty() && onde.ondeB.empty() &&
        onde.ondeG.empty() && onde.ondeD.empty()) {
        onde.onde=false;
    }
    else {
        int front, back, cons, size;
        size=onde.ondeH.size();
        if(size>0) {
            cons=onde.ondeH.front().getY()-1;
            front = onde.ondeH.front().getX();
            onde.ondeH.push_back(Position2D(front-1,cons));
            onde.ondeH.push_back(Position2D(front,cons));
            onde.ondeH.push_back(Position2D(front+1,cons));
            onde.ondeH.pop_front();
        } 
        for(int i=1;i<size;i++) { 
            front = onde.ondeH.front().getX();
            back = onde.ondeH.back().getX();
            onde.ondeH.pop_front();
            if(back<front-1) {
                onde.ondeH.push_back(Position2D(front-1,cons));
                onde.ondeH.push_back(Position2D(front,cons));
                onde.ondeH.push_back(Position2D(front+1,cons));
            }
            else if (back<front) {
                onde.ondeH.push_back(Position2D(front,cons));
                onde.ondeH.push_back(Position2D(front+1,cons));
            }
            else onde.ondeH.push_back(Position2D(front+1,cons));
        }

        size=onde.ondeB.size();
        if(size>0) {
            cons=onde.ondeB.front().getY()+1;
            front = onde.ondeB.front().getX();
            onde.ondeB.push_back(Position2D(front-1,cons));
            onde.ondeB.push_back(Position2D(front,cons));
            onde.ondeB.push_back(Position2D(front+1,cons));
            onde.ondeB.pop_front();
        } 
        for(int i=1;i<size;i++) { 
            front = onde.ondeB.front().getX();
            back = onde.ondeB.back().getX();
            onde.ondeB.pop_front();
            if(back<front-1) {
                onde.ondeB.push_back(Position2D(front-1,cons));
                onde.ondeB.push_back(Position2D(front,cons));
                onde.ondeB.push_back(Position2D(front+1,cons));
            }
            else if (back<front) {
                onde.ondeB.push_back(Position2D(front,cons));
                onde.ondeB.push_back(Position2D(front+1,cons));
            }
            else onde.ondeB.push_back(Position2D(front+1,cons));
        }

        size=onde.ondeG.size();
        if(size>0) {
            cons=onde.ondeG.front().getX()-1;
            front = onde.ondeG.front().getY();
            onde.ondeG.push_back(Position2D(cons,front-1));
            onde.ondeG.push_back(Position2D(cons,front));
            onde.ondeG.push_back(Position2D(cons,front+1));
            onde.ondeG.pop_front();
        } 
        for(int i=1;i<size;i++) { 
            front = onde.ondeG.front().getY();
            back = onde.ondeG.back().getY();
            onde.ondeG.pop_front();
            if(back<front-1) {
                onde.ondeG.push_back(Position2D(cons,front-1));
                onde.ondeG.push_back(Position2D(cons,front));
                onde.ondeG.push_back(Position2D(cons,front+1));
            }
            else if (back<front) {
                onde.ondeG.push_back(Position2D(cons,front));
                onde.ondeG.push_back(Position2D(cons,front+1));
            }
            else onde.ondeG.push_back(Position2D(cons,front+1));
        } 

        size=onde.ondeD.size();
        if(size>0) {
            cons=onde.ondeD.front().getX()+1;
            front = onde.ondeD.front().getY();
            onde.ondeD.push_back(Position2D(cons,front-1));
            onde.ondeD.push_back(Position2D(cons,front));
            onde.ondeD.push_back(Position2D(cons,front+1));
            onde.ondeD.pop_front();
        } 
        for(int i=1;i<size;i++) { 
            front = onde.ondeD.front().getY();
            back = onde.ondeD.back().getY();
            onde.ondeD.pop_front();
            if(back<front-1) {
                onde.ondeD.push_back(Position2D(cons,front-1));
                onde.ondeD.push_back(Position2D(cons,front));
                onde.ondeD.push_back(Position2D(cons,front+1));
            }
            else if (back<front) {
                onde.ondeD.push_back(Position2D(cons,front));
                onde.ondeD.push_back(Position2D(cons,front+1));
            }
            else onde.ondeD.push_back(Position2D(cons,front+1));
        }
        onde.debut=timer;
        clearOnde();
    }
}

/** 
\fn void BOSS::clearOnde()
@brief
Enleve de l'onde les cases qui sont hors map ou dans le vide
*/
void BOSS::clearOnde() {
    onde.ondeH.remove_if([this] (const Position2D& pos) 
    {return pos.getValPos(this->ter) == VIDE || pos.getValPos(this->ter) == '?';});
    onde.ondeB.remove_if([this] (const Position2D& pos) 
    {return pos.getValPos(this->ter) == VIDE || pos.getValPos(this->ter) == '?';});
    onde.ondeG.remove_if([this] (const Position2D& pos) 
    {return pos.getValPos(this->ter) == VIDE || pos.getValPos(this->ter) == '?';});
    onde.ondeD.remove_if([this] (const Position2D& pos) 
    {return pos.getValPos(this->ter) == VIDE || pos.getValPos(this->ter) == '?';});
}


/** 
\fn void BOSS::poserFleur()
@brief prend un position aléatoire libre sur la map et pose une fleur de puissance à
la position choisi.
*/
void BOSS::poserFleur(){
    Position2D p;
    do{
        p.setX(rand()%30);
        p.setY(rand()%18);
    }while(ter.getMap(p.getX(), p.getY()) != CHEMIN);
    posF.push_back(p);
    ter.modifTer(posF[0].getX(), posF[0].getY(), FLEUR);
    timerF = timer;
}

/** 
\fn void BOSS::tirerFeu(const Position2D &p, int s)
\param Prend en paramètre une position2D en mode données/résultat, et un entier désignant le sens de tir
@brief tire une boule de feu en lui donnant comme position de départ une position autour du personnage
selon le sens (s) de tir, et la met place dans un tableau dynamique.
*/
void BOSS::tirerFeu(const Position2D &p, int s){
    Balles B;
    B.timerB = timer;
    B.sens = s;
    switch(s){
        case 0:
            B.posBalle.setX(p.getX());
            B.posBalle.setY(p.getY()+1);
            B.vitessePos = (float)(vFeu/ter.getDimy());
            tmp.setX(p.getX());
            tmp.setY(p.getY()+1); 
            break;
        case 1:
            B.posBalle.setX(p.getX());
            B.posBalle.setY(p.getY()-1);
            B.vitessePos = (float)(vFeu/ter.getDimy());
            tmp.setX(p.getX());
            tmp.setY(p.getY()-1); 
            break;
        case 2:
            B.posBalle.setX(p.getX()+1);
            B.posBalle.setY(p.getY());
            B.vitessePos = (float)(vFeu/ter.getDimx());
            tmp.setX(p.getX()+1);
            tmp.setY(p.getY()); 
            break;

        case 3:
            B.posBalle.setX(p.getX()-1);
            B.posBalle.setY(p.getY());
            B.vitessePos = (float)(vFeu/ter.getDimx());
            tmp.setX(p.getX()-1);
            tmp.setY(p.getY()); 
            break;
    }
    bouleF.push_back(B);
}


/** 
\fn void BOSS::testReussie(Jeu &j)
\param Prend en paramètre un Jeu en mode données/résultat
@brief
Test si la manche est réussie ou non : \n
Si le perso n'a plus de vie, défaite; \n
Si le boss n'a plus de vie, victoire.
*/
void BOSS::testReussie(Jeu &j){
    if(enCours == false || viePerso<=0) Defaite(j);    
    
    else if(vieBOSS <= 0) {
        Victoire(j);
        tabRoquettes.clear();
        posF.clear();
        bouleF.clear();
    }
}

/** 
\fn int BOSS::testRegression() 
 \return retourne -1 en cas d'echec et 0 sinon;
@brief 
Effectue les tests de non regression
*/
int BOSS::testRegression() {
	cout<<"Tests sur le boss"<<endl;
	Lancement();
	cout<<"Tests d'initialisation"<<endl;
	Jeu j;
	init(j.perso);
	if (!tabRoquettes.empty() || !(tmp==j.perso.pos) ||
        !bouleF.empty() || !posF.empty()) {
			cout<<"Erreur d'initialisation"<<endl;
			return -1;
	}
	cout<<"Tests de déroulement de la manche"<<endl;
    setTimer(timer+50000);
    update(j.perso);
    if (tabRoquettes.empty()) {
        cout<<"Erreur lors du tir d'une roquette"<<endl;
        return -1;
    }
    if (posF.empty()) {
        cout<<"Erreur lors de la pose d'une fleur"<<endl;
        return -1;
    }
    j.perso.pos = boss;
    construire(j.perso.pos, 0);
    if (ter.getMap(boss.getX(),boss.getY()-1) != MUR) {
        cout<<"Erreur lors de la pose d'un mur"<<endl;
        return -1;
    }
    construire(j.perso.pos,1);
    if (ter.getMap(boss.getX(),boss.getY()+1) == MUR) {
        cout<<"Erreur, pose d'un mur sur le boss"<<endl;
        return -1;
    }
    taper(j.perso.pos, 1);
    if (vieBOSS == 3000) {
        cout<<"Erreur lors du coup au boss"<<endl;
        return -1;
    }
    update(j.perso);
    if (viePerso > 100 && !invincible) {
        cout<<"Erreur lors de la prise de dégats en etant sur le boss"<<endl;
        return -1;
    }
    j.perso.pos.haut(ter);
    invincible = false;
    int vB = vieBOSS;
    int vP = viePerso;
    perdreVieBOSS(1000);
    if (vieBOSS != vB - 1000 && !onde.saut) {
        cout<<"Erreur lors de la prise de dégats par le boss"<<endl;
        return -1;
    }
    perdreViePerso(2);
    if (viePerso != vP - 2 || !invincible) {
        cout<<"Erreur lors de la prise de dégats par le perso"<<endl;
        return -1;
    }
    
    setTimer(timer+10000);
    update(j.perso);
    if (!onde.onde || onde.ondeB.size() != 2 || onde.ondeH.size() != 2 ||
        invincible || onde.ondeG.size() != 2 || onde.ondeD.size() != 2) {
        cout<<"Erreur au lancement de l'onde"<<endl;
        return -1;
    }
    setTimer(timer + 1000);
    update(j.perso);
    if (viePerso > 0) {
        cout<<"Erreur lors de la prise de dégats par l'onde"<<endl;
        return -1;
    }
    testReussie(j);
	if (enCours) {
		cout<<"Erreur lors de la défaite"<<endl;
		return -1;
	}
    init(j.perso);
    if (!tabRoquettes.empty() || !(tmp==j.perso.pos) ||
        !bouleF.empty() || !posF.empty()) {
			cout<<"Erreur d'initialisation"<<endl;
			return -1;
	}
    setFeu(true);
    j.perso.pos = Position2D(boss.getX(), boss.getY()+3);
    tirerFeu(j.perso.pos, 1);
	setTimer(j.getTimer()+1000);
    update(j.perso);
    setTimer(j.getTimer()+1000);
    update(j.perso);
    if (vieBOSS==3000) {
        cout<<"Erreur lors du tir de boule de feu"<<endl;
        return -1;
    }
    perdreVieBOSS(3000);
	testReussie(j);
	if (!reussie) {
		cout<<"Erreur lors de la victoire de la manche"<<endl;
		return -1;
	}
	cout<<"Tests sur le Boss tous passés"<<endl;
	return 0;
}