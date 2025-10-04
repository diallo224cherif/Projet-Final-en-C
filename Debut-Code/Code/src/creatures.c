#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "creatures.h"

void initialiser_creature(CreatureMarine *creature, int id, const char *nom, int pv_max, int att_min, int att_max, int def, int vit, const char *effet) {
    creature->id = id;
    strcpy(creature->nom, nom);
    creature->points_de_vie_max = pv_max;
    creature->points_de_vie_actuels = pv_max;
    creature->attaque_minimale = att_min;
    creature->attaque_maximale = att_max;
    creature->defense = def;
    creature->vitesse = vit;
    strcpy(creature->effet_special, effet);
    creature->est_vivant = 1;
}

void generer_creatures_zone(CreatureMarine creatures[], int *nb_creatures, int profondeur) {
    // Initialisation aléatoire
    srand(time(NULL));

    // Nombre de créatures selon la profondeur
    if (profondeur > -100) {
        *nb_creatures = rand() % 2 + 1; // 1-2 créatures
    } else if (profondeur > -200) {
        *nb_creatures = rand() % 3 + 1; // 1-3 créatures
    } else {
        *nb_creatures = rand() % 3 + 2; // 2-4 créatures
    }

    // Types de créatures disponibles
    typedef struct {
        char nom[20];
        int pv_min, pv_max;
        int att_min, att_max;
        int defense;
        int vitesse;
        char effet[20];
    } TypeCreature;

    TypeCreature types[] = {
        {"Kraken", 120, 180, 25, 40, 10, 3, "double_attaque"},
        {"Requin", 60, 100, 15, 25, 5, 8, "frenesie"},
        {"Méduse", 20, 40, 8, 15, 2, 6, "paralysie"},
        {"Poisson-Épée", 70, 90, 18, 28, 7, 7, "perforant"},
        {"Crabe Géant", 80, 120, 12, 20, 15, 4, "carapace"}
    };

    // Génération des créatures
    for (int i = 0; i < *nb_creatures; i++) {
        int type_index = rand() % 5; // 0-4
        TypeCreature type = types[type_index];

        int pv = rand() % (type.pv_max - type.pv_min + 1) + type.pv_min;
        int att_min = type.att_min;
        int att_max = type.att_max;

        initialiser_creature(&creatures[i],
                           i,
                           type.nom,
                           pv,
                           att_min,
                           att_max,
                           type.defense,
                           type.vitesse,
                           type.effet);
    }
}

void afficher_creatures(const CreatureMarine creatures[], int nb_creatures) {
    printf("\n=== CRÉATURES MARINES ===\n");

    if (nb_creatures == 0) {
        printf("Aucune créature dans la zone.\n");
        return;
    }

    for (int i = 0; i < nb_creatures; i++) {
        if (creatures[i].est_vivant) {
            printf("[%d] %s ", creatures[i].id, creatures[i].nom);
            printf("(%d/%d PV) ", creatures[i].points_de_vie_actuels, creatures[i].points_de_vie_max);
            printf("ATK:%d-%d DEF:%d VIT:%d",
                   creatures[i].attaque_minimale,
                   creatures[i].attaque_maximale,
                   creatures[i].defense,
                   creatures[i].vitesse);

            if (strcmp(creatures[i].effet_special, "aucun") != 0) {
                printf(" [%s]", creatures[i].effet_special);
            }
            printf("\n");
        }
    }
}

void attaquer_creature(CreatureMarine *creature, int degats) {
    int degats_reels = degats - creature->defense;
    if (degats_reels < 1) {
        degats_reels = 1; // Dégâts minimum garantis
    }

    creature->points_de_vie_actuels -= degats_reels;

    if (creature->points_de_vie_actuels <= 0) {
        creature->points_de_vie_actuels = 0;
        creature->est_vivant = 0;
        printf("%s a été vaincu!\n", creature->nom);
    } else {
        printf("%s perd %d PV (%d/%d restants)\n",
               creature->nom, degats_reels,
               creature->points_de_vie_actuels, creature->points_de_vie_max);
    }
}

int creature_est_morte(const CreatureMarine *creature) {
    return !creature->est_vivant;
}

int calcul_degats_creature(const CreatureMarine *creature) {
    int degats_base = rand() % (creature->attaque_maximale - creature->attaque_minimale + 1) + creature->attaque_minimale;

    // Application des effets spéciaux
    if (strcmp(creature->effet_special, "frenesie") == 0 &&
        creature->points_de_vie_actuels < creature->points_de_vie_max / 2) {
        degats_base = (int)(degats_base * 1.3); // +30% en frénésie
        printf("%s entre en frénésie! Dégâts augmentés!\n", creature->nom);
    }

    return degats_base;
}