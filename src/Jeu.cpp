#include "Jeu.h"
#include <iostream>
using namespace std;

/** 
\fn unsigned int Jeu::getTimer() const
\return renvoie la valeur courante du timer
*/
unsigned int Jeu::getTimer() const {
    return timer;
}

/**
\fn bool Jeu::getManche(int ind) const
\param indice de manche
\return renvoie le booléen contenu dans le tableau 
de manches
*/
bool Jeu::getManche(int ind) const {
    return manches[ind];
}

/**
\fn int Jeu::getNbReussie() const
\return renvoie le nombre de manches réussies
*/
int Jeu::getNbReussie() const {
    return nbReussie;
}

/**
\fn void Jeu::setTimer(unsigned int time)
@brief
permet la mise à jour du timer
\param valeur du temps en milisecondes
*/
void Jeu::setTimer(unsigned int time) {
    timer = time;
}

/** 
@brief constructeur de la classe Jeu : 
initialise le tableau de manches à faux
*/
Jeu::Jeu() : ter(-1) {
    nbReussie = 0;
    timer = 0;
    for (int i=0;i<NB_MANCHES;i++)
    {
        manches[i] = false;
    }
}

/**
@brief
 destructeur de la classe jeu */
Jeu::~Jeu() {

}

/**
\fn void Jeu::MancheReussie(int ind)
\param indice de manche 
@brief passe à vrai le tableau de manches à l'indice passé en paramètre
*/
void Jeu::MancheReussie(int ind) {
    nbReussie++;
    manches[ind] = true;
}

/** 
\fn void Jeu::debutPartie() const 
@brief lance la partie et affiche le terrain initial
*/
void Jeu::debutPartie() const {
    cout<<"La partie commence !! Bonne chance !"<<endl;
    ter.afficheTer();
}

/** 
\fn void Jeu::finPartie() const
@brief 
met fin à la partie : \n
si le joueur n'a plus de vie, il perd \n
si le joueur atteint l'arrivé, il gagne \n
*/
void Jeu::finPartie() const {
    if (perso.getVies() == 0) defaite();
    else if (perso.pos.getValPos(ter) == 'A') victoire();
    else cout<<"Partie quittee"<<endl;
    cout<<"La partie est finie"<<endl;
}

/** 
\fn void Jeu::victoire() const
@brief affiche message de victoire, et le temps mis
*/
void Jeu::victoire() const {
    cout<<"Bravo, vous avez atteint l'objectif final en ";
    cout<<timer/1000<<" secondes !"<<endl;
}

/**
\fn void Jeu::defaite() const 
@brief affiche message de defaite, le timer et le nombre de manche(s) réussie(s)
*/
void Jeu::defaite() const {
    cout<<"Vous avez perdu :("<<endl<<"nombre de manches réussies : ";
    cout<<nbReussie<<endl<<"temps : "<<timer/1000<<" secondes"<<endl;
}

/** \fn int Jeu::testRegression()
@brief 
fonction de test de bon fonctionnement général de la classe jeu
\return Renvoie 0 ou -1 selon le bon fonctionnement de la manche
*/
int Jeu::testRegression() {

    cout<<"Debut du test sur le corps du jeu"<<endl;

    cout<<endl<<"Tests sur la classe Jeu"<<endl;
    cout<<endl<<"Verification des getters/setters"<<endl;
    if (getNbReussie()!=nbReussie) return -1;
    if (getTimer() != timer) return -1;
    for (int i=0;i<NB_MANCHES;++i) {
        if (getManche(i) != manches[i]) return -1;
    }

    cout<<"Verification de l'initialisation du jeu"<<endl; 

    for (int i=0;i<NB_MANCHES;i++) {
        if (manches[i]==true) {
            cout<<"Erreur : manche initialisee a true"<<endl;
            return -1;
        }
    }
    cout<<endl<<"Tests sur le Jeu tous passes"<<endl;
    if (ter.testRegression() == -1) {
        cout<<"erreur lors des verifications sur le terrain"<<endl;
        return -1;
    }
    if (perso.testRegression() == -1) {
        cout<<"erreur lors des verifications sur le personnage"<<endl;
        return -1;
    }

    cout<<endl<<endl<<"Test : SIMULATION D'UNE PARTIE"<<endl;
    debutPartie();
    cout<<"Verifications des mouvements du personnage"<<endl;
    if (perso.pos.getValPos(ter) != CHEMIN) {
        cout<<"Erreur de recuperation de la case actuelle"<<endl;
        return -1;
    }
    int tx, ty, x, y;
    x=perso.pos.getX();
    y=perso.pos.getY();

    tx = x;
    ty = y;
    perso.pos.haut(ter);
    x = perso.pos.getX();
    y = perso.pos.getY();
    cout<<x<<" "<<y<<endl<<tx<<" "<<ty<<endl;
    if (x!=tx) return -1;
    if (y==ty && ter.estLibre(tx,ty-1)) return -1;
    if (y!=ty && y!=ty-1) return -1;
    if (perso.pos.getValPos(ter)==MUR) return -1;

    tx = x;
    ty = y;
    perso.pos.bas(ter);
    x = perso.pos.getX();
    y = perso.pos.getY();
    if (x!=tx) return -1;
    if (y==ty && ter.estLibre(tx,ty+1)) return -1;
    if (y!=ty && y!=ty+1) return -1;
    if (perso.pos.getValPos(ter)==MUR) return -1;

    tx = x;
    ty = y;
    perso.pos.gauche(ter);
    x = perso.pos.getX();
    y = perso.pos.getY();
    if (y!=ty) return -1;
    if (x==tx && ter.estLibre(tx-1,ty)) return -1;
    if (x!=tx && x!=tx-1) return -1;
    if (perso.pos.getValPos(ter)==MUR) return -1;

    tx = x;
    ty = y;
    perso.pos.droite(ter);
    x = perso.pos.getX();
    y = perso.pos.getY();
    if (y!=ty) return -1;
    if (x==tx && ter.estLibre(tx+1,ty)) return -1;
    if (x!=tx && x!=tx+1) return -1;
    if (perso.pos.getValPos(ter)==MUR) return -1;

    cout<<endl<<"Validation des manches"<<endl;
    for (int i=0;i<NB_MANCHES;++i) {
        MancheReussie(i);
        if (manches[i]==false) {
            cout<<"Erreur dans la validation des manches"<<endl;
            return -1;
        }
    }
    perso.pos.setX(ter.getDimx()-1);
    perso.pos.setY(ter.getDimy()-1);
    cout<<endl<<"Doit afficher que la partie est gagnee :"<<endl;
    finPartie();
    perso.perdreVie();
    perso.perdreVie();
    cout<<endl<<"Doit afficher que la partie est perdue :"<<endl;
    finPartie();
    cout<<endl<<endl<<"Tous les tests ont ete passes avec succes"<<endl;
    return 0;
}
