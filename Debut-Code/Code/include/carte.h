#ifndef CARTE_H
#define CARTE_H

typedef struct Carte Carte;

typedef enum {
    LIEU_BASE = 0,
    LIEU_OCEAN,
    LIEU_BATEAU,
    LIEU_RECIF,
    LIEU_EPAVE,
    LIEU_ALGUES,
    LIEU_GROTTE,
    LIEU_ZONE_BOSS,
    LIEU_BLOQUE
} TypeLieu;

#define CARTE_LIGNES 3
#define CARTE_COLONNES 4

Carte* carte_charger_defaut(struct Joueur* joueur); // ✅ Ajout paramètre Joueur
void   carte_reinitialiser(Carte* c);
void   carte_detruire(Carte* c);
void   carte_afficher(const Carte* c);
int    carte_deplacer(Carte* c, int dx, int dy);
int    carte_profondeur_actuelle(const Carte* c);
int    carte_case_sure(const Carte* c);
int    carte_nb_ennemis_max(const Carte* c);
int    carte_case_tresor(const Carte* c);

#endif