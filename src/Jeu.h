#ifndef JEU_H
#define JEU_H

#include "Personnage.h"
#include "Terrain.h"

const int NB_MANCHES = 6;

/** 
\class Jeu
@brief
Classe Jeu qui gère :\n
le nombre de vie(s) du personnage;\n
le timer global; \n
le debut de partie; \n
la fin de partie; \n
la victoire d'une manche; \n
la defaite d'une manche \

*/
class Jeu {
    
    private : 
    
        unsigned int timer;
        int nbReussie;
        bool manches[NB_MANCHES];

        
    public:

        Personnage perso;
        Terrain ter;
    
    //methodes 

        unsigned int getTimer() const;
        int getNbReussie() const;
        bool getManche(int ind) const;

        void setTimer(unsigned int time);

        Jeu();
        ~Jeu();

        void MancheReussie(int ind);
        void finPartie() const;
        void debutPartie() const;

        int testRegression();

    private:
    
        void victoire() const;
        void defaite() const;
};

#endif
