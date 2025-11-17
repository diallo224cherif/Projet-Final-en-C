#ifndef JOUEUR_H
#define JOUEUR_H

typedef struct Joueur {
    int pv;
    int pv_max;

    int oxygene;
    int oxygene_max;

    int fatigue;    /* 0+ */
    int paralysie;  /* nb de tours restants */

    int attaque;
    int defense_base;

    int niveau;
    int experience; /* 0..100 */

    int apnee;      /* réduit conso oxygène */
    int resistance; /* augmente défense */
    int force;      /* augmente attaque */
} Joueur;

/* Creation / destruction */
Joueur* creerJoueur(void);
void    joueur_detruire(Joueur* j);
void    joueur_reinitialise(Joueur* j);

/* Etat simple */
int joueur_mort(const Joueur* j);
int joueur_pv(const Joueur* j);
int joueur_oxygene(const Joueur* j);
int joueur_fatigue(const Joueur* j);

/* Combat / gestion stats */
void joueur_degats_subis(Joueur* j, int degats);
int  joueur_defense(const Joueur* j);
void joueur_reduction_oxygene(Joueur* j, int delta);
void joueur_application_paralysie_tick(Joueur* j, int delta);
void joueur_consommation_tick(Joueur* j);
void joueur_consommation_oxygene(Joueur* j, int profondeur);
void joueur_recuperation_fatigue(Joueur* j, int delta);

/* XP / competences */
void joueur_ajouter_experience(Joueur* j, int xp);
void joueur_debloquer_competence(Joueur* j);

#endif