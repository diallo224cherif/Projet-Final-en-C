#ifndef CREATURES_H
#define CREATURES_H

#include <stdlib.h>
#include "moteur.h"

#define MAX_CREATURES 4
#define TAILLE_NOM 30

typedef struct {
    int id;
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20];
    int est_vivant;
} CreatureMarine;

// Prototypes
void initialiserCreature(CreatureMarine *creature, int id, const char *nom, int pv_max, int att_min, int att_max, int def, int vit, const char *effet);
void genererCreature(CreatureMarine *creature, int id, int profondeur);
void genererGroupeCreatures(CreatureMarine groupe[], int *nbCreatures, int profondeur);
void afficherCreature(const CreatureMarine *creature);
void afficherGroupeCreatures(const CreatureMarine groupe[], int nbCreatures);
int estGroupeVivant(const CreatureMarine groupe[], int nbCreatures);

/* type de creature */
typedef enum {
    C_KRAKEN = 0,
    C_MEDUSE,
    C_REQUIN,
    C_POISSON_EPEE,
    C_CRABE_GEANT
} TypeCreature;

/* effets persistants par creature */
typedef struct {
    int carapace_active; /* réduit les dégâts subis */
} EffetsCreature;

/* modèle de creature */
typedef struct {
    TypeCreature type;
    int pv;
    int pv_max;
    int att;
    int def;
    int vitesse;
    EffetsCreature effets;
    int en_vie; /* evite test de vie négatif */
} Creature;

typedef struct {
    Creature* tab;
    int nb;
} GroupeCreatures;

int creatures_phase_attaque(MoteurJeu* jeu, GroupeCreatures* groupe);

Creature creature_creer(TypeCreature t, int pv, int pv_max, int att, int def, int vitesse);

#endif