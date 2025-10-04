#include <stdio.h>
#include "joueur.h"

void initialiser_plongeur(Plongeur *plongeur) {
    plongeur->points_de_vie_max = 100;
    plongeur->points_de_vie = 100;
    plongeur->niveau_oxygene_max = 100;
    plongeur->niveau_oxygene = 100;
    plongeur->niveau_fatigue = 0;
    plongeur->perles = 0;
}

void afficher_etat_plongeur(const Plongeur *plongeur) {
    printf("\n=== ÉTAT DU PLONGEUR ===\n");
    printf("Vie: %d/%d\n", plongeur->points_de_vie, plongeur->points_de_vie_max);
    printf("Oxygène: %d/%d", plongeur->niveau_oxygene, plongeur->niveau_oxygene_max);

    if (oxygene_critique(plongeur)) {
        printf(" ⚠️ CRITIQUE!");
    }
    printf("\n");

    printf("Fatigue: %d/5\n", plongeur->niveau_fatigue);
    printf("Perles: %d\n", plongeur->perles);

    // Barre de fatigue visuelle
    printf("Fatigue: [");
    for (int i = 0; i < 5; i++) {
        if (i < plongeur->niveau_fatigue) {
            printf("█");
        } else {
            printf("_");
        }
    }
    printf("]\n");
}

void modifier_vie(Plongeur *plongeur, int changement) {
    plongeur->points_de_vie += changement;

    // Limites
    if (plongeur->points_de_vie > plongeur->points_de_vie_max) {
        plongeur->points_de_vie = plongeur->points_de_vie_max;
    }
    if (plongeur->points_de_vie < 0) {
        plongeur->points_de_vie = 0;
    }
}

void modifier_oxygene(Plongeur *plongeur, int changement) {
    plongeur->niveau_oxygene += changement;

    // Limites
    if (plongeur->niveau_oxygene > plongeur->niveau_oxygene_max) {
        plongeur->niveau_oxygene = plongeur->niveau_oxygene_max;
    }
    if (plongeur->niveau_oxygene < 0) {
        plongeur->niveau_oxygene = 0;
    }
}

void modifier_fatigue(Plongeur *plongeur, int changement) {
    plongeur->niveau_fatigue += changement;

    // Limites 0-5
    if (plongeur->niveau_fatigue > 5) {
        plongeur->niveau_fatigue = 5;
    }
    if (plongeur->niveau_fatigue < 0) {
        plongeur->niveau_fatigue = 0;
    }
}

void ajouter_perles(Plongeur *plongeur, int quantite) {
    plongeur->perles += quantite;
    if (plongeur->perles < 0) {
        plongeur->perles = 0;
    }
}

int est_en_vie(const Plongeur *plongeur) {
    return plongeur->points_de_vie > 0;
}

int oxygene_critique(const Plongeur *plongeur) {
    return plongeur->niveau_oxygene <= 10;
}