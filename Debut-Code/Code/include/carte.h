#ifndef CARTE_H
#define CARTE_H

typedef struct Carte {
    int amodifier;
} Carte;

Carte* carte_charger_defaut(void);
void   carte_detruire(Carte* c);
void   carte_reinitialiser(Carte* c);

#endif