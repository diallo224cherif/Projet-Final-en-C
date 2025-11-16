#ifndef CREATURES_H
#define CREATURES_H

typedef struct MoteurJeu MoteurJeu;
typedef struct Joueur Joueur;

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

int creatures_generation(MoteurJeu* jeu);
void creatures_postcombat(MoteurJeu* jeu);
GroupeCreatures* combat_obtenir_groupe(MoteurJeu* jeu);

#endif