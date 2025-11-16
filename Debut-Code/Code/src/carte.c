#include "../include/carte.h"
#include <stdlib.h>
#include <stdio.h>

Carte* carte_charger_defaut(void) {
    Carte* c = (Carte*)malloc(sizeof(Carte));
    if (!c) {
        printf("[Carte] Erreur d’allocation de la carte.\n");
        return NULL;
    }
    c->amodifier = 0;
    printf("[Carte] Carte par defaut chargee.\n");
    return c;
}

void carte_detruire(Carte* c) {
    if (!c) return;
    free(c);
    printf("[Carte] Carte detruite.\n");
}

void carte_reinitialiser(Carte* c) {
    if (!c) return;
    c->amodifier = 0;
    printf("[Carte] Carte reinitialisee.\n");
}