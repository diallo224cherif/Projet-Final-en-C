#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moteur.c"

#include <time.h>
#ifndef CREATURES_H
#define CREATURES_H

#include <stdlib.h>

#define MAX_CREATURES 4
#define TAILLE_NOM 30

typedef struct {
    int id;
    char nom[30];
    int points_de_vie_max;
    int points_de_vie_actuels;
    int attaque_minimale;
    int attaque_maximale;
    int defense;
    int vitesse;
    char effet_special[20];
    int est_vivant;
} CreatureMarine;



/* type de creature */
typedef enum {
    C_KRAKEN = 0,
    C_MEDUSE,
    C_REQUIN,
    C_POISSON_EPEE,
    C_CRABE_GEANT
} TypeCreature;

/* effets persistants par creature */
typedef struct {
    int carapace_active; /* réduit les dégâts subis */
} EffetsCreature;

/* modèle de creature */
typedef struct {
    TypeCreature type;
    int pv;
    int pv_max;
    int att;
    int def;
    int vitesse;
    EffetsCreature effets;
    int en_vie; /* evite test de vie négatif */
} Creature;

typedef struct {
    Creature* tab;
    int nb;
} GroupeCreatures;


#endif

// Initialise une créature avec des valeurs spécifiques
void initialiserCreature(CreatureMarine *creature, int id, const char *nom, int pv_max, int att_min, int att_max, int def, int vit, const char *effet) {
    creature->id = id;
    strncpy(creature->nom, nom, TAILLE_NOM - 1);
    creature->nom[TAILLE_NOM - 1] = '\0';
    creature->points_de_vie_max = pv_max;
    creature->points_de_vie_actuels = pv_max;
    creature->attaque_minimale = att_min;
    creature->attaque_maximale = att_max;
    creature->defense = def;
    creature->vitesse = vit;
    strncpy(creature->effet_special, effet, 19);
    creature->effet_special[19] = '\0';
    creature->est_vivant = 1;
}

// Génère une créature aléatoire selon la profondeur
void genererCreature(CreatureMarine *creature, int id, int profondeur) {
    if (profondeur < 100) {
        // Créatures faciles
        int type = rand() % 3;
        switch (type) {
            case 0: initialiserCreature(creature, id, "Meduse Bleue", 30, 8, 15, 2, 12, "paralysie"); break;
            case 1: initialiserCreature(creature, id, "Poisson-Epee", 80, 18, 28, 5, 15, "perforant"); break;
            case 2: initialiserCreature(creature, id, "Crabe Geant", 100, 12, 20, 10, 8, "carapace"); break;
        }
    } else if (profondeur < 250) {
        // Créatures moyennes
        int type = rand() % 2;
        switch (type) {
            case 0: initialiserCreature(creature, id, "Requin-Tigre", 90, 15, 25, 7, 18, "frenesie"); break;
            case 1: initialiserCreature(creature, id, "Kraken", 150, 25, 40, 12, 10, "tentacule"); break;
        }
    } else {
        // Créatures difficiles
        initialiserCreature(creature, id, "Kraken Geant", 180, 30, 45, 15, 9, "tentacule");
    }
}

// Génère un groupe de 1 à 4 créatures
void genererGroupeCreatures(CreatureMarine groupe[], int *nbCreatures, int profondeur) {

    *nbCreatures = (rand() % 4) + 1; // Entre 1 et 4 créatures

    for (int i = 0; i < *nbCreatures; i++) {
        genererCreature(&groupe[i], i + 1, profondeur);
    }
}

// Affiche les informations d'une créature
void afficherCreature(const CreatureMarine *creature) {
    printf("[%d] %s (%d/%d PV) - ATK: %d-%d - DEF: %d - VIT: %d - Effet: %s\n",
           creature->id,
           creature->nom,
           creature->points_de_vie_actuels,
           creature->points_de_vie_max,
           creature->attaque_minimale,
           creature->attaque_maximale,
           creature->defense,
           creature->vitesse,
           creature->effet_special);
}

// Affiche tout le groupe de créatures
void afficherGroupeCreatures(const CreatureMarine groupe[], int nbCreatures) {
    printf("\n--- CREATURES RENCONTREES ---\n");
    for (int i = 0; i < nbCreatures; i++) {
        if (groupe[i].est_vivant) {
            afficherCreature(&groupe[i]);
        } else {
            printf("[%d] %s (MORT)\n", groupe[i].id, groupe[i].nom);
        }
    }
    printf("-----------------------------\n");
}

