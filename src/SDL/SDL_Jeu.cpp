#include <cassert>
#include <time.h>
#include "SDL_Jeu.h"
#include <stdlib.h>
#include <string>
#include "../Manche1.h"
#include "../Manche.h"
#include "../Manche2.h"
#include "../Manche3.h"
#include "../Manche4.h"
#include "../Manche5.h"
#include "../BOSS.h"
#include "../Jeu.h"
#include "../Terrain.h"
#include <ctime>




#include <iostream>
using namespace std;

const int TAILLE_SPRITE = 50;
const int DIMX = 1000;
const int DIMY = 600;


// ============= CLASS IMAGE =============== //

/** 
@brief
Constructeur de la classe Image, qui initialise une fenêtre vierge
*/
Image::Image () { // initialise une fenêtre vierge
    surface = NULL;
    texture = NULL;
    has_changed = false;
}

/** 
@brief
Destructeur de la classe Image.
*/
Image::~Image() {
    //if (surface != NULL) SDL_FreeSurface(surface);
    if (texture != NULL) SDL_DestroyTexture(texture);
    //surface = NULL;
    texture = NULL;
} 

/** 
\fn void Image::loadFromFile (const char* filename, SDL_Renderer * renderer)
\param Prend en paramètre un fichier de chaîne de caractère, et le renderer de la fenêtre SDL
@brief 
Charge une image sur la fenêtre SDL
*/
void Image::loadFromFile (const char* filename, SDL_Renderer * renderer) { // charge une image sur la fenêtre SDL
    surface = IMG_Load(filename); 
    if (surface == NULL) {  // test si le fichier peut être lu
        string nfn = string("../") + filename;  // construit un nouveau chemin pour trouver le fichier
        cout << "Error: cannot load "<< filename <<". Trying "<<nfn<<endl;
        surface = IMG_Load(nfn.c_str());
        if (surface == NULL) { // si le fichier n'est toujours pas lu 
            nfn = string("../") + nfn; // le chemin est à nouveau reconstruit
            surface = IMG_Load(nfn.c_str());
        }
    }
    if (surface == NULL) { // si le chemin echoue après 2 reconstructions
        cout<<"Error: cannot load "<< filename <<endl; // on renvoie une erreur
        exit(1); // on quite le programme
    }

    SDL_Surface * surfaceCorrectPixelFormat = SDL_ConvertSurfaceFormat(surface,
                                                SDL_PIXELFORMAT_ARGB8888,0);
    SDL_FreeSurface(surface);
    surface = surfaceCorrectPixelFormat;
    loadFromCurrentSurface(renderer);
    SDL_FreeSurface(surfaceCorrectPixelFormat);
}


/** 
\fn void Image::loadFromCurrentSurface (SDL_Renderer * renderer)
\param Prend en paramètre le renderer de la fenêtre SDL
@brief Crée une texture, ou renvoie une erreur si ce n'est pas possible
*/
void Image::loadFromCurrentSurface (SDL_Renderer * renderer) {
    if (texture != NULL) SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(renderer,surface);
    if (texture == NULL) {
        cout << "Error: problem to create the texture from surface " << endl;
        exit(1);
    }
}

/** 
\fn void Image::draw (SDL_Renderer * renderer, int x, int y, int w, int h)
\param Prend en paramètre le renderer de la fenêtre SDL, ainsi que 4 entiers désignants la hauteur,
la largeur, l'ettirement en largeur et l'ettirement en hauteur.
*/
void Image::draw (SDL_Renderer * renderer, int x, int y, int w, int h) {
    int ok;
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = (w<0)?surface->w:w;
    r.h = (h<0)?surface->h:h;

    if (has_changed) {
        ok = SDL_UpdateTexture(texture,NULL,surface->pixels,surface->pitch);
        assert(ok == 0);
        has_changed = false;
    }

    ok = SDL_RenderCopy(renderer,texture,NULL,&r);
    assert(ok == 0);
}

/** 
\fn SDL_Texture * Image::getTexture() const
\return Renvoie la texture courante
*/
SDL_Texture * Image::getTexture() const {return texture;}


/** 
\fn void Image::setSurface(SDL_Surface * surf)
\param Prend une surface en paramètre
@brief Affecte la surface passée en paramètre à la surface de la classe.
*/
void Image::setSurface(SDL_Surface * surf) {
    if (surface != NULL) SDL_FreeSurface(surface);
    surface = surf;
}


// CLASSE TEXTE

/** 
@brief
Constructeur par défaut de la classe Texte
*/
Texte::Texte() {
    surface = NULL;
    texture = NULL;
    r = {0,0,0,0};
    coul = {255,255,255};
}

/** 
@brief
Constructeur de la classe Texte
\param Prend en paramètre le renderer de la fenêtre SDL, une police d'écriture et une chaîne de caractères.
*/
Texte::Texte (SDL_Renderer * renderer, TTF_Font * f, const char * msg) {
    Texte();
    setTexte(renderer, f, msg);
}

/** 
@brief
Destructeur de la classe Texte.
*/
Texte::~Texte() {
    if (surface != NULL) SDL_FreeSurface(surface);
    if (texture != NULL) SDL_DestroyTexture(texture);
    surface = NULL;
    texture = NULL;
}

/** 
\fn SDL_Surface* Texte::getSurface() const
\return Retourne le pointeur vers la surface representant le texte
*/
SDL_Surface* Texte::getSurface() const {
    return surface;
}

