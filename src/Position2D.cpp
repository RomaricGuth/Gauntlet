#include "Position2D.h"
#include <iostream>
using namespace std;

/** 
\fn int Position2D::getX() const
\return Renvoie l'abscisse de la position2D.
*/
int Position2D::getX() const {
    return x;
}

/** 
\fn int Position2D::getY() const
\return Renvoie l'ordonnée de la position2D.
*/
int Position2D::getY() const {
    return y;
}

/** 
\fn void Position2D::setX(int nx)
\param Prend en paramètre la nouvelle valeur à effecter à l'abscisse de la position2D.
*/
void Position2D::setX(int nx) {
    x = nx;
}

/** 
\fn void Position2D::setY(int ny)
\param Prend en paramètre la nouvelle valeur à effecter à l'ordonnée de la position2D.
*/
void Position2D::setY(int ny) {
    y = ny;
}

/** 
\fn char Position2D::getValPos(const Terrain &t) const
\param Prend en paramètre un terrain constant en mode données/résultat
\return Renvoie la position du personnage sur le terrain.
*/
char Position2D::getValPos(const Terrain &t) const {
    return t.getMap(x,y);
}

/** 
\fn void Position2D::gauche (const Terrain & t)
\param Prend en paramètre un terrain constant en mode données/résultat
@brief Si la case à gauche du personnage est libre, on y déplace le personnage
*/
void Position2D::gauche (const Terrain & t) {
	if (t.estLibre(x-1,y)) x--;
}

/** 
\fn void Position2D::droite (const Terrain & t)
\param Prend en paramètre un terrain constant en mode données/résultat
@brief Si la case à droite du personnage est libre, on y déplace le personnage
*/
void Position2D::droite (const Terrain & t) {
	if (t.estLibre(x+1,y)) x++;
}

/**
\fn void Position2D::haut (const Terrain & t) 
\param Prend en paramètre un terrain constant en mode données/résultat
@brief Si la case en haut du personnage est libre, on y déplace le personnage
*/
void Position2D::haut (const Terrain & t) {
	if (t.estLibre(x,y-1)) y--;
}

/** 
\fn void Position2D::bas(const Terrain & t)
\param Prend en paramètre un terrain constant en mode données/résultat
@brief Si la case en bas du personnage est libre, on y déplace le personnage
*/
void Position2D::bas(const Terrain & t) {
	if (t.estLibre(x,y+1)) y++;
}

/** 
\fn void Position2D::operator= (Position2D p2)
\param Prend en paramètre une Position2D
@brief surcharge de l'opérateur "="
*/
void Position2D::operator= (Position2D p2) {
    x=p2.getX();
    y=p2.getY();
}

/** 
\fn bool Position2D::operator== (Position2D p3)
\param Prend en paramètre une Position2D
\return Renvoie un booléen
@brief Surcharge de l'opérateur "=="
*/
bool Position2D::operator== (Position2D p3){
     return (x == p3.getX() && y == p3.getY());
}

/** 
@brief
Constructeur de la classe.
*/
Position2D::Position2D() {
    x=0;
    y=0;
}

/** 
\param Prend en paramètres deux entiers 
@brief Constructeur de la classe. \n
Crée une Position2D en fonction des valeurs passées en paramètres.
*/
Position2D::Position2D(int nx, int ny) {
    x=nx;
    y=ny;
}

/** 
@brief
Destructeur de la classe.
*/
Position2D::~Position2D() {

}

/** 
\fn int Position2D::testRegression()
return Renvoie 0 ou -1 selon le bon fonctionnement de la classe.
*/
int Position2D::testRegression() {
    cout<<endl<<"Tests sur la classe position"<<endl;
    cout<<"Verification de l'initialisation de la position"<<endl;
    if (x!=0 || y!=0) return -1;
    cout<<"Verification des getters/setters"<<endl;
    if (getX() != x || getY() != y) return -1;
    cout<<endl<<"Tests sur la position tous passes"<<endl;
    return 0;
}