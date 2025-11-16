#include "../include/combat.h"
#include "../include/joueur.h"
#include <stdio.h>
#include <stdlib.h>

void combat_afficher_etat(const MoteurJeu* jeu) {
    if (!jeu || !jeu->joueur) return;

    const Joueur* j = jeu->joueur;

    printf("\n=== ETAT DU COMBAT ===\n");
    printf("Profondeur : %d\n", jeu->profondeur);
    printf("PV Joueur  : %d/%d\n", j->pv, j->pv_max);
    printf("Oxygène    : %d/%d\n", j->oxygene, j->oxygene_max);
    printf("Fatigue    : %d\n", j->fatigue);
    if (j->paralysie > 0) {
        printf("Statut: PARALYSE (%d tour(s) restant(s))\n", j->paralysie);
    }
}

void combat_action_joueur(MoteurJeu* jeu, GroupeCreatures* groupe) {
    if (!jeu || !jeu->joueur || !groupe) return;

    Joueur* j = jeu->joueur;

    printf("\n=== TOUR DU JOUEUR ===\n");

    if (j->paralysie > 0) {
        printf("Vous êtes paralysé et ne pouvez pas agir ce tour.\n");
        j->paralysie --;
        return;
    }

    printf("1.Attaquer\n");
    printf("2.Utiliser une capsule d'oxygène (+20 oxygène)\n");
    printf("3.Tenter de fuir\n");

    int choix = moteur_choix("Choisissez une action", 1, 3);

    if (choix == 1) {
        int cible_index = -1;
        int i;
        for (i = 0; i < groupe->nb; i++) {
            if (groupe->tab[i].en_vie && groupe->tab[i].pv > 0) {
                cible_index = i;
                break;
            }
        }

    if (cible_index == -1) {
        printf("Aucune créature à attaquer.\n");
        return;
    }

    Creature* c = &groupe->tab[cible_index];

        int deg = j->attaque - c->def;
        if (deg < 1) deg = 1;
        printf("Vous attaquez la créature (type=%d) et infligez %d dégâts.\n",
            (int)c->type, deg);

        c->pv -= deg;
        if (c->pv <= 0) {
            c->pv = 0;
            c->en_vie = 0;
            printf("La créature est morte !\n");
        } else {
            printf("PV créature restante : %d/%d\n", c->pv, c->pv_max);
        }
    } else if (choix == 2) {
        int avant = j->oxygene;
        j->oxygene += 20;
        if (j->oxygene > j->oxygene_max) {
            j->oxygene = j->oxygene_max;
            printf("Vous utilisez une capsule d'oxygène: %d -> %d\n", avant, j->oxygene);
        } else {
            printf("Vous prenez la fuite!\n");
            int i;
            for (i = 0; i < groupe->nb; i++) {
                groupe->tab[i].en_vie = 0;
                groupe->tab[i].pv = 0;
            }
            groupe->nb = 0;
        }
    }
}


extern int joueur_mort(const Joueur* j);
extern int joueur_pv(const Joueur* j);
extern int joueur_oxygene(const Joueur* j);
extern int joueur_fatigue(const Joueur* j);

extern void joueur_consommation_oxygene(Joueur* j, int profondeur);

/* gestion retrait oxygene selon profondeur */

extern void joueur_recuperation_fatigue(Joueur* j, int delta);

/* groupe de creature pour ce combat */

extern GroupeCreatures* combat_obtenir_groupe(MoteurJeu* jeu);

/* verification creatures mortes */

static int combat_creatures_mortes(const GroupeCreatures* g) {
    int i;
    if (!g || !g->tab || g->nb <= 0) return 1; /* pas de créatures => toutes mortes */
    for (i = 0; i < g->nb; i++) {
        if (g->tab[i].en_vie && g->tab[i].pv > 0) {
            return 0; /* au moins une en vie */
        }
    }
    return 1; /* toutes mortes */
}

/* alerte oxygène */

static void combat_alerte_oxygene(const Joueur* j) {
    int oxy = joueur_oxygene(j);
    if (oxy <= 0) {
        printf("!!! VOUS N'AVEZ PLUS D'OXYGÈNE !!!\n");
    } else if (oxy <= 10) {
        printf("!!! ALERTE OXYGÈNE CRITIQUE (%d) !!!\n", oxy);
        printf("Utilisez une capsule ou remontez rapidement !\n");
    } else if (oxy <= 30) {
        printf("! Alerte oxygène faible (%d) !\n", oxy);
    }
}

/* fonction principale tour de combat */

int combat_resolution(MoteurJeu* jeu) {
    if (!jeu || !jeu->joueur) return 0;

    GroupeCreatures* groupe = combat_obtenir_groupe(jeu);
    if (!groupe) {
        return 1; /* pas de créatures => victoire */
    }

    combat_afficher_etat(jeu);

    combat_action_joueur(jeu, groupe);

    if (combat_creatures_mortes(groupe)) {
        return 1;
    }

    if (joueur_mort(jeu->joueur)) {
        return -1; /* défaite */
    }

    joueur_consommation_oxygene(jeu->joueur, jeu->profondeur);

    combat_alerte_oxygene(jeu->joueur);

    if (joueur_mort(jeu->joueur)) {
        return -1; /* défaite */
    }

    int nb_attaques = creatures_phase_attaque(jeu, groupe);
    printf("[Combat] %d attaques de créatures effectuées ce tour.\n", nb_attaques);

    if (joueur_mort(jeu->joueur)) {
        return -1;
    }

    joueur_recuperation_fatigue(jeu->joueur, 1);

    if (combat_creatures_mortes(groupe)) {
        return 1; /* victoire */
    }

    if (joueur_mort(jeu->joueur)) {
        return -1;
    }

    return 0; /* combat continue */
}

int calcul_degats(int attaque_min, int attaque_max, int defense) {
    int degats_base = rand() % (attaque_max - attaque_min + 1) + attaque_min;
    int degats = degats_base - defense;
    if (degats < 1) degats = 1;
    return degats;
}