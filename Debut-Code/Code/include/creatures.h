#ifndef CREATURES_H
#define CREATURES_H

#define MAX_CREATURES 4

typedef struct {
    int id; // identifiant unique
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20]; // "paralysie", "poison", "aucun"
    int est_vivant;
} CreatureMarine;

// Prototypes
void initialiser_creature(CreatureMarine *creature, int id, const char *nom, int pv_max, int att_min, int att_max, int def, int vit, const char *effet);
void generer_creatures_zone(CreatureMarine creatures[], int *nb_creatures, int profondeur);
void afficher_creatures(const CreatureMarine creatures[], int nb_creatures);
void attaquer_creature(CreatureMarine *creature, int degats);
int creature_est_morte(const CreatureMarine *creature);
int calcul_degats_creature(const CreatureMarine *creature);

#endif