// Vérifie si au moins une créature est en vie
int estGroupeVivant(const CreatureMarine groupe[], int nbCreatures) {
    for (int i = 0; i < nbCreatures; i++) {
        if (groupe[i].est_vivant) return 1;
    }
    return 0;
}

extern void joueur_degats_subis(Joueur* j, int degats);
extern int joueur_defense(const Joueur* j);
extern void joueur_reduction_oxygene(Joueur* j, int delta);
extern void joueur_application_paralysie_tick(Joueur* j, int delta);

Creature creature_creation(TypeCreature t, int pv, int pv_max, int att, int def, int vitesse) {
    Creature c;
    c.type = t;
    c.pv = pv;
    c.pv_max = pv_max;
    c.att = att;
    c.def = def;
    c.vitesse = vitesse;
    c.effets.carapace_active = (t == C_CRABE_GEANT) ? 1 : 0; /* carapace active par défaut */
    c.en_vie = (pv > 0);
    return c;
}

static int calcul_degats_sur_joueur(const Creature* c, const Joueur* j) {
    int def_j = joueur_defense(j);
    int att_c = c->att;

    /*cas special poisson épée*/
    if (c->type == C_POISSON_EPEE) {
        def_j -= 2;
        if (def_j < 0) def_j = 0;
    }

    int degats = att_c - def_j;
    if (degats < 1) degats = 1; /* minimum 1 dégât */

    if (c->type == C_REQUIN && c->pv > 0) {
        if (c->pv*2 < c->pv_max){ degats = (degats * 130)/100; } /* +30% dégâts si pv < 50% */
    }
    return degats;
}

/* trie par vitesse décroissante */
static void trier_vitesse_desc(const GroupeCreatures* g, int* indices) {
    int i, j;
    for (i = 0; i < g->nb; i++)
    for (i = 0; i < g->nb - 1; i++) {
        for (j = 0; j < g->nb - 1 - i; j++) {
            int a = indices[j], b = indices[j+1];
            if (g->tab[a].vitesse < g->tab[b].vitesse) {
                int temp = indices[j];
                indices[j] = indices[j+1];
                indices[j+1] = temp;
            }
        }
    }
}

/* application stress oxygène après attaque subie */
static void appliquer_stress_oxygene(MoteurJeu* jeu) {
    int delta = 1 + (rand() % 2); /* 1 ou 2 */
    joueur_reduction_oxygene(jeu->joueur, delta);
}

/* déclenchement effets speciaux avant/pendant attaque*/
static int creature_effectue_attaque(MoteurJeu* jeu, Creature* c) {
    if (!c->en_vie || c->pv <= 0) return 0;

    int attaque_effectuees = 0;

    /* Effet spécial : paralysie (méduse) */
    if (c->type == C_MEDUSE) {
        joueur_application_paralysie_tick(jeu->joueur, 1);
        printf("[Méduse] applique Piqûre paralysante: -1 action au tour suivant\n");
    }

    /* Kraken attaques */
    int nb_frappe = (c->type == C_KRAKEN) ? 2 : 1;
    for (int k = 0; k < nb_frappe; k++) {
        if (!c->en_vie || c->pv <= 0) break;

        int degats = calcul_degats_sur_joueur(c, jeu->joueur);

        printf("[Attaque] Créature type=%d inflige %d dégâts au joueur.\n", (int)c->type, degats);
        joueur_degats_subis(jeu->joueur, degats);
        appliquer_stress_oxygene(jeu);
        attaque_effectuees++;
    }

    return attaque_effectuees;
}

int creatures_phase_attaque(MoteurJeu* jeu, GroupeCreatures* groupe) {
    if (!jeu || !groupe || !groupe->tab || groupe->nb <= 0) return 0;

    /* limitation nombre de créatures */
    int indices[32];
    int N = groupe->nb;
    if (N > 32) N = 32;

    trier_vitesse_desc(groupe, indices);

    /* boucle attaque dans ordre */
    int total_attaques = 0;
    for (int ii = 0; ii < N; ++ii) {
        Creature* c = &groupe->tab[indices[ii]];
        if (!c->en_vie || c->pv <= 0) continue;

        total_attaques += creature_effectue_attaque(jeu, c);
    }

    return total_attaques;
}