#ifndef CREATURES_H
#define CREATURES_H

#include "joueur.h"

/* forward declaration du moteur, sans typedef */
struct MoteurJeu;

typedef enum {
    C_KRAKEN = 0,
    C_MEDUSE,
    C_REQUIN,
    C_POISSON_EPEE,
    C_CRABE_GEANT,
    C_NB_TYPES
} TypeCreature;

typedef struct {
    int carapace_active; /* pour le crabe, etc. */
} EffetsCreature;

typedef struct {
    TypeCreature type;
    int pv;
    int pv_max;
    int att;
    int def;
    int vitesse;
    EffetsCreature effets;
    int en_vie;
} Creature;

typedef struct {
    Creature* tab;
    int       nb;
} GroupeCreatures;

/* API */
const char*      creature_nom(TypeCreature t);
Creature         creature_creer(TypeCreature t,
                                int pv, int pv_max,
                                int att, int def, int vitesse);

int              creatures_phase_attaque(struct MoteurJeu* jeu,
                                         GroupeCreatures* groupe);
int              creatures_generation(struct MoteurJeu* jeu);
void             creatures_postcombat(struct MoteurJeu* jeu);
GroupeCreatures* combat_obtenir_groupe(struct MoteurJeu* jeu);

#endif