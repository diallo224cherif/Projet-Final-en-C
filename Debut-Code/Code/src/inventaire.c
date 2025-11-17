#include "inventaire.h"
#include <stdio.h>
#include <stdlib.h>

Inventaire_s* inventaire_creer(void) {
    Inventaire_s* inv = (Inventaire_s*)malloc(sizeof(Inventaire_s));
    if (!inv) return NULL;

    inv->perles             = 50;
    inv->nb_capsules_oxygene = 2;
    inv->nb_kits_soins       = 1;

    return inv;
}

void inventaire_detruire(Inventaire_s* inv) {
    if (!inv) return;
    free(inv);
}

static void inventaire_afficher(const Inventaire_s* inv) {
    if (!inv) return;

    printf("\n=== INVENTAIRE ===\n");
    printf("Perles: %d\n\n", inv->perles);

    printf("1. Capsule d'oxygene (+30 O2) x%d\n", inv->nb_capsules_oxygene);
    printf("2. Kit de soin (+30 PV) x%d\n",     inv->nb_kits_soins);
    printf("R. Retour\n");
}

/* Interaction simple : on reste dans cette fonction jusqu'à 'R' */
void inventaire_interagir(Inventaire_s* inv, Joueur* joueur) {
    if (!inv || !joueur) return;

    char buffer[16];

    while (1) {
        inventaire_afficher(inv);
        printf("\nChoix: ");
        if (scanf("%15s", buffer) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        char choix = buffer[0];

        if (choix == 'R' || choix == 'r') {
            break;
        } else if (choix == '1') {
            if (inv->nb_capsules_oxygene <= 0) {
                printf("Vous n'avez plus de capsules d'oxygene.\n");
            } else if (joueur->oxygene >= joueur->oxygene_max) {
                printf("Votre oxygene est deja au maximum.\n");
            } else {
                inv->nb_capsules_oxygene--;
                int avant = joueur->oxygene;
                joueur->oxygene += 30;
                if (joueur->oxygene > joueur->oxygene_max)
                    joueur->oxygene = joueur->oxygene_max;
                printf("Vous utilisez une capsule d'oxygene: %d -> %d\n",
                       avant, joueur->oxygene);
            }
        } else if (choix == '2') {
            if (inv->nb_kits_soins <= 0) {
                printf("Vous n'avez plus de kits de soin.\n");
            } else if (joueur->pv >= joueur->pv_max) {
                printf("Vos PV sont deja au maximum.\n");
            } else {
                inv->nb_kits_soins--;
                int avant = joueur->pv;
                joueur->pv += 30;
                if (joueur->pv > joueur->pv_max)
                    joueur->pv = joueur->pv_max;
                printf("Vous utilisez un kit de soin: %d -> %d\n",
                       avant, joueur->pv);
            }
        } else {
            printf("Choix invalide.\n");
        }
    }
}

/* Récompenses en perles + chance d'objets en fonction de la profondeur */
void distribuer_recompenses_combat(Inventaire_s* inv,
                                   int profondeur,
                                   int nb_creatures_vaincues) {
    if (!inv || nb_creatures_vaincues <= 0) return;

    int base = 10 + profondeur / 10;
    int total = base * nb_creatures_vaincues;
    inv->perles += total;

    printf("[Loot] Vous obtenez %d perles (Total: %d).\n",
           total, inv->perles);

    /* chance d'objets */
    for (int i = 0; i < nb_creatures_vaincues; ++i) {
        int r = rand() % 100;
        if (r < 20) { /* 20% chance capsule */
            inv->nb_capsules_oxygene++;
            printf("[Loot] Vous trouvez une capsule d'oxygene.\n");
        } else if (r < 30) { /* 10% chance kit de soin */
            inv->nb_kits_soins++;
            printf("[Loot] Vous trouvez un kit de soin.\n");
        }
    }
}