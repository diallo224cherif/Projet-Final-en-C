#ifndef CARTE_H
#define CARTE_H

#include "joueur.h"
#include "creatures.h"

#define MAX_ZONES 12
#define TAILLE_CARTE 4

typedef enum {
    SURFACE,
    RECIF,
    EPAVE,
    ALGUES,
    GROTTE,
    FOSSE,
    INCONNU
} TypeZone;

typedef struct {
    int x, y;
    TypeZone type;
    char nom[30];
    int profondeur;
    int nb_creatures;
    int a_tresor;
    int est_decouverte;
    int est_accessible;
} Zone;

typedef struct {
    Zone zones[TAILLE_CARTE][TAILLE_CARTE];
    int position_x;
    int position_y;
    int profondeur_max;
} Carte;

// Prototypes
void initialiser_carte(Carte *carte);
void afficher_carte(const Carte *carte);
void afficher_zone_actuelle(const Carte *carte);
int deplacer_joueur(Carte *carte, int direction);
int peut_se_deplacer(const Carte *carte, int direction);
void explorer_zone(Carte *carte);
void afficher_carte_avancee(const Carte *carte, const Plongeur *joueur, const CreatureMarine creatures[], int nb_creatures);
int zone_contient_creatures(const Carte *carte, int x, int y, const CreatureMarine creatures[], int nb_creatures);

// Directions
#define DIR_HAUT 0
#define DIR_BAS 1
#define DIR_GAUCHE 2
#define DIR_DROITE 3

#endif