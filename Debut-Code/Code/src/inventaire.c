#include "inventaire.h"
#include <stdio.h>

void inventaire_afficher(MoteurJeu* jeu) {
    (void)jeu;
    printf("\n=== INVENTAIRE ===\n");
    printf("(L’inventaire n’est pas encore implemente.)\n");
}

EtatJeuId inventaire_interagir(MoteurJeu* jeu) {
    (void)jeu;
    printf("Appuyez sur Entree pour revenir a l’exploration.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    return ETAT_EXPLORATION;
}