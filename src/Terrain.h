#ifndef TERRAIN_H
#define TERRAIN_H


const char CHEMIN = ' ';
const char MUR = '/';
const char ARRIVEE = 'A';
const char VIDE = '!';
const char FLEUR = 'F';

/** 
\classe Terrain
@brief
Classe terrain qui gère : \n
L'affichage des différents terrains; \n
La vérification de la liberté d'une cellule du terrain; \n
La possibilité de modifié un terrain (dans la mesure où celui-ci n'est pas un const).
*/
class Terrain {
    
    private:

        int dimx;
        int dimy;
        char mapTxt[30][18];

    public:

        int getDimx() const;
        int getDimy() const;
        char getMap (int x, int y) const;
        bool estLibre(int x, int y) const;

        
        Terrain(int indM  = -1);
        ~Terrain();
        void reinit(int indM);

        void afficheTer() const;
        void modifTer(int i, int j, char val);
        int testRegression();

};

#endif