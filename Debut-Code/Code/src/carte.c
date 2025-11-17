#include "carte.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    TypeLieu type;
    int safe;
    int nb_ennemis_max;
    int tresor;
    int shop;
} CaseCarte;

struct Carte {
    CaseCarte cases[CARTE_LIGNES][CARTE_COLONNES];
    int pos_ligne;
    int pos_colonne;
};

static void case_init(CaseCarte* c, TypeLieu t, int safe,
                      int nb_enemis, int tresor, int shop) {
    c->type           = t;
    c->safe           = safe;
    c->nb_ennemis_max = nb_enemis;
    c->tresor         = tresor;
    c->shop           = shop;
}

Carte* carte_charger_defaut(Joueur* joueur) {
    Carte* c = (Carte*)malloc(sizeof(Carte));
    if (!c) return NULL;

    /* Ligne 0 */
    case_init(&c->cases[0][0], LIEU_BASE,   1, 0, 0, 0);
    case_init(&c->cases[0][1], LIEU_OCEAN,  1, 0, 0, 0);
    case_init(&c->cases[0][2], LIEU_OCEAN,  1, 0, 0, 0);
    case_init(&c->cases[0][3], LIEU_BATEAU, 1, 0, 0, 1);

    /* Ligne 1 */
    case_init(&c->cases[1][0], LIEU_RECIF,  0, 3, 0, 0);
    case_init(&c->cases[1][1], LIEU_EPAVE,  0, 2, 1, 0);
    case_init(&c->cases[1][2], LIEU_ALGUES, 0, 5, 0, 0);
    case_init(&c->cases[1][3], LIEU_GROTTE, 1, 0, 0, 0);

    /* Ligne 2 */
    case_init(&c->cases[2][0], LIEU_ZONE_BOSS, 0, 4, 0, 0);
    case_init(&c->cases[2][1], LIEU_BLOQUE,    1, 0, 0, 0);
    case_init(&c->cases[2][2], LIEU_ZONE_BOSS, 0, 4, 0, 0);
    case_init(&c->cases[2][3], LIEU_BLOQUE,    1, 0, 0, 0);

    c->pos_ligne   = 0;
    c->pos_colonne = 0;

    printf("[Carte] Carte par defaut chargee.\n");

    if (joueur) {
        joueur_ajouter_experience(joueur, 10);
        joueur_debloquer_competence(joueur);
    }

    return c;
}

void carte_reinitialiser(Carte* c) {
    if (!c) return;
    c->pos_ligne   = 0;
    c->pos_colonne = 0;
}

void carte_detruire(Carte* c) {
    if (!c) return;
    free(c);
}

void carte_afficher(const Carte* c) {
    if (!c) return;

    int l, col;
    printf("\n===== CARTOGRAPHIE OCEANIQUE =====\n\n");
    for (l = 0; l < CARTE_LIGNES; l++) {
        for (col = 0; col < CARTE_COLONNES; col++) {
            const CaseCarte* cc = &c->cases[l][col];
            char symbole = '?';

            switch (cc->type) {
                case LIEU_BASE:      symbole = 'B'; break;
                case LIEU_OCEAN:     symbole = 'O'; break;
                case LIEU_BATEAU:    symbole = 'S'; break;
                case LIEU_RECIF:     symbole = 'R'; break;
                case LIEU_EPAVE:     symbole = 'E'; break;
                case LIEU_ALGUES:    symbole = 'A'; break;
                case LIEU_GROTTE:    symbole = 'G'; break;
                case LIEU_ZONE_BOSS: symbole = 'X'; break;
                case LIEU_BLOQUE:    symbole = '#'; break;
                default:             symbole = '?'; break;
            }

            if (l == c->pos_ligne && col == c->pos_colonne) {
                printf("[%c] ", symbole);
            } else {
                printf(" %c  ", symbole);
            }
        }
        int prof = (l == 0) ? 0 : (l == 1) ? 50 : 150;
        printf("(profondeur ~%dm)\n", prof);
    }

    printf("\nLegende : B=Base, O=Ocean, S=Bateau (Shop), R=Recif, "
           "E=Epave, A=Algues, G=Grotte, X=Zone Boss, #=Bloque\n");
}

int carte_deplacer(Carte* c, int dx, int dy) {
    if (!c) return 0;

    int nouvelle_ligne   = c->pos_ligne + dy;
    int nouvelle_colonne = c->pos_colonne + dx;

    if (nouvelle_ligne < 0 || nouvelle_ligne >= CARTE_LIGNES ||
        nouvelle_colonne < 0 || nouvelle_colonne >= CARTE_COLONNES) {
        printf("Deplacement hors limites de la carte.\n");
        return 0;
    }

    if (c->cases[nouvelle_ligne][nouvelle_colonne].type == LIEU_BLOQUE) {
        printf("Vous ne pouvez pas aller par la.\n");
        return 0;
    }

    c->pos_ligne   = nouvelle_ligne;
    c->pos_colonne = nouvelle_colonne;
    return 1;
}

int carte_profondeur_actuelle(const Carte* c) {
    if (!c) return 0;
    if (c->pos_ligne == 0) return 0;
    if (c->pos_ligne == 1) return 50;
    if (c->pos_ligne == 2) return 150;
    return 0;
}

int carte_case_sure(const Carte* c) {
    if (!c) return 1;
    const CaseCarte* cc = &c->cases[c->pos_ligne][c->pos_colonne];
    return cc->safe;
}

int carte_nb_ennemis_max(const Carte* c) {
    if (!c) return 0;
    const CaseCarte* cc = &c->cases[c->pos_ligne][c->pos_colonne];
    return cc->nb_ennemis_max;
}

int carte_case_tresor(const Carte* c) {
    if (!c) return 0;
    const CaseCarte* cc = &c->cases[c->pos_ligne][c->pos_colonne];
    return cc->tresor;
}