/** 
\fn void Texte::setTexte(SDL_Renderer * renderer, TTF_Font* f, string msg)
\param Prend en paramètre le renderer de la fenêtre SDL, une police d'écriture et une chaîne de caractères.
@brief
Crée la surface et la texture correspondants au texte passé en paramètres. \n
Le rectangle d'affichage est initialisé au centre pour occuper toute la longueur.
*/
void Texte::setTexte(SDL_Renderer * renderer, TTF_Font* f, string msg) {
    if (surface != NULL) SDL_FreeSurface(surface);
    if (texture != NULL) SDL_DestroyTexture(texture);

    surface = NULL;
    texture = NULL;
    
    surface = TTF_RenderText_Blended(f,msg.c_str(),coul);
    //assert(surface != NULL);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    //assert(texture != NULL);

    r.w = DIMX - 60;
    r.x = 30;
    r.h = (float)DIMX/(float)msg.length();
    r.y = DIMY/2.0 - r.h/2.0;
}

/** 
\fn void Texte::setRect(int x, int y, int w, int h)
\param Prend en paramètres 4 entiers correspondants à la largeur, la hauteur, l'ettirement en largeur et 
l'ettirement en hauteur du texte courrant.
@brief
Crée un rectangle aux dimensions passées en paramètres.
*/
void Texte::setRect(int x, int y, int w, int h) {
    assert (x>=0 && y>=0 && w>=0 && h>=0);
    r = {x,y,w,h};

}

/** 
\fn void Texte::setRect(int perx, int pery, int perw)
\param Prend en paramètres 3 entiers correspondants aux pourcentages par rapport à la taille de la fenêtre
pour la position et l'ettirement en largeur.
@brief
Crée un rectangle en pourcentages de la fenêtre selon la taille du texte courrant.
*/
void Texte::setRect(int perx, int pery, int perw) {
    assert (surface != NULL);
    assert (perx <= 100 && pery <=100 && perw <= 100);
    assert (perx >= 0 && pery >= 0 && perw >= 0);
    float scl = (float)DIMX*(float(perw)/100.0)/float(surface->w);

    r.w = surface->w * scl;
    r.h = surface->h * scl;

    r.x = perx/100.0 * DIMX - r.w/2;
    r.y = pery/100.0 * DIMY - r.h/2;
    if (r.x < 0) r.x = 0;
    if (r.x + r.w > DIMX) r.x = DIMX - r.w;
    if (r.y < 0) r.y = 0;
    if (r.y + r.h > DIMY) r.y = DIMY - r.h;    
}

/** 
\fn void Texte::resizeH (float pery)
\param Prend en paramètre le nouveau pourcentage de la fenetre à occuper
@brief
Crée un rectangle en pourcentages de la fenêtre selon la taille du texte courrant.
*/
void Texte::resizeH (float pery) {
    float scl;
    scl = DIMY*(float)pery/(float)r.h;
    int tw = r.w;
    int th = r.h;
    r.h = pery*DIMY;
    r.y = r.y+(r.h-th)/2.0;
    r.w *= scl;
    r.x = r.x+(r.w-tw)/2.0;
    if (r.h > DIMY) {
        r.h = DIMY-50;
        r.y = 25;
    }
    if (r.w > DIMX) {
        r.w = DIMX-60;
        r.x = 30;
    }
}

/** 
\fn void Texte::resizeW (float perw)
\param Prend en paramètre le nouveau pourcentage de la fenetre à occuper
@brief
Crée un rectangle en pourcentages de la fenêtre selon la taille du texte courrant.
*/
void Texte::resizeW (float perw) {
    float scl;
    scl = DIMX*(float)perw/(float)r.w;
    int tw = r.w;
    int th = r.h;
    r.w = perw;
    r.x = r.x+(tw-r.w)/2.0;
    r.h *= scl;
    r.y = r.y+(th-r.h)/2.0;
    if (r.h > DIMY) {
        r.h = DIMY;
        r.y = 0;
    }
    if (r.w > DIMX) {
        r.w = DIMX;
        r.x = 0;
    }
}     

/** 
\fn void Texte::deplacer (int x, int y)
\param Prend en paramètre la nouvelle position
@brief
Deplace le rectangle d'affichage à la position indiquée.
*/
void Texte::deplacer (int x, int y) {
    if (x>=DIMX) x = DIMX-1;
    if (y>=DIMY) y = DIMY-1;
    if (x>=0) r.x = x;
    if (y>=0) r.y = y;
}



void Texte::setCoul (unsigned char r, unsigned char v, unsigned char b) {
    coul = {r,v,b};
}

/** 
\fn void Texte::afficheTexte(SDL_Renderer * renderer) const
\param Prend en paramètre le renderer de la fenêtre SDL.
@brief Affiche le texte sur la fenêtre SDL.
*/
void Texte::afficheTexte(SDL_Renderer * renderer) const {
    SDL_RenderCopy(renderer, texture, NULL, &r);
}


// SDL_JEU

/** 
@brief Constructeur de la classe sdlJeu, qui : \n
Crée et ouvre la fenêtre SDL; \n
Initialise le booléen quit à faux; \n
Initialise une police d'écriture; \n
Crée un renderer; \n
Initialise les images courraments utilisées; \n
Débute la partie.
*/
sdlJeu::sdlJeu () {
    // Initialisation de la SDL
    rgl = true;
    cred = true;
    quit = false;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "Erreur lors de l'initialisation de la SDL : " ;
        cout << SDL_GetError() << endl;SDL_Quit();exit(1);
    }

    

    // Creation de la fenetre
    window = SDL_CreateWindow("gauntlet", SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED, DIMX, DIMY,
                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError();
        cout << endl; SDL_Quit(); exit(1);
    }

    if (TTF_Init() != 0) {
        cout << "Erreur lors de l'initialisation de la SDL_ttf : " ;
        cout << TTF_GetError() << endl;SDL_Quit();exit(1);
    }
    font = TTF_OpenFont("data/arial.ttf",550);
    if (font == NULL) {
            cout << "Failed to load font arial! SDL_TTF Error: " ;
            cout << TTF_GetError() << endl; SDL_Quit(); exit(1);
	}

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    mur.loadFromFile("data/mur.png", renderer);
    arrivee.loadFromFile("data/arrivee.png",renderer);
    chemin.loadFromFile("data/chemin.png", renderer);
    perso.loadFromFile("data/perso.png", renderer);
    jouer.loadFromFile("data/bouton_jouer.PNG", renderer);
    regle.loadFromFile("data/bouton_regles.png", renderer);
    ragequit.loadFromFile("data/bouton_quitter.png", renderer);
    credits.loadFromFile("data/bouton_credits.png", renderer);

    infos.setTexte(renderer, font, "Vies : 3");
    infos.setRect(100,0,15);
    jeu.debutPartie();
}

