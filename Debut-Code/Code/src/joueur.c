#include "joueur.h"
#include <stdlib.h>
#include <stdio.h>

Joueur* creerJoueur(void) {
    Joueur* j = (Joueur*)malloc(sizeof(Joueur));
    if (!j) return NULL;

    j->pv_max = 100;
    j->pv     = j->pv_max;

    j->oxygene_max = 100;
    j->oxygene     = j->oxygene_max;

    j->fatigue   = 0;
    j->paralysie = 0;

    j->attaque      = 10;
    j->defense_base = 3;

    j->niveau     = 1;
    j->experience = 0;

    j->apnee      = 0;
    j->resistance = 0;
    j->force      = 0;

    return j;
}

void joueur_detruire(Joueur* j) {
    if (!j) return;
    free(j);
}

void joueur_reinitialise(Joueur* j) {
    if (!j) return;

    j->pv     = j->pv_max = 100;
    j->oxygene     = j->oxygene_max = 100;
    j->fatigue     = 0;
    j->paralysie   = 0;

    j->attaque      = 10;
    j->defense_base = 3;

    j->niveau     = 1;
    j->experience = 0;

    j->apnee      = 0;
    j->resistance = 0;
    j->force      = 0;
}

/* ====== Etat simple ====== */

int joueur_mort(const Joueur* j) {
    return !j || j->pv <= 0 || j->oxygene <= 0;
}

int joueur_pv(const Joueur* j) {
    return j ? j->pv : 0;
}

int joueur_oxygene(const Joueur* j) {
    return j ? j->oxygene : 0;
}

int joueur_fatigue(const Joueur* j) {
    return j ? j->fatigue : 0;
}

/* ====== Combat / stats ====== */

void joueur_degats_subis(Joueur* j, int degats) {
    if (!j || degats <= 0) return;
    j->pv -= degats;
    if (j->pv < 0) j->pv = 0;
    printf("[Joueur] Subit %d degats, PV = %d/%d\n", degats, j->pv, j->pv_max);
}

int joueur_defense(const Joueur* j) {
    if (!j) return 0;
    return j->defense_base + j->resistance;
}

void joueur_reduction_oxygene(Joueur* j, int delta) {
    if (!j || delta <= 0) return;
    j->oxygene -= delta;
    if (j->oxygene < 0) j->oxygene = 0;
    if (j->oxygene > j->oxygene_max) j->oxygene = j->oxygene_max;
}

void joueur_application_paralysie_tick(Joueur* j, int delta) {
    if (!j || delta <= 0) return;
    j->paralysie += delta;
    if (j->paralysie > 3) j->paralysie = 3; /* cap pour eviter le perma stun */
}

/* Consommation a chaque deplacement sur la carte */
void joueur_consommation_tick(Joueur* j) {
    if (!j) return;
    int conso = 1;
    if (j->apnee > 0) conso -= j->apnee;
    if (conso < 0) conso = 0;

    joueur_reduction_oxygene(j, conso);
    j->fatigue++;
}

/* Consommation par tour de combat, depend de la profondeur */
void joueur_consommation_oxygene(Joueur* j, int profondeur) {
    if (!j) return;
    int conso = 2 + profondeur / 10; /* 0m -> 2, 50m -> 7, 150m -> 17 */
    conso -= j->apnee;               /* apnee reduit la conso */
    if (conso < 1) conso = 1;

    int avant = j->oxygene;
    joueur_reduction_oxygene(j, conso);
    printf("[Joueur] Consomme %d oxygene (profondeur=%d). Oxygene = %d/%d\n",
           conso, profondeur, j->oxygene, j->oxygene_max);
}

void joueur_recuperation_fatigue(Joueur* j, int delta) {
    if (!j || delta <= 0) return;
    j->fatigue -= delta;
    if (j->fatigue < 0) j->fatigue = 0;
}

/* ====== XP / competences ====== */

void joueur_ajouter_experience(Joueur* j, int xp) {
    if (!j || xp <= 0) return;

    j->experience += xp;
    while (j->experience >= 100) {
        j->experience -= 100;
        j->niveau++;

        j->pv_max      += 10;
        j->oxygene_max += 5;

        j->pv      = j->pv_max;
        j->oxygene = j->oxygene_max;

        printf("[Niveau] Vous passez niveau %d ! PV=%d, O2=%d\n",
               j->niveau, j->pv_max, j->oxygene_max);
    }
}

void joueur_debloquer_competence(Joueur* j) {
    if (!j) return;

    if (j->niveau >= 2 && j->apnee == 0) {
        j->apnee = 1;
        printf("[Competence] Vous debloquez Apnee (conso d'oxygene reduite) !\n");
    }
    if (j->niveau >= 3 && j->resistance == 0) {
        j->resistance = 2;
        printf("[Competence] Vous debloquez Resistance (+DEF) !\n");
    }
    if (j->niveau >= 4 && j->force == 0) {
        j->force = 3;
        j->attaque += j->force;
        printf("[Competence] Vous debloquez Force (+ATQ) !\n");
    }
}