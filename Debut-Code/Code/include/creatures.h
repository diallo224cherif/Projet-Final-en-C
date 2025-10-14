#ifndef CREATURES_H
#define CREATURES_H

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

#endif