/** 
@brief
Destructeur de la classe sdlJeu
*/
sdlJeu::~sdlJeu () {
    if (!quit) {
        jeu.finPartie();
        if (jeu.perso.getVies() == 0) 
            attLancer("Vous avez perdu :( \n Appuyez sur entree pour quitter");
        else 
            attLancer ("Vous avez gagne ! Bravo !\n Appuyez sur entree pour quitter");
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/** 
\fn bool sdlJeu::getQuit() const
\return Renvoie la valeur du booléen quit.
*/
bool sdlJeu::getQuit() const {
    return quit;
}

/** 
\fn void sdlJeu::quitter()
@brief 
Passe le booléen quit à vrai pour permettre la fermeture de la fenêtre.
*/
void sdlJeu::quitter() {
    quit = true;
    cout<<"Vous avez quitté la partie"<<endl;
}


/** 
\fn void sdlJeu::attLancer(string msg)
\param Prend en paramètre une chaîne de caractères.
@brief
Permet l'attente d'un appuie sur la touche Entrée pour lancer une manche.
*/
void sdlJeu::attLancer(string msg) {
    vector<string> lignes;
    int nbLignes = 0;
    int debut = 0;
    for (unsigned int i=0;i<msg.length();i++) {
        if (msg[i] == '\n') {
            lignes.push_back(msg.substr(debut,i-debut));
            debut = i+1;
            nbLignes++;
        }
    }
    lignes.push_back(msg.substr(debut,msg.length()-debut));
    nbLignes++;
    Texte consignes[nbLignes];
    int h = (float)DIMY/nbLignes;
    int ecart = h*0.1;
    h-= ecart;
    int y = ecart;
    float pery = (float)h/DIMY;
    for (int i=0;i<nbLignes;i++) {
        consignes[i].setTexte(renderer,font,lignes[i].c_str());
        consignes[i].resizeH(pery);
        consignes[i].deplacer(-1,y);
        y+=h+ecart;
    }
    bool att = true;
    while (att) {
        while (SDL_PollEvent(&e)) {
        switch(e.type){
            case SDL_QUIT:
                quitter();
                att = false;
                break;

            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_RETURN)
                    att = false;
            }
            
        }
        sdlClear();
        for (int i=0; i<nbLignes;i++) {
            consignes[i].afficheTexte(renderer);
        }
        infos.afficheTexte(renderer);
        SDL_RenderPresent(renderer);
    }
}

/** 
\fn void sdlJeu::sdlClear()
@brief
Remplie l'écran de noir
*/
void sdlJeu::sdlClear() {
	//Remplir l'cran de noir
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

/** 
\fn bool sdlJeu::enCours() const
\return Renvoie le booléen vérifiant si la partie est toujours en cours.
*/
bool sdlJeu::enCours() const {
    return (jeu.perso.getVies() > 0 && !getQuit() &&
            jeu.perso.pos.getValPos(jeu.ter) != ARRIVEE);
}

/** 
\fn void sdlJeu::testManches()
@brief Lanche des différentes manches en fonction du caractère présent dans le tableau du terrain courrant. 
*/
void sdlJeu::testManches() {
    
    switch (jeu.perso.pos.getValPos(jeu.ter)) {
        case '1': if (!jeu.getManche(0)) manche1(); break;
        case '2': if (!jeu.getManche(1)) manche2(); break;
        case '3': if (!jeu.getManche(2)) manche3(); break;
        case '4': if (!jeu.getManche(3)) manche4(); break;
        case '5': if (!jeu.getManche(4)) manche5(); break;
        case '6': if (!jeu.getManche(5)) Boss(); break;
    }
}

/** 
\fn void sdlJeu::sdlEvents()
@brief
Gère les évenements basiques sur la fenêtre (se déplacer, quitter la fenêtre).
*/
void sdlJeu::sdlEvents() {
    while (SDL_PollEvent(&e)) {
        switch(e.type){
            case SDL_QUIT:
                quitter();
                break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_UP: jeu.perso.pos.haut(jeu.ter); break;
                    case SDLK_DOWN: jeu.perso.pos.bas(jeu.ter); break;
                    case SDLK_LEFT: jeu.perso.pos.gauche(jeu.ter); break;
                    case SDLK_RIGHT: jeu.perso.pos.droite(jeu.ter); break;
                }
        }
    }
}

/** 
\fn void sdlJeu::defaiteManche()
@brief
Affiche un message en cas de défaite durant une manche et met à jour la chaîne de caractères comportant
le nombre de vies.
*/
void sdlJeu::defaiteManche(){
    infos.setTexte(renderer,font,
    ("Vies : " + to_string(jeu.perso.getVies())).c_str());
    if (jeu.perso.getVies() > 0) 
        attLancer("Manche perdue, appuyer sur entree pour relancer ! \n\n\n\n\n\n");
    infos.setRect(100,0,15);
}

/** 
\fn void sdlJeu::afficheMenu()
@brief
Affiche sur la fenêtre SDL le menu du jeu.
*/
void sdlJeu::afficheMenu (){

    Texte titre;
    Texte msg;
    

    while(!quit && enCours()){

        sdlClear();
        jouer.draw(renderer,450,250,150,65);
        regle.draw(renderer,450,350,150,65);
        ragequit.draw(renderer,800,500,150,65);
        credits.draw(renderer,50,500,150,65);
        titre.setTexte(renderer,font,"GAUNTLET");
        titre.setRect(50,20,80);
        titre.afficheTexte(renderer);
        SDL_PollEvent(&e); ///Attendre que l'on appuie sur une touche

        switch(e.type){
            case SDL_QUIT:
                quitter();
                break;
            case SDL_MOUSEBUTTONUP: ///Si un clic souris à été détecté
                if(e.button.x >= 450 && e.button.y >= 250 && e.button.x < 601 &&
                   e.button.y < 301 && rgl){
                    sdlBoucle();
                }
                if(e.button.x >= 450 && e.button.y >= 350 && e.button.x < 601 &&
                   e.button.y < 416){
                    rgl = false;            
                }
                if(e.button.x >= 800 && e.button.y >= 500 && e.button.x < 951 &&
                   e.button.y < 566){

                    quitter();
                }
                if(e.button.x >= 50 && e.button.y >= 500 && e.button.x < 201 &&
                   e.button.y < 566){

                    cred = false;
                }

            case SDL_KEYDOWN:///Si une touche a été appuyé
                if(e.key.keysym.sym == SDLK_RETURN) {
                    rgl = true;
                    cred = true;
                    break;
                }
 
        }
        if(rgl == false) regles();
        else if(cred == false) credit();
        SDL_RenderPresent(renderer); 
    } 
   
}

/** 
\fn void sdlJeu::regles()
@brief Affiche sur la fenêtre SDL les règles du jeu nécessaire pour pouvoir jouer.
*/
void sdlJeu::regles(){
    Texte regle;

    sdlClear();
    regle.setTexte(renderer,font, "Regles du jeu :");
    regle.setRect(40,20,230,50);
    regle.afficheTexte(renderer); 
    regle.setTexte(renderer,font, "Le but du jeu est de traverser un labyrinthe pour gagner.");
    regle.setRect(40,90,900,40);
    regle.afficheTexte(renderer);  
    regle.setTexte(renderer,font, "Cependant votre chemin sera seme d'embuche.");
    regle.setRect(40,140,750,40);
    regle.afficheTexte(renderer); 
    regle.setTexte(renderer,font, "Vous aurez donc 5 manches plus ou moins difficile a realiser,");
    regle.setRect(40,190,920,40);
    regle.afficheTexte(renderer);     
    regle.setTexte(renderer,font, "Et avant l'arrivee une epreuve finale terrifiante vous attend !!!");
    regle.setRect(40,240,950,40);
    regle.afficheTexte(renderer);
    regle.setTexte(renderer,font, "Information technique :");
    regle.setRect(40,340,330,40);
    regle.afficheTexte(renderer);    
    regle.setTexte(renderer,font, "Pour realiser toutes les manche vous aurez besoin uniquement ");
    regle.setRect(40,390,920,40);
    regle.afficheTexte(renderer);
    regle.setTexte(renderer,font, "de votre clavier.");
    regle.setRect(40,440,230,40);
    regle.afficheTexte(renderer);    
    regle.setTexte(renderer,font, "Maintenant que vous savez tous, C'EST PARTI !!!!!!!!");
    regle.setRect(40,530,850,40);
    regle.afficheTexte(renderer);     
}

/** 
\fn void sdlJeu::credit()
@brief Affiche sur la fenêtre SDL les personnes ayant réalisé ce projet.
*/
void sdlJeu::credit(){
    Texte credit;

    sdlClear();
    credit.setTexte(renderer,font, "Projet LIFAP4 de :");
    credit.setRect(350,100,300,50);
    credit.afficheTexte(renderer);     
    credit.setTexte(renderer,font, "Aurelien MARTINET");
    credit.setRect(350,170,300,50);
    credit.afficheTexte(renderer);     
    credit.setTexte(renderer,font, "Romaric GUTH");
    credit.setRect(375,220,250,50);
    credit.afficheTexte(renderer);    
    credit.setTexte(renderer,font, "Thomas RIMET");
    credit.setRect(375,270,250,50);
    credit.afficheTexte(renderer); 
    credit.setTexte(renderer,font, "Merci d'avoir jouer =)");
    credit.setRect(325,400,350,50);
    credit.afficheTexte(renderer);    
}

/** 
\fn void sdlJeu::afficheTerrain()
@brief Affiche sur la fenêtre SDL le terrain courrant.
*/
void sdlJeu::afficheTerrain() {
    int x = 0;
    int y = 0;
    int w = TAILLE_SPRITE;
    int h = TAILLE_SPRITE;
    
    for (int j=0;j<jeu.ter.getDimy();++j) {
        for (int i=0;i<jeu.ter.getDimx();++i) {
            if (jeu.ter.getMap(i,j) == MUR) {
                mur.draw(renderer,x,y,w,h);
            }
            else {
                chemin.draw(renderer,x,y,w,h);
            }
            x=x+w;
        }
        x = 0;
        y=y+h;
    }
    perso.draw(renderer,jeu.perso.pos.getX()*w,jeu.perso.pos.getY()*h,w,h);
    arrivee.draw(renderer,DIMX-w,DIMY-h,w,h);
}

/** 
\fn void sdlJeu::sdlBoucle()
@brief Déroulement courrant de la partie.
*/
void sdlJeu::sdlBoucle() {
    while (enCours()) {
        jeu.setTimer(SDL_GetTicks());
        sdlClear();
        testManches();
        sdlEvents();
        afficheTerrain(); 
        infos.afficheTexte(renderer);
        SDL_RenderPresent(renderer);   
    }
}

/** 
\fn void sdlJeu::manche1()
@brief
gère les évenements SDL liés à la Manche 1.
*/
void sdlJeu::manche1() {
    // PARAMS
    Manche1 m; 
    m.Lancement();

    bool aff;
    Texte ope;
    Texte result;
    Texte msg ;
    
    msg.setTexte(renderer,font, 
            "Calculez l'operation suivante. Attention a memoriser vos resultats");
    msg.setRect(30,50,DIMX-60,DIMY/15);

    attLancer("Lancement de la manche 1\nCalcul mental !\nAppuyez sur entree");
    // TANT QU'ON PEUT LANCER LA MANCHE
    while (!m.getReussie() && jeu.perso.getVies()>0 && !quit) {
        // INIT
        m.init();
        jeu.setTimer(SDL_GetTicks());
        aff = true;
        // TANT QUE LA MANCHE EST EN COURS
        while (m.getEnCours() && !quit) {

            // MAJ DES TEXTES
            
            if (aff) {
                ope.setTexte(renderer, font, m.getOperation().c_str());
                ope.setRect(50,40,50);
                result.setTexte(renderer, font, to_string(m.getRes()).c_str());
                result.setRect(50, 80, 10);
                aff = false;
            }
            
            // TESTS CHIFFRES ENTRES
            
            while (SDL_PollEvent(&e)) {
                switch(e.type){
                    case SDL_QUIT:
                        quitter();
                        break;
                    case SDL_KEYDOWN:
                        aff = true;
                        if(e.key.keysym.sym >= SDLK_KP_1 &&
                            e.key.keysym.sym <= SDLK_KP_0) {
                        
                            if (e.key.keysym.sym == SDLK_KP_0) {
                                m.aSaisi(0);
                                break;
                            }
                            m.aSaisi(e.key.keysym.sym - SDLK_KP_1 + 1);
                            break;
                        }     
                        else if(e.key.keysym.sym >= SDLK_0 &&
                                e.key.keysym.sym <= SDLK_9) {
                            m.aSaisi(e.key.keysym.sym - SDLK_0);
                            break;
                        }
                        else if(e.key.keysym.sym == SDLK_RETURN) {
                            m.verifRes();
                            break;
                        }
                        else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                            m.modifRes();
                            break;
                        }
                        else {
                            cout << "rentrez un nombre avec le pavé numériqe"<<endl;
                        }
                        
                        

                }
            }

            // ACTIONS AUTOMATIQUES
            
            sdlClear();
            msg.afficheTexte(renderer);
            ope.afficheTexte(renderer);
            result.afficheTexte(renderer);
            infos.afficheTexte(renderer);
            SDL_RenderPresent(renderer); 

            m.setTimer(SDL_GetTicks());
            m.testReussie(jeu);        
        }    
        if (!m.getReussie() && !quit)    defaiteManche();
    }  
}

