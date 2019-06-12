#ifndef BOSS_H
#define BOSS_H
#include <string>
#include "Manche.h"
#include "Terrain.h"
#include "Position2D.h"
#include "Manche5.h"
#include "Manche4.h"
#include <vector>
#include <list>

/**
\struct Onde
@brief
Structure représentant les ondes de chocs déclenchées par le boss. \n
*/
struct Onde {      
    bool saut;
    bool onde;
    unsigned int debut;
    int vieDernierSaut;
    list<Position2D> ondeH;
    list<Position2D> ondeB;
    list<Position2D> ondeG;
    list<Position2D> ondeD; 
};

/**
\class BOSS
@brief
Classe BOSS qui hérite de la classe Manches pour permettre l'utilisation de paramètres globaux
à toutes les manches. \n
Elle gère : \n
Une génération aléatoire de chiffres (correspondants à sa position de départ et sa direction); \n
La gestion dynamique du nombre de roquettes à l'aide d'un tableau dynamique; \n
Le tir et le déplacement en temps réel de chaques roquettes; \n
La possibilité de construire un mur sur le terrain; \n
La possibilité de subir des dégâts (par des roquettes ou le boss); \n
La victoire et la défaite de la manche.
*/
class BOSS : public Manches{

    private:

        bool changePos;
        bool invincible;
        char sens;
        unsigned int timerInvincible;
        unsigned int timerBoss;
        int vieBOSS;
        int viePerso;
        unsigned int cadenceR;
        unsigned int timerR;
        unsigned int tFeu;
        unsigned int vRoquette;
        unsigned int vFeu;
        Porte tabTelep[5];
        unsigned int timerF;
        Position2D tmp;
        bool feu;
        

    public:

        vector<Balles> tabRoquettes;
        Terrain ter;
        Position2D boss;
        vector<Position2D> posF;
        vector<Balles> bouleF;
        Position2D tmpPerso;

        Onde onde;
        
        bool getFeu()const;
        void setFeu(bool f);
        int getVieBOSS() const;
        char getSens() const;
        void setVieBOSS();
        int getViePerso()const;
        void setViePerso();
        void setSens(char s);

        bool blinkPerso();
        bool blinkBoss();
        void teleportation();
        void tirerRoquette(const Position2D &p);
        void avancerBalle(Balles &B, unsigned int v);
        void sauter();
        void lancerOnde();
        void updateOnde();
        void clearOnde();
        void ondeChoc();
        void poserFleur();
        void tirerFeu(const Position2D &p, int s);
        void construire(const Position2D &pos, int s);
        void taper(const Position2D &pos, int s);
        void perdreViePerso(int degats);
        void perdreVieBOSS(int degats);
        void init(Personnage &perso);
        void initTelep();
        void update(Personnage &perso);
        void testReussie(Jeu &j);
        int testRegression();

        BOSS(Personnage &perso);
        ~BOSS();
};

#endif

