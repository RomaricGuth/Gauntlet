#include <iostream>
#include "Manche.h"
#include "Manche1.h"
#include "Manche2.h"
#include "Manche3.h"
#include "Manche4.h"
#include "Manche5.h"
#include "BOSS.h"
#include "Jeu.h"
using namespace std;

int main() {
    Jeu j;
    Manches m;
    Manche1 m1;
    Manche2 m2;
    Manche3 m3(j.perso.pos);
    Manche4 m4(j.perso.pos);
    Manche5 m5(j.perso.pos);
    BOSS m6(j.perso);

    if (m.testRegression() == -1) {
        cout<<"Erreur lors des tests du module manches"<<endl;
        return -1;
    }
    if (m1.testRegression() == -1) {
        cout<<"Erreur lors des tests de la manche 1"<<endl;
        return -1;
    }
    if (m2.testRegression() == -1) {
        cout<<"Erreur lors des tests de la manche 2"<<endl;
        return -1;
    }
    if (m3.testRegression() == -1) {
        cout<<"Erreur lors des tests de la manche 3"<<endl;
        return -1;
    }
    if (m4.testRegression() == -1) {
        cout<<"Erreur lors des tests de la manche 4"<<endl;
        return -1;
    }
    if (m5.testRegression() == -1) {
        cout<<"Erreur lors des tests de la manche 5"<<endl;
        return -1;
    }
    if (m6.testRegression() == -1) {
        cout<<"Erreur lors des tests du boss"<<endl;
        return -1;
    }
    cout<<"Toutes les manches sont fonctionnelles"<<endl;
    return 0;
}