/** 
\fn void sdlJeu::manche1()
@brief
gère les évenements SDL liés à la Manche 2.
*/
void sdlJeu::manche2(){
    Manche2 m;
    m.Lancement();
    bool aff;
    Texte lettre;
    Texte nbJuste;
    Texte indicNbJuste;
    Texte indicLettre;

    indicNbJuste.setTexte(renderer, font, "Nombre d'essais reussis :");
    indicLettre.setTexte(renderer, font, "Vous devez appuyer sur");
    indicNbJuste.setRect(50, 20, 70);
    indicLettre.setRect(50,60,60);
    
    attLancer("Lancement de la manche 2\nAppuyez sur la lettre demande\nappuyez sur entree");


    // TANT QU ON PEUT LANCER LA MANCHE
    while (!m.getReussie() && jeu.perso.getVies()>0 && !quit) {
        
        // PREPARATION DE LA MANCHE

        m.init();
        jeu.setTimer(SDL_GetTicks()); 
        aff = true;

        // TANT QUE LA MANCHE EST EN COURS
        while (m.getEnCours() && !quit) {
 
            // MAJ DES TEXTES

            if (aff) {
                cout<<m.getTouche()<<endl;
                lettre.setTexte(renderer, font, m.getTouche().c_str());
                nbJuste.setTexte(renderer, font, to_string(m.getNbJuste()).c_str());
                lettre.setRect(50,80,5);
   	            nbJuste.setRect(50,40,10);
                aff = false;
            }
               
            //TEST DES EVENTS

            while (SDL_PollEvent(&e)) {
                switch(e.type){
                    case SDL_QUIT:
                    quitter();
                    break;

                    case SDL_KEYDOWN:
                        m.verifTouche(int(e.key.keysym.sym));          
                        aff = true;
                }
            }

            //AFFICHAGE ET ACTIONS AUTO

            sdlClear();
            lettre.afficheTexte(renderer);
            nbJuste.afficheTexte(renderer);
            indicNbJuste.afficheTexte(renderer);
            indicLettre.afficheTexte(renderer);
            infos.afficheTexte(renderer);
            SDL_RenderPresent(renderer);

            m.setTimer(SDL_GetTicks());
            m.testReussie(jeu);         
        }
        if (!m.getReussie() && !quit)  defaiteManche();
    }  
}

