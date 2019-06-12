#include "Terrain.h"
#include <cassert>
#include <iostream>
#include <string>
#include<fstream>
using namespace std;

/** 
\fn int Terrain::getDimx() const
\return Renvoie le nombre de colonnes du tableau.
*/
int Terrain::getDimx() const {
    return dimx;
}

/** 
\fn int Terrain::getDimy() const
\return renvoie le nombre de lignes du tableau.
*/
int Terrain::getDimy() const {
    return dimy;
}

/** 
\fn char Terrain::getMap(int x, int y) const
\param Prend en paramètres deux entiers correspondants aux indices du tableau.
\return Renvoie le charactère du tableaux aux indices x,y.
*/
char Terrain::getMap(int x, int y) const {
    if(x<0 || y<0 || x>=dimx || y>=dimy) return '?';
    return mapTxt[x][y];
}

/** 
\fn bool Terrain::estLibre(int x, int y) const
\param Prend en paramètres deux entiers correspondants aux indices du tableau.
\return un booléen attestant de la liberté d'une case du tableau ou non.
*/
bool Terrain::estLibre(int x, int y) const {
    if (x <0 || x >= dimx || y < 0 || y >= dimy) return false;
    char t = mapTxt[x][y];
    return (t!=MUR && t!=VIDE);
}

/** 
\param Prend en paramètre un entier désignant le numéro du terrain à construire.
@brief
Constructeur de la classe. Le terrain est construit en recopiant un fichier.
*/
Terrain::Terrain(int indM) {
    reinit(indM);
}

/** 
@brief Destructeur de la classe.
*/
Terrain::~Terrain() {
    
}

/** 
\fn void Terrain::reinit(int indM)
\param Prend en paramètre un entier désignant le numéro du terrain à construire.
@brief
Reinitialise le terrain tel qu'il est dans le fichier
*/
void Terrain::reinit(int indM) {
    {
    ifstream file;
    switch (indM) {
        case -1: file.open("data/terrainMain.txt"); break;
        case 3: file.open("data/terrain3.txt"); break;
        case 4: file.open("data/terrain4.txt"); break;
        case 5: file.open("data/terrainBoss.txt"); break;
        default: cerr<<"Mauvais indice à l'initialisation du terrain"<<endl;   
    }
    assert(file);
    string mot;
    file>>dimx>>dimy;
    for(int j=0;j<dimy;j++) {
        for (int i=0;i<dimx;i++) {
            file>>mot;
            if (mot=="CHEMIN") mapTxt[i][j] = CHEMIN;
            else if (mot=="MUR") mapTxt[i][j] = MUR;
            else if (mot=="VIDE") mapTxt[i][j] = VIDE;
            else if (mot=="ARRIVEE") mapTxt[i][j] = ARRIVEE;
            else if (mot=="FLEUR") mapTxt[i][j] = FLEUR;
            else if (mot.length()==1) mapTxt[i][j] = mot.front();
        }
    }
    file.close();
    }
}

/** 
\fn void Terrain::afficheTer() const
@brief Affiche le terrain en version console
*/
void Terrain::afficheTer() const {
    cout<<"Le terrain en version texte"<<endl;
    for(int j=0;j<dimy;j++) {
        for (int i=0;i<dimx;i++) {
            cout<<mapTxt[i][j];
        }
        cout<<endl;
    }
}

/** 
\fn void Terrain::modifTer(int i, int j, char val)
\param Prend en paramètre 2 entiers désignants la cellule du tableau à modifier, et une chaine de caractère
à affecter à la cellule précédente.
*/
void Terrain::modifTer(int i, int j, char val){
    mapTxt[i][j] = val;
}
/** 
\fn int Terrain::testRegression()
\return Renvoie 0 ou -1 selon le bon fonctionnement de la classe.
*/
int Terrain::testRegression() {
    cout<<endl<<"Tests sur la classe Terrain"<<endl;
    cout<<endl<<"Verification des getters/setters"<<endl;
    if (getDimx() != dimx) return -1;
    if (getDimy() != dimy) return -1;
    for (int j=0;j<dimy;++j) {
        for (int i=0;i<dimx;++i) {
            if (getMap(i,j)!=mapTxt[i][j]) return -1;
            if (mapTxt[i][j]==MUR && estLibre(i,j)==true) return -1;
            else if (mapTxt[i][j]!= MUR && estLibre(i,j)==false) return -1;
        }
    }

    cout<<endl<<"Verification de l'initialisation du terrain"<<endl;
    for (int j=0;j<dimy;++j) {
        for (int i=0;i<dimx;++i) {
            if (sizeof(mapTxt[i][j])!=sizeof(char)) return -1;
        }
    }
    cout<<endl<<"Voici le terrain tel qu'il sera affiche"<<endl;
    afficheTer();
    cout<<endl<<"Tests sur le terrain tous passés"<<endl;
    return 0;
}