#include "sauvegarde.h"
#include "joueur.h"
#include "inventaire.h"
#include <stdio.h>

int sauvegarde_ecrire(MoteurJeu* moteur) {
    if (!moteur || !moteur->joueur || !moteur->inventaire) return 0;

    FILE* f = fopen("save.dat", "w");
    if (!f) {
        printf("Impossible d'ouvrir le fichier de sauvegarde.\n");
        return 0;
    }

    Joueur* j        = moteur->joueur;
    Inventaire_s* inv = moteur->inventaire;

    /* Format simple texte */
    fprintf(f,
            "%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
            j->pv, j->pv_max,
            j->oxygene, j->oxygene_max,
            j->fatigue, j->paralysie,
            j->attaque, j->defense_base,
            j->niveau, j->experience,
            j->apnee, j->resistance, j->force);

    fprintf(f, "%d %d %d\n",
            inv->perles,
            inv->nb_capsules_oxygene,
            inv->nb_kits_soins);

    fprintf(f, "%d\n", moteur->profondeur);

    fclose(f);
    return 1;
}

int sauvegarde_charger(MoteurJeu* moteur) {
    if (!moteur || !moteur->joueur || !moteur->inventaire) return 0;

    FILE* f = fopen("save.dat", "r");
    if (!f) {
        printf("Aucune sauvegarde trouvee.\n");
        return 0;
    }

    Joueur* j         = moteur->joueur;
    Inventaire_s* inv = moteur->inventaire;

    int ok1 = fscanf(f,
                     "%d %d %d %d %d %d %d %d %d %d %d %d %d",
                     &j->pv, &j->pv_max,
                     &j->oxygene, &j->oxygene_max,
                     &j->fatigue, &j->paralysie,
                     &j->attaque, &j->defense_base,
                     &j->niveau, &j->experience,
                     &j->apnee, &j->resistance, &j->force);

    int ok2 = fscanf(f, "%d %d %d",
                     &inv->perles,
                     &inv->nb_capsules_oxygene,
                     &inv->nb_kits_soins);

    int ok3 = fscanf(f, "%d", &moteur->profondeur);

    fclose(f);

    if (ok1 != 13 || ok2 != 3 || ok3 != 1) {
        printf("Fichier de sauvegarde corrompu.\n");
        return 0;
    }

    if (j->pv > j->pv_max) j->pv = j->pv_max;
    if (j->oxygene > j->oxygene_max) j->oxygene = j->oxygene_max;

    return 1;
}