/** 
\fn void sdlJeu::manche3()
@brief
gère les évenements SDL liés à la Manche 3.
*/
void sdlJeu::manche3(){
    Manche3 m(jeu.perso.pos);
    Image objec;
    m.Lancement();

    attLancer("Lancement de la manche 3\nmemorisez la map\nAppuyez sur entree");
    objec.loadFromFile("data/objec.png", renderer);
    
    
    while (!m.getReussie() && jeu.perso.getVies() > 0 && !quit) {
        
        m.init(jeu);  

        afficheTerrain();
        objec.draw(renderer, m.objectif.getX()*TAILLE_SPRITE,
                 m.objectif.getY()*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);
        SDL_RenderPresent(renderer);
        SDL_Delay(5000);
        jeu.setTimer(SDL_GetTicks());

        while (m.getEnCours()  && !quit) {

            m.setTimer(SDL_GetTicks());
            sdlClear();
            objec.draw(renderer, m.objectif.getX()*TAILLE_SPRITE,
                m.objectif.getY()*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE); 
            sdlEvents();
            perso.draw(renderer,jeu.perso.pos.getX()*TAILLE_SPRITE,
                 jeu.perso.pos.getY()*TAILLE_SPRITE,TAILLE_SPRITE,TAILLE_SPRITE);
            SDL_RenderPresent(renderer);

            m.testReussie(jeu);
            

            if (!m.getEnCours() && !m.getReussie())  defaiteManche();

        }
        jeu.perso.pos.setX(m.postmp.getX());
        jeu.perso.pos.setY(m.postmp.getY());
    }
}

