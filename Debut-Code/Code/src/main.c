#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/creatures.h"
#include "../include/joueur.h"
#include "../include/combat.h"


int main() {
    printf("=== TEST GENERATION CREATURES MULTI-PROFONDEURS ===\n\n");

    srand(time(NULL));

    CreatureMarine groupe[MAX_CREATURES];
    int nbCreatures;

    // Test différentes profondeurs
    int profondeurs_test[] = {50, 100, 200, 300};
    int nb_tests = 4;
    Plongeur joueur = {100, 100, 30, 30, 0, 0};
    Creature requin = {C_KRAKEN, 80, 5, 15};

    attaque_plongeur(joueur, requin, 20, 30, 3);

    printf("PV du Requin: %d | Oxygène restant: %d | Fatigue: %d\n",
           requin.pv, joueur.niveau_oxygene, joueur.niveau_fatigue);

    return 0;
    for (int i = 0; i < nb_tests; i++) {
        printf("\n=== TEST Profondeur %dm ===\n", profondeurs_test[i]);
        genererGroupeCreatures(groupe, &nbCreatures, profondeurs_test[i]);
        afficherGroupeCreatures(groupe, nbCreatures);

        printf("\nAppuyez sur Entree pour continuer...");
        getchar();
    }

    return 0;
}