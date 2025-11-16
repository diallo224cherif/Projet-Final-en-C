#include "moteur.h"
#include "joueur.h"
#include "carte.h"
#include "inventaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern Joueur* creerJoueur(void);
extern void joueur_detruire(Joueur* j);
extern void joueur_reinitialise(Joueur* j);
extern void joueur_consommation_tick(Joueur* j);

extern Carte* carte_charger_defaut(Joueur* joueur); // ✅ Paramètre joueur
extern void carte_detruire(Carte* c);
extern void carte_reinitialiser(Carte* c);

extern int creatures_generation(MoteurJeu* moteur);
extern void creatures_postcombat(MoteurJeu* moteur);

extern int combat_resolution(MoteurJeu* moteur);
extern void inventaire_afficher(MoteurJeu* moteur);
extern EtatJeuId inventaire_interagir(MoteurJeu* moteur);

extern int sauvegarde_ecrire(MoteurJeu* moteur);
extern int sauvegarde_charger(MoteurJeu* moteur);

int demarrage_moteur(MoteurJeu* moteur) {
    if(!moteur) return 0;

    moteur->etat = ETAT_MENU;
    moteur->en_cours = 1;

    moteur->seed_aleatoire = (unsigned int)time(NULL);
    srand(moteur->seed_aleatoire);

    moteur->joueur = creerJoueur();
    moteur->carte = carte_charger_defaut(moteur->joueur); // ✅ Bonus XP intégré
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

/* Le reste du fichier reste inchangé (exploration, combat, inventaire, etc.) */