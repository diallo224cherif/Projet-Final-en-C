//
// Created by brouc on 22/10/2025.
//
#ifndef MOTEUR_H
#define MOTEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    ETAT_MENU = 0,
    ETAT_EXPLORATION,
    ETAT_COMBAT,
    ETAT_INVENTAIRE,
    ETAT_SAUVEGARDE,
    ETAT_QUITTE
} EtatJeuId;

typedef struct Joueur Joueur;
typedef struct Carte Carte;

typedef struct MoteurJeu {
    EtatJeuId etat;
    int en_cours;

    Joueur* joueur;
    Carte* carte;
    int profondeur;

    unsigned int seed_aleatoire;
} MoteurJeu;

int demarrage_moteur(MoteurJeu* moteur);
void lancer_moteur(MoteurJeu* moteur);
void stop_moteur(MoteurJeu* moteur);

int moteur_choix(const char* message, int min, int max);


#endif