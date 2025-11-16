#ifndef CARTE_H
#define CARTE_H

#include "joueur.h"

typedef struct Carte {
    int amodifier;
} Carte;

Carte* carte_charger_defaut(Joueur* joueur);
void   carte_detruire(Carte* c);
void   carte_reinitialiser(Carte* c);

#endif