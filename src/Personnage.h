#ifndef PERSONNAGE_H
#define PERSONNAGE_H

#include "Position2D.h"

/** 
\class Personnage
@brief
Classe Personnage qui gère : \n
La position du personnage; \n
Le nombre de vies du personnage.
*/
class Personnage {
    
    private:

        int vies;
        

    public:  

        Position2D pos;

        int getVies() const;

        void perdreVie();

        Personnage();
        ~Personnage();

        int testRegression();
};

#endif