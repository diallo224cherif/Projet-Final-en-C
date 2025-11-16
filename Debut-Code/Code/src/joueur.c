#include "joueur.h"
#include <stdlib.h>
#include <stdio.h>

#define JOUEUR_PV_MAX        100 /* points de vie maximum */
#define JOUEUR_OXYGENE_MAX   100 /* oxygène maximum */
#define JOUEUR_DEF_BASE      5 /* défense de base */
#define JOUEUR_ATT_BASE      10 /* attaque de base */


Joueur* creerJoueur(void) {
    Joueur* j = (Joueur*)malloc(sizeof(Joueur));
    if (!j) return NULL;

    j->pv         = JOUEUR_PV_MAX;
    j->pv_max     = JOUEUR_PV_MAX;
    j->oxygene    = JOUEUR_OXYGENE_MAX;
    j->oxygene_max= JOUEUR_OXYGENE_MAX;
    j->fatigue    = 0;
    j->defense    = JOUEUR_DEF_BASE;
    j->attaque    = JOUEUR_ATT_BASE;
    j->paralysie  = 0;

    return j;
}

void joueur_detruire(Joueur* j) {
    if (!j) return;
    free(j);
}

void joueur_reinitialise(Joueur* j) {
    if (!j) return;
    j->pv         = j->pv_max;
    j->oxygene    = j->oxygene_max;
    j->fatigue    = 0;
    j->paralysie  = 0;
    j->defense    = JOUEUR_DEF_BASE;
    j->attaque    = JOUEUR_ATT_BASE;
}


void joueur_consommation_tick(Joueur* j) {
    if (!j) return;
    if (j->oxygene > 0) { /* oxygène diminue lentement */
        j->oxygene -= 1;
        if (j->oxygene < 0) j->oxygene = 0;
    }
    j->fatigue += 1; /* fatigue augmente lentement */
}

int joueur_pv(const Joueur* j) {
    if (!j) return 0;
    return j->pv;
}

int joueur_oxygene(const Joueur* j) {
    if (!j) return 0;
    return j->oxygene;
}

int joueur_fatigue(const Joueur* j) {
    if (!j) return 0;
    return j->fatigue;
}

int joueur_mort(const Joueur* j) {
    if (!j) return 1;
    return (j->pv <= 0);
}

int joueur_defense(const Joueur* j) {
    if (!j) return 0;
    return j->defense;
}

void joueur_degats_subis(Joueur* j, int degats) {
    if (!j) return;
    if (degats < 0) degats = 0;

    j->pv -= degats;
    if (j->pv < 0) j->pv = 0;

    printf("[Joueur] Subit %d dégâts, PV = %d/%d\n", degats, j->pv, j->pv_max);
}

void joueur_reduction_oxygene(Joueur* j, int delta) {
    if (!j) return;
    if (delta < 0) delta = 0;

    j->oxygene -= delta;
    if (j->oxygene < 0) j->oxygene = 0;
}

void joueur_consommation_oxygene(Joueur* j, int profondeur) {
    if (!j) return;

    int conso = 2 + (profondeur / 10); /* consommation augmente avec la profondeur */
    if (conso < 1) conso = 1; /* consommation minimale */

    j->oxygene -= conso;
    if (j->oxygene < 0) j->oxygene = 0;

    printf("[Joueur] Consomme %d oxygène (profondeur=%d). Oxygène = %d/%d\n",
           conso, profondeur, j->oxygene, j->oxygene_max);
}

void joueur_recuperation_fatigue(Joueur* j, int delta) {
    if (!j) return;
    if (delta < 0) delta = 0;

    j->fatigue -= delta;
    if (j->fatigue < 0) j->fatigue = 0;
}

void joueur_application_paralysie_tick(Joueur* j, int delta) {
    if (!j) return;
    if (delta < 0) delta = 0;

    j->paralysie += delta;
    if (j->paralysie > 3) j->paralysie = 3; /* max 3 tours de paralysie */
}