/** 
\fn void sdlJeu::manche4()
@brief
gère les évenements SDL liés à la Manche 4.
*/
void sdlJeu::manche4(){

    Manche4 m(jeu.perso.pos);
    Image porte;
    m.Lancement();

    attLancer("Lancement de la manche 4\nrejoignez la sortie !\nAppuyez sur entree");
    porte.loadFromFile("data/porte.png", renderer);
    while (!m.getReussie() && jeu.perso.getVies() > 0 && !quit) {
        m.init(jeu.perso); 
        jeu.setTimer(SDL_GetTicks());
        while(m.getEnCours() && !quit) {
            
            int x = 0;
            int y = 0;
            int w = ceil((float)DIMX / m.ter.getDimx());
            int h = ceil((float)DIMY / m.ter.getDimy());

            for(int j = 0; j < m.ter.getDimy(); j++){
                char map;
                for(int i = 0; i < m.ter.getDimx(); i++ ){
                    map = m.ter.getMap(i,j);
                    switch(map) {
                        case MUR: mur.draw(renderer,x,y,w,h); break;
                        case CHEMIN: chemin.draw(renderer,x,y,w,h); break;
                        default: porte.draw(renderer,x,y,w,h); 
                    }
                    x += w;
                }
                x = 0;
                y += h;
            }

            perso.draw(renderer,jeu.perso.pos.getX()*w,
                        jeu.perso.pos.getY()*h,w,h);
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&e)) {
                switch(e.type){
                    case SDL_QUIT:
                    quitter();
                    break;

                    case SDL_KEYDOWN:
                        switch(e.key.keysym.sym) {
                            case SDLK_RIGHT:
                                jeu.perso.pos.droite(m.ter);
                                break;
                            case SDLK_LEFT:
                                jeu.perso.pos.gauche(m.ter);
                                break;
                            case SDLK_UP:
                                m.prendrePorte(jeu.perso);
                                break;
                        }               
                }
            }
            m.setTimer(SDL_GetTicks());
            m.testReussie(jeu);
        }
        if (m.getReussie()){
             jeu.perso.pos.setX(m.postmp.getX());
             jeu.perso.pos.setY(m.postmp.getY());
        }
        if (!m.getEnCours() && !m.getReussie())  defaiteManche();            
    }
}

/** 
\fn void sdlJeu::manche5()
@brief
gère les évenements SDL liés à la Manche 5.
*/
void sdlJeu::manche5(){

    Manche5 m(jeu.perso.pos);
    Image canonH;
    Image canonB;
    Image canonG;
    Image canonD;
    Image balleH;
    Image balleB;
    Image balleG;
    Image balleD;
    m.Lancement();

    canonH.loadFromFile("data/canonH.png", renderer);
    canonB.loadFromFile("data/canonB.png", renderer);
    canonG.loadFromFile("data/canonG.png", renderer);
    canonD.loadFromFile("data/canonD.png", renderer);

    balleH.loadFromFile("data/balleH.png", renderer);
    balleB.loadFromFile("data/balleB.png", renderer);
    balleG.loadFromFile("data/balleG.png", renderer);
    balleD.loadFromFile("data/balleD.png", renderer);

    attLancer("Lancement de la manche 5\nevitez les balles !\nAppuyez sur entree");

    while (!m.getReussie() && jeu.perso.getVies() > 0 && !quit) {
        m.init(jeu.perso); 
        jeu.setTimer(SDL_GetTicks());
        while(m.getEnCours() && !quit) {
            
            m.setTimer(SDL_GetTicks());

            while (SDL_PollEvent(&e)) {
                switch(e.type){
                    case SDL_QUIT:
                        quitter();
                        break;

                    case SDL_KEYDOWN:
                        switch(e.key.keysym.sym) {
                            case SDLK_UP: jeu.perso.pos.bas(m.ter); break;
                            case SDLK_DOWN: jeu.perso.pos.haut(m.ter); break;
                            case SDLK_LEFT: jeu.perso.pos.droite(m.ter); break;
                            case SDLK_RIGHT: jeu.perso.pos.gauche(m.ter); break;
                        }               
                }
            }

            m.update();

            // AFFICHAGE

            sdlClear();

            int x = 0;
            int y = 0;
            int w = ceil((float)DIMX / m.ter.getDimx());
            int h = ceil((float)DIMY / m.ter.getDimy());

            for(int j = 0; j < m.ter.getDimy(); j++) {
                for(int i = 0; i < m.ter.getDimx(); i++ ) {
                    if (m.ter.getMap(i,j)==CHEMIN) chemin.draw(renderer,x,y,w,h);
                    else {
                        mur.draw(renderer,x,y,w,h);
                        switch(m.ter.getMap(i,j)) {
                            case 'h': canonH.draw(renderer,x,y,w,h); break;
                            case 'b': canonB.draw(renderer,x,y,w,h); break;
                            case 'g': canonG.draw(renderer,x,y,w,h); break;
                            case 'd': canonD.draw(renderer,x,y,w,h); break; 
                            default: break;
                        }
                    }
                    x += w;
                }
                x = 0;
                y += h;
            }
            for (unsigned int i = 0;i < m.tabBalles.size();++i) {
                Position2D pos = m.tabBalles[i].posBalle;
                switch (m.tabBalles[i].sens) {
                    case 0: // sens bas
                        balleB.draw(renderer, pos.getX()*w,
                        (pos.getY()+(m.getTimer()-m.tabBalles[i].timerB)/
                        (float) m.tabBalles[i].vitessePos)*h,w,h);
                        break;
                    case 1: // sens haut
                        balleH.draw(renderer, pos.getX()*w, 
                        (pos.getY()-(m.getTimer()-m.tabBalles[i].timerB)/
                        (float)m.tabBalles[i].vitessePos)*h,w,h);
                        break;
                    case 2: // sens droite
                        balleD.draw(renderer,
                        (pos.getX()+(m.getTimer()-m.tabBalles[i].timerB)/
                        (float)m.tabBalles[i].vitessePos)*w, pos.getY()*h,w,h);
                        break;
                    case 3: // sens gauche
                        balleG.draw(renderer,
                        (pos.getX()-(m.getTimer()-m.tabBalles[i].timerB)/
                        (float)m.tabBalles[i].vitessePos)*w, pos.getY()*h,w,h);
                        break;
                    default: break;
                }
            }

            perso.draw(renderer,jeu.perso.pos.getX()*w,jeu.perso.pos.getY()*h,w,h);
            SDL_RenderPresent(renderer);
            
            m.testReussie(jeu);
        }
        if (m.getReussie()){
             jeu.perso.pos.setX(m.postmp.getX());
             jeu.perso.pos.setY(m.postmp.getY());
        }
        if (!m.getEnCours() && !m.getReussie())  defaiteManche();            
    }
}

