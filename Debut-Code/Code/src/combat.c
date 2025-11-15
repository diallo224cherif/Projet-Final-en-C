#include "../include/creatures.h"
#include "../include/joueur.h"
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

int calcul_degats(int attaque_min, int attaque_max, int defense) {
    int degats_base = rand() % (attaque_max - attaque_min + 1) + attaque_min;
    int degats = degats_base - defense;
    if (degats < 1) degats = 1;
    return degats;
}

void attaque_plongeur(Plongeur p, Creature c, int attaque_min, int attaque_max, int consommation_oxygene) {
    if (p.niveau_fatigue >= 5) {
        printf("Vous êtes trop fatigué pour attaquer !\n");
        return;
    }

    // Consommation d’oxygène
    p.niveau_oxygene -= consommation_oxygene;
    if (p.niveau_oxygene <= 10 && p.niveau_oxygene > 0)
        printf("\033[1;31mALERTE CRITIQUE : Niveau d'oxygène bas (%d)\033[0m\n", p.niveau_oxygene);
    else if (p.niveau_oxygene <= 0) {
        printf("\033[1;31mOXYGÈNE ÉPUISÉ ! Vous perdez 5 PV par tour !\033[0m\n");
        p.points_de_vie -= 5;
    }

    // Calcul des dégâts
        int degats = calcul_degats(attaque_min, attaque_max, c.def);
    c.pv -= degats;
    if (c.pv < 0) c.pv = 0;

    // Fatigue
    p.niveau_fatigue++;

    printf("Vous attaquez %s et infligez %d dégâts !\n", c.type, degats);
    printf("Oxygène consommé: -%d | Fatigue augmentée: +1\n", consommation_oxygene);
}
