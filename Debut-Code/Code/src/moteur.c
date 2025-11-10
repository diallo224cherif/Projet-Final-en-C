//
// Created by brouc on 22/10/2025.
//
#include "moteur.h"
#include "joueur.h"
#include "carte.h"
#include "inventaire.h"
#include <stdio.h>
#include <stdlib.h>

extern Joueur* creerJoueur(void);
extern void joueur_detruire(Joueur* j);
extern void joueur_reinitialise(Joueur* j);
extern void joueur_consommation_tick(Joueur* j);

extern Carte* carte_charger_defaut(void);
extern void carte_detruire(Carte* c);
extern void carte_reinitialiser(Carte* c);

extern int creatures_generation(MoteurJeu* moteur);
extern void creatures_postcombat(MoteurJeu* moteur);

extern int combat_resolution(MoteurJeu* moteur);
extern void inventaire_afficher(MoteurJeu* moteur);
extern EtatJeuId inventaire_interagir(MoteurJeu* moteur);

extern int sauvegarde_ecrire(MoteurJeu* moteur);
extern int sauvegarde_charger(MoteurJeu* moteur);

/* Etats du jeu */

static void etat_menu (MoteurJeu* moteur);
static void etat_exploration(MoteurJeu* moteur);
static void etat_combat(MoteurJeu* moteur);
static void etat_inventaire(MoteurJeu* moteur);
static void etat_sauvegarde(MoteurJeu* moteur);


/* API du moteur pour gérer les états du jeu */

int demarrage_moteur(MoteurJeu* moteur) {
	if(!moteur) return 0;

	moteur->etat = ETAT_MENU;
	moteur->en_cours = 1;

	/* Initialiser la RNG pour la seed */
	moteur->seed_aleatoire = (unsigned int)time(NULL);
	srand(moteur->seed_aleatoire);

	/* Création sous systèmes du moteur */
	moteur->joueur = creerJoueur();
	moteur->carte = carte_charger_defaut();
	moteur->profondeur = 0;

	if(!moteur->joueur || !moteur->carte){
	if(moteur->carte) {
		carte_detruire(moteur->carte);
		moteur->carte = NULL;
	}
	if(moteur->joueur) {
		joueur_detruire(moteur->joueur);
		moteur->joueur = NULL;
	}
	return 0;
	}
	return 1;
}

void lancer_moteur(MoteurJeu* moteur) {
	if(!moteur) return;

	while(moteur->en_cours) {
	switch(moteur->etat){
		case ETAT_MENU: etat_menu(moteur);
			break;
		case ETAT_EXPLORATION: etat_exploration(moteur);
			break;
		case ETAT_COMBAT: etat_combat(moteur);
			break;
		case ETAT_INVENTAIRE: etat_inventaire(moteur);
			break;
		case ETAT_SAUVEGARDE: etat_sauvegarde(moteur);
			break;
		case ETAT_QUITTE:
		default:
			moteur->en_cours = 0;
			break;
		}
	}
}

void stop_moteur(MoteurJeu* moteur) {
	if(!moteur) return;

	if(moteur->carte) {
		carte_detruire(moteur->carte);
		moteur->carte = NULL;
	}
	if(moteur->joueur) {
		joueur_detruire(moteur->joueur);
		moteur->joueur = NULL;
	}

	moteur->en_cours = 0;
}

/* Lecture du choix utilisateur borné */

int moteur_choix(const char* message, int min, int max) {
	int choix = 0;
	int ok = 0;

	do{
		printf("%s (%d-%d): ", message, min, max);
		if(scanf("%d", &choix) == 1 && choix >= min && choix <= max) {
			ok = 1;
		} else{
			int c;
			while((c = getchar()) != '\n' && c != EOF){}
			printf("Entrée invalide.\n");
		}
	} while(!ok);
	return choix;
}


/* Implémentation des états */

static void etat_menu(MoteurJeu* moteur){
	printf("\n===OCEANDEPTH===\n");
	printf("1. Continuer\n");
	printf("2. Nouvelle partie\n");
	printf("3. Charger\n");
	printf("4. Quitter\n");

	int c = moteur_choix("Choix", 1, 4);
	if (c == 1 || c == 2) {
		if (c==2) {
			if (moteur->joueur) joueur_reinitialise(moteur->joueur);
			if (moteur->carte) carte_reinitialiser(moteur->carte);
			moteur->profondeur = 0;
		}
		moteur->etat = ETAT_EXPLORATION;
	} else if (c == 3) {
		if (sauvegarde_charger(moteur)) {
			moteur->etat = ETAT_EXPLORATION;
		} else {
			printf("Échec du chargement de la sauvegarde.\n");
			moteur->etat = ETAT_MENU;
		}
	} else {
		moteur->etat = ETAT_QUITTE;
	}
}

static void etat_exploration(MoteurJeu* moteur) {
	int rencontres = creatures_generation(moteur);

	if (rencontres > 0) {
		moteur->etat = ETAT_COMBAT;
		return;
	}

	printf("\nExploration (profondeur=%d)\n", moteur->profondeur);
	printf("1. Descendre\n");
	printf("2. Ouvrir l'inventaire\n");
	printf("3. Sauvegarder\n");
	printf("4. Retour au menu principal\n");

	int c = moteur_choix("Choix", 1, 4);
	if (c == 1) {
		moteur->profondeur++;
		if (moteur->joueur) joueur_consommation_tick(moteur->joueur); /* consommation oxygène et énergie du joueur*/
	} else if (c == 2) {
		moteur->etat = ETAT_INVENTAIRE;
	} else if (c == 3) {
		moteur->etat = ETAT_SAUVEGARDE;
	} else {
		moteur->etat = ETAT_MENU;
	}
}

static void etat_combat(MoteurJeu* moteur){
	int res = combat_resolution(moteur); /* Résolution du combat */

	if (res>0){
		creatures_postcombat(moteur); /* Gestion post-combat */
		moteur->etat = ETAT_EXPLORATION;
	} else if (res < 0) {
		printf("Vous avez perdu...\n");
		moteur->etat = ETAT_MENU;
	} else {
		/* Combat non terminé, rester en combat */
	}
}

static void etat_inventaire(MoteurJeu* moteur){
	inventaire_afficher(moteur); /* Affichage de l'inventaire */

	EtatJeuId next = inventaire_interagir(moteur); /* Interaction avec l'inventaire */
	moteur->etat = next;
}

static void etat_sauvegarde(MoteurJeu* moteur){
	if (sauvegarde_ecrire(moteur)){
		printf("Sauvegarde réussie.\n");
	} else {
		printf("Échec de la sauvegarde.\n");
	}
	moteur->etat = ETAT_EXPLORATION;
}