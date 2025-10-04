#ifndef JOUEUR_H
#define JOUEUR_H

typedef struct {
    int points_de_vie;
    int points_de_vie_max;
    int niveau_oxygene;
    int niveau_oxygene_max;
    int niveau_fatigue; // 0 à 5
    int perles; // monnaie du jeu
} Plongeur;

// Prototypes
void initialiser_plongeur(Plongeur *plongeur);
void afficher_etat_plongeur(const Plongeur *plongeur);
void modifier_vie(Plongeur *plongeur, int changement);
void modifier_oxygene(Plongeur *plongeur, int changement);
void modifier_fatigue(Plongeur *plongeur, int changement);
void ajouter_perles(Plongeur *plongeur, int quantite);
int est_en_vie(const Plongeur *plongeur);
int oxygene_critique(const Plongeur *plongeur);

#endif