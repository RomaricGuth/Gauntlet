#ifndef POSITION2D_H
#define POSITION2D_H

#include "Terrain.h"


/** 
\class Position2D
@brief
Classe Position2D qui gère : \n
Le déplacement des objets; \n
L'affectation d'une position à un objet.
*/
class Position2D {

    private:

        int x;
        int y;

    public:

        int getX() const;
        int getY() const;
        void setX(int x);
        void setY(int y);
        char getValPos(const Terrain &t) const;
        void gauche (const Terrain & t);
        void droite (const Terrain & t);
        void haut (const Terrain & t);
        void bas (const Terrain & t);

        void operator= (Position2D p2);
        bool operator== (Position2D p3);
        
        Position2D();
        Position2D(int nx, int ny);
        ~Position2D();

        int testRegression();
};

#endif