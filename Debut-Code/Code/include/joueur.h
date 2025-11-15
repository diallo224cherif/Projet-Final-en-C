#ifndef COMBAT_H
#define COMBAT_H
#include "moteur.h"
#include "creatures.h"

int combat_resolution(MoteurJeu* jeu);
typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0 à 5
    int perles;
} Plongeur;



#endif