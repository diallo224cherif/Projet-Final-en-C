#include "joueur.h"
#include <stdlib.h>
#include <stdio.h>

#define JOUEUR_PV_MAX        100
#define JOUEUR_OXYGENE_MAX   100
#define JOUEUR_DEF_BASE      5
#define JOUEUR_ATT_BASE      10
#define XP_SEUIL             100

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

    j->experience = 0;
    j->niveau     = 1;
    j->apnee      = 1;
    j->resistance = 1;
    j->force      = 1;

    return j;
}

void joueur_detruire(Joueur* j) {
    if (j) free(j);
}

void joueur_reinitialise(Joueur* j) {
    if (!j) return;
    j->pv      = j->pv_max;
    j->oxygene = j->oxygene_max;
    j->fatigue = 0;
    j->paralysie = 0;
    j->defense = JOUEUR_DEF_BASE;
    j->attaque = JOUEUR_ATT_BASE;
}

void joueur_consommation_tick(Joueur* j) {
    if (!j) return;
    if (j->oxygene > 0) {
        j->oxygene -= 1;
        if (j->oxygene < 0) j->oxygene = 0;
    }
    j->fatigue += 1;
}

int joueur_pv(const Joueur* j) { return j ? j->pv : 0; }
int joueur_oxygene(const Joueur* j) { return j ? j->oxygene : 0; }
int joueur_fatigue(const Joueur* j) { return j ? j->fatigue : 0; }
int joueur_mort(const Joueur* j) { return (!j || j->pv <= 0); }
int joueur_defense(const Joueur* j) { return j ? j->defense : 0; }

void joueur_degats_subis(Joueur* j, int degats) {
    if (!j) return;
    if (degats < 0) degats = 0;
    j->pv -= degats;
    if (j->pv < 0) j->pv = 0;
    printf("[Joueur] Subit %d degats, PV = %d/%d\n", degats, j->pv, j->pv_max);
}

void joueur_reduction_oxygene(Joueur* j, int delta) {
    if (!j) return;
    if (delta < 0) delta = 0;
    j->oxygene -= delta;
    if (j->oxygene < 0) j->oxygene = 0;
}

void joueur_consommation_oxygene(Joueur* j, int profondeur) {
    if (!j) return;
    int conso = 2 + (profondeur / 10);
    if (conso < 1) conso = 1;
    j->oxygene -= conso;
    if (j->oxygene < 0) j->oxygene = 0;
    printf("[Joueur] Consomme %d oxygene (profondeur=%d). Oxygene = %d/%d\n",
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
    if (j->paralysie > 3) j->paralysie = 3;
}

/* --- Système de progression amélioré --- */
void joueur_ajouter_experience(Joueur* j, int xp) {
    if (!j) return;
    j->experience += xp;

    while (j->experience >= XP_SEUIL) {
        j->experience -= XP_SEUIL;
        j->niveau++;


        j->apnee      *= 2;
        j->resistance *= 2;
        j->force      *= 2;

        printf("[Joueur] Niveau %d atteint ! Capacités améliorées (+10%%).\n", j->niveau);

        joueur_debloquer_competence(j);
    }
}

void joueur_debloquer_competence(Joueur* j) {
    if (!j) return;
    if (j->niveau == 2) {
        printf("[Compétence] Débloquée : Coup puissant (+20%% dégâts)!\n");
    } else if (j->niveau == 3) {
        printf("[Compétence] Débloquée : Respiration contrôlée (+15%% oxygène)!\n");
    } else if (j->niveau == 5) {
        printf("[Compétence] Débloquée : Plongée rapide!\n");
    }
}