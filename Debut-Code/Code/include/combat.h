#ifndef COMBAT_H
#define COMBAT_H
#include "moteur.h"


int combat_resolution(MoteurJeu* jeu);
void attaque_plongeur(Plongeur p, Creaturec, int attaque_min, int attaque_max, int consommation_oxygene);

#endif