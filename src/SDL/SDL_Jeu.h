#ifndef _SDLJEU_H
#define _SDLJEU_H

#include <SDL2/SDL.h>
#include <string>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "../Jeu.h"
using namespace std;

/** 
\class Image
@brief
Classe Image qui gère : \n
L'importation d'une image; \n
La création d'une surface et d'une texture; \n
L'affiche du texte; \n
*/
class Image {

private:

    SDL_Surface * surface;
    SDL_Texture * texture;
    bool has_changed;

public:

    Image();
    ~Image();
    void loadFromFile (const char* filename, SDL_Renderer * renderer);
    void loadFromCurrentSurface (SDL_Renderer * renderer);
    void draw (SDL_Renderer * renderer, int x, int y, int w=-1, int h=-1);
    SDL_Texture * getTexture() const;
    void setSurface(SDL_Surface * surf);
};


/** 
\class Texte
@brief
Classe Texte qui gère : \n
La création d'un rectangle pour la zone d'affichage du texte; \n
L'affichage du texte sur la fenêtre SDL.
*/
class Texte {

    private:

        SDL_Surface * surface;
        SDL_Texture * texture;
        SDL_Rect r;
        SDL_Color coul;

    public:

        Texte();
        Texte(SDL_Renderer * renderer, TTF_Font * font, const char* msg);
        ~Texte();
        SDL_Surface* getSurface() const;
        void setTexte (SDL_Renderer * renderer, TTF_Font * f, string msg);
        void setRect (int x, int y, int w, int h);
        void setRect (int perx, int pery, int perw);
        void resizeH (float pery);
        void resizeW (float perw);
        void deplacer (int x, int y);
        void setCoul (unsigned char r, unsigned char v, unsigned char b);
        void afficheTexte (SDL_Renderer * renderer) const;
};


/**
\class sdlJeu
@brief
Classe sdlJeu qui gère : \n
Les différents évenements SDL liés à la fenêtre (clear, quitté, ect..); \n
L'affichage des terrains; \n
La partie SDL des différentes Manches.
*/
class sdlJeu {

private :

	Jeu jeu;

    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Event e;
    TTF_Font * font;
    
    Image mur;
    Image arrivee;
    Image chemin;
    Image perso;
    Image menu;
    Image jouer;
    Image regle;
    Image ragequit;
    Image credits;

    Texte infos;

    bool quit;
    bool rgl;
    bool cred;


public :

    sdlJeu ();
    ~sdlJeu ();

    bool getQuit() const;
    void quitter();
    void sdlBoucle ();
    void sdlClear ();
    void afficheMenu();
    void afficheTerrain();
    void regles();
    void credit();
    void manche1();
    void manche2();
    void manche3();
    void manche4();
    void manche5();
    void Boss();

    bool enCours() const;
    void testManches();
    void attLancer(string msg);
    void sdlEvents();

    void defaiteManche();
};

#endif
