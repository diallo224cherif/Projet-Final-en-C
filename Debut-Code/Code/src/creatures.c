#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "creatures.h"

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