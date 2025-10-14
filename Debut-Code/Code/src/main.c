#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "creatures.h"

int main() {
    printf("=== TEST GENERATION CREATURES MULTI-PROFONDEURS ===\n\n");

    srand(time(NULL));

    CreatureMarine groupe[MAX_CREATURES];
    int nbCreatures;

    // Test différentes profondeurs
    int profondeurs_test[] = {50, 100, 200, 300};
    int nb_tests = 4;

    for (int i = 0; i < nb_tests; i++) {
        printf("\n=== TEST Profondeur %dm ===\n", profondeurs_test[i]);
        genererGroupeCreatures(groupe, &nbCreatures, profondeurs_test[i]);
        afficherGroupeCreatures(groupe, nbCreatures);

        printf("\nAppuyez sur Entree pour continuer...");
        getchar();
    }

    return 0;
}