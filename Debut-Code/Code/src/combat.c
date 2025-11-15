#include "combat.h"
#include "creatures.h"
#include "joueur.h"
#include <stdio.h>
#include <stdlib.h>

extern int joueur_mort(const Joueur* j);
extern int joueur_pv(const Joueur* j);
extern int joueur_oxygene(const Joueur* j);
extern int joueur_fatigue(const Joueur* j);

extern void joueur_consommation_oxygene(Joueur* j, int profondeur);

/* gestion retrait oxygene selon profondeur */

extern void joueur_recuperation_fatigue(Joueur* j, int delta);

/* affichage et action joueur */
extern void combat_afficher_etat(const MoteurJeu* jeu);

extern void combat_action_joueur(MoteurJeu* jeu, GroupeCreatures* groupe);

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
    if (oxy <= 10) {
        printf("!!! ALERTE OXYGÈNE CRITIQUE (%d) !!!\n", oxy);
        printf("Utilisez une capsule ou remontez rapidement !\n");
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