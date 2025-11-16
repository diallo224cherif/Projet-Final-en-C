#include "sauvegarde.h"
#include "joueur.h"
#include <stdio.h>

int sauvegarde_ecrire(MoteurJeu* jeu) {
    if (!jeu || !jeu->joueur) return 0;

    FILE* f = fopen("save.txt", "w");
    if (!f) {
        printf("[Sauvegarde] Impossible d'ouvrir save.txt en ecriture.\n");
        return 0;
    }

    Joueur* j = jeu->joueur;

    fprintf(f, "%d %d %d %d %d\n",
            j->pv,
            j->oxygene,
            j->fatigue,
            j->paralysie,
            jeu->profondeur);

    fclose(f);
    printf("[Sauvegarde] Sauvegarde effectuee dans save.txt\n");
    return 1;
}

int sauvegarde_charger(MoteurJeu* jeu) {
    if (!jeu || !jeu->joueur) return 0;

    FILE* f = fopen("save.txt", "r");
    if (!f) {
        printf("[Sauvegarde] Aucune sauvegarde trouvee (save.txt).\n");
        return 0;
    }

    Joueur* j = jeu->joueur;
    int pv, oxy, fat, para, prof;

    int lus = fscanf(f, "%d %d %d %d %d",
                     &pv, &oxy, &fat, &para, &prof);
    fclose(f);

    if (lus != 5) {
        printf("[Sauvegarde] Fichier de sauvegarde invalide.\n");
        return 0;
    }

    if (pv < 0) pv = 0;
    if (pv > j->pv_max) pv = j->pv_max;

    if (oxy < 0) oxy = 0;
    if (oxy > j->oxygene_max) oxy = j->oxygene_max;

    if (fat < 0) fat = 0;
    if (para < 0) para = 0;

    j->pv        = pv;
    j->oxygene   = oxy;
    j->fatigue   = fat;
    j->paralysie = para;
    jeu->profondeur = prof;

    printf("[Sauvegarde] Sauvegarde chargee depuis save.txt\n");
    return 1;
}
