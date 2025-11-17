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
    ETAT_QUITTE,
} EtatJeuId;

/* Forward declarations uniquement, pas de typedef ici */
struct Joueur;
struct Carte;
struct Inventaire_s;

typedef struct MoteurJeu {
    EtatJeuId etat;
    int en_cours;

    struct Joueur* joueur;
    struct Carte* carte;
    struct Inventaire_s* inventaire;

    int profondeur;
    unsigned int seed_aleatoire;
} MoteurJeu;

/* API du moteur */
int  demarrage_moteur(MoteurJeu* moteur);
void lancer_moteur(MoteurJeu* moteur);
void stop_moteur(MoteurJeu* moteur);

/* Lecture du choix utilisateur */
int moteur_choix(const char* message, int min, int max);

#endif