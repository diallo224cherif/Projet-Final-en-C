#include "moteur.h"
#include "joueur.h"
#include "carte.h"
#include "inventaire.h"
#include "combat.h"
#include "creatures.h"
#include "sauvegarde.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* --- Fonctions externes --- */

extern struct Joueur* creerJoueur(void);
extern void           joueur_detruire(struct Joueur* j);
extern void           joueur_reinitialise(struct Joueur* j);
extern void           joueur_consommation_tick(struct Joueur* j);
extern int            joueur_mort(const struct Joueur* j);

extern struct Carte*  carte_charger_defaut(struct Joueur* joueur);
extern void           carte_detruire(struct Carte* c);
extern void           carte_reinitialiser(struct Carte* c);
extern void           carte_afficher(const struct Carte* c);
extern int            carte_deplacer(struct Carte* c, int dx, int dy);
extern int            carte_profondeur_actuelle(const struct Carte* c);
extern int            carte_case_sure(const struct Carte* c);

extern int            creatures_generation(MoteurJeu* moteur);
extern void           creatures_postcombat(MoteurJeu* moteur);

extern int            combat_resolution(MoteurJeu* moteur);

extern int            sauvegarde_ecrire(MoteurJeu* moteur);
extern int            sauvegarde_charger(MoteurJeu* moteur);

/* Inventaire */
extern struct Inventaire_s* inventaire_creer(void);
extern void                  inventaire_detruire(struct Inventaire_s* inv);
extern void                  inventaire_interagir(struct Inventaire_s* inv,
                                                  struct Joueur* joueur);

/* Prototypes des états */
static void etat_menu       (MoteurJeu* moteur);
static void etat_exploration(MoteurJeu* moteur);
static void etat_combat     (MoteurJeu* moteur);
static void etat_inventaire (MoteurJeu* moteur);
static void etat_sauvegarde (MoteurJeu* moteur);

/* ========================================================= */

int demarrage_moteur(MoteurJeu* moteur) {
    if (!moteur) return 0;

    moteur->etat     = ETAT_MENU;
    moteur->en_cours = 1;

    moteur->seed_aleatoire = (unsigned int)time(NULL);
    srand(moteur->seed_aleatoire);

    moteur->joueur     = creerJoueur();
    moteur->carte      = carte_charger_defaut(moteur->joueur);
    moteur->inventaire = inventaire_creer();
    moteur->profondeur = 0;

    if (!moteur->joueur || !moteur->carte || !moteur->inventaire) {
        if (moteur->carte) {
            carte_detruire(moteur->carte);
            moteur->carte = NULL;
        }
        if (moteur->joueur) {
            joueur_detruire(moteur->joueur);
            moteur->joueur = NULL;
        }
        if (moteur->inventaire) {
            inventaire_detruire(moteur->inventaire);
            moteur->inventaire = NULL;
        }
        return 0;
    }
    return 1;
}

void lancer_moteur(MoteurJeu* moteur) {
    if (!moteur) return;

    while (moteur->en_cours) {
        switch (moteur->etat) {
            case ETAT_MENU:
                etat_menu(moteur);
                break;
            case ETAT_EXPLORATION:
                etat_exploration(moteur);
                break;
            case ETAT_COMBAT:
                etat_combat(moteur);
                break;
            case ETAT_INVENTAIRE:
                etat_inventaire(moteur);
                break;
            case ETAT_SAUVEGARDE:
                etat_sauvegarde(moteur);
                break;
            case ETAT_QUITTE:
            default:
                moteur->en_cours = 0;
                break;
        }
    }
}

void stop_moteur(MoteurJeu* moteur) {
    if (!moteur) return;

    if (moteur->carte) {
        carte_detruire(moteur->carte);
        moteur->carte = NULL;
    }
    if (moteur->joueur) {
        joueur_detruire(moteur->joueur);
        moteur->joueur = NULL;
    }
    if (moteur->inventaire) {
        inventaire_detruire(moteur->inventaire);
        moteur->inventaire = NULL;
    }
    moteur->en_cours = 0;
}

/* ========================================================= */

int moteur_choix(const char* message, int min, int max) {
    int choix = 0;
    int ok = 0;

    do {
        printf("%s (%d-%d): ", message, min, max);

        if (scanf("%d", &choix) == 1 && choix >= min && choix <= max) {
            ok = 1;
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Entree invalide.\n");
        }
    } while (!ok);

    return choix;
}

