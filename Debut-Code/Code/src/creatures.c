#include "creatures.h"
#include "moteur.h"
#include "joueur.h"
#include "carte.h"
#include <stdio.h>
#include <stdlib.h>

/* Fonctions joueur utilisées ici */
extern void joueur_degats_subis(Joueur* j, int degats);
extern int  joueur_defense(const Joueur* j);
extern void joueur_reduction_oxygene(Joueur* j, int delta);
extern void joueur_application_paralysie_tick(Joueur* j, int delta);

/* Noms des créatures */
static const char* NOMS_CREATURES[C_NB_TYPES] = {
    "Kraken",
    "Meduse",
    "Requin",
    "Poisson epee",
    "Crabe geant"
};

const char* creature_nom(TypeCreature t) {
    if (t < 0 || t >= C_NB_TYPES) return "Inconnue";
    return NOMS_CREATURES[t];
}

Creature creature_creer(TypeCreature t,
                        int pv, int pv_max,
                        int att, int def, int vitesse) {
    Creature c;
    c.type    = t;
    c.pv      = pv;
    c.pv_max  = pv_max;
    c.att     = att;
    c.def     = def;
    c.vitesse = vitesse;
    c.effets.carapace_active = (t == C_CRABE_GEANT) ? 1 : 0;
    c.en_vie  = (pv > 0);
    return c;
}

/* ==================== logiques d'attaque ================== */

static int calcul_degats_sur_joueur(const Creature* c, const Joueur* j) {
    int def_j = joueur_defense(j);
    int att_c = c->att;

    if (c->type == C_POISSON_EPEE) {
        def_j -= 2;
        if (def_j < 0) def_j = 0;
    }

    int degats = att_c - def_j;
    if (degats < 1) degats = 1;

    if (c->type == C_REQUIN && c->pv > 0) {
        if (c->pv * 2 < c->pv_max) {
            degats = (degats * 130) / 100; /* 30% bonus quand low HP */
        }
    }

    return degats;
}

static void appliquer_stress_oxygene(struct MoteurJeu* jeu) {
    if (!jeu || !jeu->joueur) return;

    int delta = 1 + (rand() % 2); /* 1 ou 2 */
    joueur_reduction_oxygene(jeu->joueur, delta);
}

static int creature_effectue_attaque(struct MoteurJeu* jeu, Creature* c) {
    if (!jeu || !jeu->joueur || !c) return 0;
    if (!c->en_vie || c->pv <= 0) return 0;

    int attaques_effectuees = 0;

    if (c->type == C_MEDUSE) {
        joueur_application_paralysie_tick(jeu->joueur, 1);
        printf("[Meduse] applique paralysie : -1 action prochain tour\n");
    }

    int nb_frappe = (c->type == C_KRAKEN) ? 2 : 1;

    for (int k = 0; k < nb_frappe; k++) {
        if (!c->en_vie || c->pv <= 0) break;

        printf("[Combat] %s attaque !\n", creature_nom(c->type));

        int degats = calcul_degats_sur_joueur(c, jeu->joueur);
        printf("[Attaque] %s inflige %d degats.\n", creature_nom(c->type), degats);

        joueur_degats_subis(jeu->joueur, degats);
        appliquer_stress_oxygene(jeu);
        attaques_effectuees++;
    }

    return attaques_effectuees;
}

/* ==================== stockage global ===================== */

static Creature g_creatures[5];
static GroupeCreatures g_groupe = { g_creatures, 0 };

/* ================== phase d'attaque ======================= */

int creatures_phase_attaque(struct MoteurJeu* jeu, GroupeCreatures* groupe) {
    if (!jeu || !jeu->joueur || !groupe || !groupe->tab) return 0;
    if (groupe->nb <= 0) return 0;

    int total_attaques = 0;

    for (int i = 0; i < groupe->nb; ++i) {
        Creature* c = &groupe->tab[i];
        if (!c->en_vie || c->pv <= 0) continue;

        total_attaques += creature_effectue_attaque(jeu, c);
    }

    return total_attaques;
}

/* ================== generation de creatures =============== */

int creatures_generation(struct MoteurJeu* jeu) {
    if (!jeu || !jeu->carte) return 0;

    int nb_max = carte_nb_ennemis_max(jeu->carte);
    if (nb_max <= 0) {
        g_groupe.nb = 0;
        return 0;
    }

    int nb = 1 + rand() % nb_max;
    if (nb > 5) nb = 5;

    g_groupe.nb = nb;

    int profondeur = jeu->profondeur;

    for (int i = 0; i < nb; ++i) {
        TypeCreature t;

        if (profondeur < 50) {
            /* proche de la surface : meduse / requin */
            t = (rand() % 2) ? C_MEDUSE : C_REQUIN;
        } else if (profondeur < 150) {
            int r = rand() % 3;
            t = (r == 0) ? C_REQUIN : (r == 1 ? C_MEDUSE : C_POISSON_EPEE);
        } else {
            int r = rand() % 4;
            if (r == 0) t = C_KRAKEN;
            else if (r == 1) t = C_CRABE_GEANT;
            else if (r == 2) t = C_POISSON_EPEE;
            else t = C_REQUIN;
        }

        int pv     = 40;
        int att    = 12;
        int def    = 3;
        int vitesse= 5;

        if (t == C_KRAKEN) {
            pv = 80; att = 18; def = 5; vitesse = 3;
        } else if (t == C_MEDUSE) {
            pv = 40; att = 10; def = 2; vitesse = 6;
        } else if (t == C_CRABE_GEANT) {
            pv = 60; att = 14; def = 7; vitesse = 2;
        } else if (t == C_POISSON_EPEE) {
            pv = 35; att = 13; def = 2; vitesse = 7;
        }

        g_creatures[i] = creature_creer(t, pv, pv, att, def, vitesse);
        g_creatures[i].en_vie = 1;
    }

    printf("\nVous tombez sur %d creature(s) !\n", g_groupe.nb);
    for (int i = 0; i < g_groupe.nb; ++i) {
        Creature* c = &g_creatures[i];
        printf(" - [%d] %s (PV %d/%d, Att %d, Def %d, Vit %d)\n",
               i + 1, creature_nom(c->type),
               c->pv, c->pv_max,
               c->att, c->def, c->vitesse);
    }

    return g_groupe.nb;
}

void creatures_postcombat(struct MoteurJeu* jeu) {
    (void)jeu;
    printf("[Creatures] Fin combat, reset du groupe.\n");
    g_groupe.nb = 0;
}

GroupeCreatures* combat_obtenir_groupe(struct MoteurJeu* jeu) {
    (void)jeu;
    return &g_groupe;
}