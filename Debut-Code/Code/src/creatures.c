#include "creatures.h"
#include "moteur.h"
#include "joueur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void joueur_degats_subis(Joueur* j, int degats);
extern int joueur_defense(const Joueur* j);
extern void joueur_reduction_oxygene(Joueur* j, int delta);
extern void joueur_application_paralysie_tick(Joueur* j, int delta);

static int clamp_min(int x, int mn){return (x < mn) ? mn : x;}

Creature creature_creer(TypeCreature t, int pv, int pv_max, int att, int def, int vitesse) {
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

    if (!g || !g->tab || g->nb <= 0) return;

    for (i = 0; i < g->nb; i++)
        for (i = 0; i < g->nb - 1; i++) {
            for (j = 0; j < g->nb - 1 - i; j++) {
                int a = indices[j];
                int b = indices[j+1];

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
    if (!jeu || jeu->joueur) return;

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

static Creature g_creatures[5]; /* liste des créatures possibles */
static GroupeCreatures g_groupe = { g_creatures, 0};

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

int creatures_generation(MoteurJeu* jeu) {
    (void)jeu;

    g_groupe.nb = 1;
    g_creatures[0] = creature_creer(
        C_REQUIN,
        40,
        40,
        12,
        3,
        5
        );

    g_creatures[0].en_vie = 1;

    printf("[Creatures] Génération de %d créature(s).\n", g_groupe.nb);
    return g_groupe.nb;
}

void creatures_postcombat(MoteurJeu* jeu) {
    (void)jeu;
    printf("[Creatures] Fin de combat, Groupe réinitialisé.\n");
    g_groupe.nb = 0;
}

GroupeCreatures* combat_obtenir_groupe(MoteurJeu* jeu) {
    (void)jeu;
    return &g_groupe;
}