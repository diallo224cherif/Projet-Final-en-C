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
    c.effets.carapace_active = (t == C_CRABE_GEANT) ? 1 : 0;
    c.en_vie = (pv > 0);
    return c;
}

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
            degats = (degats * 130) / 100;
        }
    }
    return degats;
}

static void appliquer_stress_oxygene(MoteurJeu* jeu) {
    if (!jeu || !jeu->joueur) return;

    int delta = 1 + (rand() % 2);
    joueur_reduction_oxygene(jeu->joueur, delta);
}

static int creature_effectue_attaque(MoteurJeu* jeu, Creature* c) {
    if (!c->en_vie || c->pv <= 0) return 0;

    int attaques_effectuees = 0;

    if (c->type == C_MEDUSE) {
        joueur_application_paralysie_tick(jeu->joueur, 1);
        printf("[Méduse] applique paralysie : -1 action prochain tour\n");
    }

    int nb_frappe = (c->type == C_KRAKEN) ? 2 : 1;

    for (int k = 0; k < nb_frappe; k++) {
        if (!c->en_vie || c->pv <= 0) break;

        int degats = calcul_degats_sur_joueur(c, jeu->joueur);
        printf("[Attaque] Créature type=%d inflige %d dégâts.\n", (int)c->type, degats);

        joueur_degats_subis(jeu->joueur, degats);
        appliquer_stress_oxygene(jeu);
        attaques_effectuees++;
    }

    return attaques_effectuees;
}

static Creature g_creatures[5];
static GroupeCreatures g_groupe = { g_creatures, 0 };

int creatures_phase_attaque(MoteurJeu* jeu, GroupeCreatures* groupe) {
    if (!jeu || !jeu->joueur || !groupe || groupe->tab <= 0) return 0;
    if (groupe->nb <= 0) return 0;

    int total_attaques = 0;

    for (int i = 0; i < groupe->nb; ++i) {
        Creature* c = &groupe->tab[i];
        if (!c->en_vie || c->pv <= 0) continue;

        total_attaques += creature_effectue_attaque(jeu, c);
    }

    return total_attaques;
}

int creatures_generation(MoteurJeu* jeu) {
    if (!jeu) return 0;

    g_groupe.nb = 1;
    g_creatures[0] = creature_creer(C_REQUIN, 40, 40, 12, 3, 5);
    g_creatures[0].en_vie = 1;

    printf("[Creatures] Génération de %d créature(s).\n", g_groupe.nb);
    return g_groupe.nb;
}

void creatures_postcombat(MoteurJeu* jeu) {
    (void)jeu;
    printf("[Creatures] Fin combat, reset du groupe.\n");
    g_groupe.nb = 0;
}

GroupeCreatures* combat_obtenir_groupe(MoteurJeu* jeu) {
    (void)jeu;
    return &g_groupe;
}
