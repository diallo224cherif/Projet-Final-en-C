#include "moteur.h"
#include <stdio.h>


int main(void) {
    MoteurJeu jeu;

    if (!demarrage_moteur(&jeu)) {
        printf("Erreur au demarrage du moteur de jeu.\n");
        return 1;
    }

    lancer_moteur(&jeu);
    stop_moteur(&jeu);

    return 0;
}