/* ======================== ETAT MENU ======================= */

static void etat_menu(MoteurJeu* moteur) {
    printf("\n===OCEANDEPTH===\n");
    printf("1. Continuer\n");
    printf("2. Nouvelle partie\n");
    printf("3. Charger\n");
    printf("4. Quitter\n");

    int c = moteur_choix("Choix", 1, 4);

    if (c == 1) {
        if (!moteur->joueur || joueur_mort(moteur->joueur)) {
            printf("Aucune partie en cours a continuer.\n");
            moteur->etat = ETAT_MENU;
            return;
        }
        moteur->etat = ETAT_EXPLORATION;
    } else if (c == 2) {
        if (moteur->joueur) joueur_reinitialise(moteur->joueur);
        if (moteur->carte)  carte_reinitialiser(moteur->carte);
        moteur->profondeur = 0;
        moteur->etat = ETAT_EXPLORATION;
    } else if (c == 3) {
        if (sauvegarde_charger(moteur)) {
            printf("Sauvegarde chargee avec succes.\n");
            moteur->etat = ETAT_EXPLORATION;
        } else {
            printf("Echec du chargement de la sauvegarde.\n");
            moteur->etat = ETAT_MENU;
        }
    } else {
        moteur->etat = ETAT_QUITTE;
        moteur->en_cours = 0;
    }
}

/* ===================== ETAT EXPLORATION =================== */

static void etat_exploration(MoteurJeu* moteur) {
    if (!moteur || !moteur->carte || !moteur->joueur) return;

    moteur->profondeur = carte_profondeur_actuelle(moteur->carte);

    printf("\n=== EXPLORATION ===\n");
    carte_afficher(moteur->carte);

    printf("\nQue voulez vous faire ?\n");
    printf("1. Se deplacer\n");
    printf("2. Ouvrir l'inventaire\n");
    printf("3. Sauvegarder la partie\n");
    printf("4. Retour au menu principal\n");

    int c = moteur_choix("Choix", 1, 4);

    if (c == 1) {
        printf("Deplacement :\n");
        printf("1. Haut\n");
        printf("2. Bas\n");
        printf("3. Gauche\n");
        printf("4. Droite\n");

        int d = moteur_choix("Direction", 1, 4);
        int dx = 0, dy = 0;

        if (d == 1)      dy = -1;
        else if (d == 2) dy = 1;
        else if (d == 3) dx = -1;
        else if (d == 4) dx = 1;

        if (carte_deplacer(moteur->carte, dx, dy)) {
            moteur->profondeur = carte_profondeur_actuelle(moteur->carte);
            joueur_consommation_tick(moteur->joueur);

            if (!carte_case_sure(moteur->carte)) {
                int rencontres = creatures_generation(moteur);
                if (rencontres > 0) {
                    moteur->etat = ETAT_COMBAT;
                    return;
                }
            }
        }

        moteur->etat = ETAT_EXPLORATION;
    } else if (c == 2) {
        moteur->etat = ETAT_INVENTAIRE;
    } else if (c == 3) {
        moteur->etat = ETAT_SAUVEGARDE;
    } else {
        moteur->etat = ETAT_MENU;
    }
}

/* ======================= ETAT COMBAT ====================== */

static void etat_combat(MoteurJeu* moteur) {
    int res = combat_resolution(moteur);

    if (res > 0) {
        creatures_postcombat(moteur);
        moteur->etat = ETAT_EXPLORATION;
    } else if (res < 0) {
        printf("Vous avez perdu...\n");
        moteur->en_cours = 0;
        moteur->etat = ETAT_QUITTE;
    } else {
        /* combat continue */
    }
}

/* ===================== ETAT INVENTAIRE ==================== */

static void etat_inventaire(MoteurJeu* moteur) {
    if (!moteur || !moteur->inventaire || !moteur->joueur) {
        moteur->etat = ETAT_EXPLORATION;
        return;
    }

    inventaire_interagir(moteur->inventaire, moteur->joueur);
    moteur->etat = ETAT_EXPLORATION;
}

/* ===================== ETAT SAUVEGARDE ==================== */

static void etat_sauvegarde(MoteurJeu* moteur) {
    if (sauvegarde_ecrire(moteur)) {
        printf("Sauvegarde reussie.\n");
    } else {
        printf("Echec de la sauvegarde.\n");
    }
    moteur->etat = ETAT_EXPLORATION;
}