/** 
\fn void sdlJeu::Boss()
@brief
gère les évenements SDL liés au boss.
*/
void sdlJeu::Boss() {


    BOSS m(jeu.perso);

    Image balleH;
    Image balleB;
    Image balleG;
    Image balleD;
    Image telep;
    Image boss;
    Image fleur;
    Image feu;
    m.Lancement();

    Texte vieP;
    Texte vieB;
    

    balleH.loadFromFile("data/balleH.png", renderer);
    balleB.loadFromFile("data/balleB.png", renderer);
    balleG.loadFromFile("data/balleG.png", renderer);
    balleD.loadFromFile("data/balleD.png", renderer);
    telep.loadFromFile("data/teleporteur.png", renderer);
    boss.loadFromFile("data/boss.png", renderer);
    fleur.loadFromFile("data/fleur.png", renderer);
    feu.loadFromFile("data/feu.png", renderer);

    attLancer("BOSS FINAL!\nA pour un coup d'epee\nZ pour construire un mur\nE pour tirer lorsque vous avez une fleur");

    while (!m.getReussie() && jeu.perso.getVies() > 0 && !quit) {
        m.setTimer(SDL_GetTicks());
        m.init(jeu.perso); 
        jeu.setTimer(SDL_GetTicks());
        while(m.getEnCours() && !quit) {
            
            m.setTimer(SDL_GetTicks());
            while (SDL_PollEvent(&e)) {
                switch(e.type){
                        case SDL_QUIT:
                            quitter();
                            break;

                        case SDL_KEYDOWN:
                            switch(e.key.keysym.sym) {
                                case SDLK_UP:   jeu.perso.pos.haut(m.ter); 
                                                m.setSens('h');
                                                break;
                                case SDLK_DOWN: jeu.perso.pos.bas(m.ter);
                                                m.setSens('b');
                                                break;
                                case SDLK_LEFT: jeu.perso.pos.gauche(m.ter);
                                                m.setSens('g');
                                                break;
                                case SDLK_RIGHT:jeu.perso.pos.droite(m.ter);
                                                m.setSens('d');
                                                break;
                                case SDLK_z: switch(m.getSens()) {
                                    
                                    case 'h': m.construire(jeu.perso.pos,0);
                                              break;     
                                    case 'b': m.construire(jeu.perso.pos,1);
                                              break;
                                    case 'g': m.construire(jeu.perso.pos,2);
                                              break; 
                                    case 'd': m.construire(jeu.perso.pos,3);
                                              break;                                 
                                }
                                break;

                                case SDLK_a: switch(m.getSens()) {
                                    
                                    case 'h': m.taper(jeu.perso.pos,0);
                                              break;     
                                    case 'b': m.taper(jeu.perso.pos,1);
                                              break;
                                    case 'g': m.taper(jeu.perso.pos,2);
                                              break; 
                                    case 'd': m.taper(jeu.perso.pos,3);
                                              break; 
                                                                     
                                }
                                break;

                                case SDLK_e:if(m.getFeu()) switch(m.getSens()){
                                    
                                    case 'b': m.tirerFeu(jeu.perso.pos,0);
                                              break;     
                                    case 'h': m.tirerFeu(jeu.perso.pos,1);
                                              break;
                                    case 'd': m.tirerFeu(jeu.perso.pos,2);
                                              break; 
                                    case 'g': m.tirerFeu(jeu.perso.pos,3);
                                              break;                                  
                                }
                                break;
                            }    
                }
            }
            m.update(jeu.perso);

            // AFFICHAGE

            sdlClear();

            int x = 0;
            int y = 0;
            int w = ceil((float)DIMX / m.ter.getDimx());
            int h = ceil((float)DIMY / m.ter.getDimy());
           


            for(int j = 0; j < m.ter.getDimy(); j++) {
                for(int i = 0; i < m.ter.getDimx(); i++ ) {
                    if (m.ter.getMap(i,j)==CHEMIN) 
                        chemin.draw(renderer,x,y,w,h);
                    else if(m.ter.getMap(i,j)==MUR) 
                        mur.draw(renderer,x,y,w,h); 
                    else if(isdigit(m.ter.getMap(i,j))) {
                        chemin.draw(renderer,x,y,w,h);
                        telep.draw(renderer,x,y,w,h);
                    }
                    else if(m.ter.getMap(i,j)==FLEUR) {
                        chemin.draw(renderer,x,y,w,h);
                        fleur.draw(renderer,x,y,w,h);
                    }
                    x += w;
                }
                x = 0;
                y += h;
            }
            if(!m.blinkPerso())
                perso.draw(renderer,jeu.perso.pos.getX()*w,
                            jeu.perso.pos.getY()*h,w,h);
            if (m.onde.saut) {
                if (m.getTimer()-m.onde.debut < 2000) {
                    boss.draw(renderer,m.boss.getX()*w,
                    (m.boss.getY()-2)*h*(1-(m.getTimer()-m.onde.debut)/2000.0),
                    2*w,2*h);
                }
                if (m.getTimer()-m.onde.debut > 3000) {
                    boss.draw(renderer,m.boss.getX()*w,
                    (m.boss.getY()-2)*h*(m.getTimer()-m.onde.debut-3000)/2000.0,
                    2*w,2*h);
                }
            }
            else if (!m.blinkBoss())
                boss.draw(renderer,m.boss.getX()*w,m.boss.getY()*h,2*w,2*h);
            if(m.onde.onde) {
                for (std::list<Position2D>::iterator it=m.onde.ondeH.begin();
                    it!=m.onde.ondeH.end(); ++it) {
                    boss.draw(renderer, it->getX()*w, it->getY()*h, w, h);
                }
                for (std::list<Position2D>::iterator it=m.onde.ondeB.begin();
                     it!=m.onde.ondeB.end(); ++it) {
                    boss.draw(renderer, it->getX()*w, it->getY()*h, w, h);
                }
                for (std::list<Position2D>::iterator it=m.onde.ondeG.begin();
                     it!=m.onde.ondeG.end(); ++it) {
                    boss.draw(renderer, it->getX()*w, it->getY()*h, w, h);
                }
                for (std::list<Position2D>::iterator it=m.onde.ondeD.begin();
                     it!=m.onde.ondeD.end(); ++it) {
                    boss.draw(renderer, it->getX()*w, it->getY()*h, w, h);
                }
            }
            for (unsigned int i = 0;i < m.tabRoquettes.size();++i) {
                Position2D pos = m.tabRoquettes[i].posBalle;
                switch (m.tabRoquettes[i].sens) {
                    case 0: // sens bas
                        balleB.draw(renderer, pos.getX()*w, 
                        (pos.getY()+(m.getTimer()-m.tabRoquettes[i].timerB)/
                        (float)m.tabRoquettes[i].vitessePos)*h,w,h);
                        break;
                    case 1: // sens haut
                        balleH.draw(renderer, pos.getX()*w, 
                        (pos.getY()-(m.getTimer()-m.tabRoquettes[i].timerB)/
                        (float)m.tabRoquettes[i].vitessePos)*h,w,h);
                        break;
                    case 2: // sens droite
                        balleD.draw(renderer,
                        (pos.getX()+(m.getTimer()-m.tabRoquettes[i].timerB)/
                        (float)m.tabRoquettes[i].vitessePos)*w, pos.getY()*h,w,h);
                        break;
                    case 3: // sens gauche
                        balleG.draw(renderer,
                        (pos.getX()-(m.getTimer()-m.tabRoquettes[i].timerB)/
                        (float)m.tabRoquettes[i].vitessePos)*w, pos.getY()*h,w,h);
                        break;
                    default: break;
                }
            }

            for(unsigned int i = 0; i < m.bouleF.size(); i++){
                Position2D pos = m.bouleF[i].posBalle;
                switch (m.bouleF[i].sens) {
                    case 0: // sens bas
                        feu.draw(renderer, pos.getX()*w, 
                        (pos.getY()+(m.getTimer()-m.bouleF[i].timerB)/
                        (float)m.bouleF[i].vitessePos)*h,w,h);
                        break;
                    case 1: // sens haut
                        feu.draw(renderer, pos.getX()*w, (
                            pos.getY()-(m.getTimer()-m.bouleF[i].timerB)/
                            (float)m.bouleF[i].vitessePos)*h,w,h);
                        break;
                    case 2: // sens droite
                        feu.draw(renderer,(
                            pos.getX()+(m.getTimer()-m.bouleF[i].timerB)/
                            (float)m.bouleF[i].vitessePos)*w, pos.getY()*h,w,h);
                        break;
                    case 3: // sens gauche
                        feu.draw(renderer,
                        (pos.getX()-(m.getTimer()-m.bouleF[i].timerB)/
                        (float)m.bouleF[i].vitessePos)*w, pos.getY()*h,w,h);
                        break;
                    default: break;
                }
            }
            vieP.setTexte(renderer,font,("vie Perso : " +
                                        to_string(m.getViePerso())).c_str());
            vieP.setRect(0,0,15);
            vieP.afficheTexte(renderer);

            vieB.setTexte(renderer,font,("vie BOSS : " +
                                        to_string(m.getVieBOSS())).c_str());
            vieB.setRect(100,0,15);
            vieB.afficheTexte(renderer);
            
            SDL_RenderPresent(renderer);
            
            m.testReussie(jeu);
        }
        if (m.getReussie()){
             jeu.perso.pos.setX(m.tmpPerso.getX());
             jeu.perso.pos.setY(m.tmpPerso.getY());
        }
    
        if (!m.getEnCours() && !m.getReussie())  defaiteManche();            
    }
}
