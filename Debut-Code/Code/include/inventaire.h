#ifndef INVENTAIRE_H
#define INVENTAIRE_H

#include "joueur.h"

typedef struct Inventaire_s {
    int perles;
    int nb_capsules_oxygene;
    int nb_kits_soins;
} Inventaire_s;

Inventaire_s* inventaire_creer(void);
void          inventaire_detruire(Inventaire_s* inv);

/* Interaction complète (affichage + choix + retour) */
void inventaire_interagir(Inventaire_s* inv, Joueur* joueur);

/* Récompenses après combat */
void distribuer_recompenses_combat(Inventaire_s* inv,
                                   int profondeur,
                                   int nb_creatures_vaincues);

#endif