#include <stdio.h>
#include <stdlib.h>
#include "carte.h"
#include "joueur.h"
#include "creatures.h"

void afficher_menu_principal() {
    printf("\n=== OCEAN DEPTH - MENU PRINCIPAL ===\n");
    printf("1 - Afficher la carte simple\n");
    printf("2 - Afficher carte avancée (avec monstres)\n");
    printf("3 - Afficher zone actuelle\n");
    printf("4 - Afficher état du plongeur\n");
    printf("5 - Afficher créatures de la zone\n");
    printf("6 - Se déplacer (Haut)\n");
    printf("7 - Se déplacer (Bas)\n");
    printf("8 - Se déplacer (Gauche)\n");
    printf("9 - Se déplacer (Droite)\n");
    printf("10 - Explorer zone actuelle\n");
    printf("11 - Quitter\n");
    printf("Choix : ");
}
int main() {
    Carte carte;
    Plongeur joueur;
    CreatureMarine creatures[MAX_CREATURES];
    int nb_creatures = 0;
    int choix = 0;

    printf("Initialisation de la carte...\n");
    initialiser_carte(&carte);

    printf("Initialisation du plongeur...\n");
    initialiser_plongeur(&joueur);

    printf("Génération des créatures...\n");
    generer_creatures_zone(creatures, &nb_creatures, -50);

    printf("Monde initialisé avec succès!\n");

    do {
        afficher_menu_principal();
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                afficher_carte(&carte);
                break;

            case 2:
                afficher_carte_avancee(&carte, &joueur, creatures, nb_creatures);
                break;

            case 3:
                afficher_zone_actuelle(&carte);
                break;

            case 4:
                afficher_etat_plongeur(&joueur);
                break;

            case 5:
                afficher_creatures(creatures, nb_creatures);
                break;

            case 6:
                if (deplacer_joueur(&carte, DIR_HAUT)) {
                    printf("Nouvelle position : (%d, %d)\n", carte.position_x, carte.position_y);
                    generer_creatures_zone(creatures, &nb_creatures,
                                         carte.zones[carte.position_x][carte.position_y].profondeur);
                }
                break;

            case 7:
                if (deplacer_joueur(&carte, DIR_BAS)) {
                    printf("Nouvelle position : (%d, %d)\n", carte.position_x, carte.position_y);
                    generer_creatures_zone(creatures, &nb_creatures,
                                         carte.zones[carte.position_x][carte.position_y].profondeur);
                }
                break;

            case 8:
                if (deplacer_joueur(&carte, DIR_GAUCHE)) {
                    printf("Nouvelle position : (%d, %d)\n", carte.position_x, carte.position_y);
                    generer_creatures_zone(creatures, &nb_creatures,
                                         carte.zones[carte.position_x][carte.position_y].profondeur);
                }
                break;

            case 9:
                if (deplacer_joueur(&carte, DIR_DROITE)) {
                    printf("Nouvelle position : (%d, %d)\n", carte.position_x, carte.position_y);
                    generer_creatures_zone(creatures, &nb_creatures,
                                         carte.zones[carte.position_x][carte.position_y].profondeur);
                }
                break;

            case 10:
                explorer_zone(&carte);
                break;

            case 11:
                printf("Au revoir!\n");
                break;

            default:
                printf("Choix invalide!\n");
                break;
        }

        // Attente
        printf("\nAppuyez sur Entrée pour continuer...");
        getchar();
        getchar();

    } while (choix != 11);

    return 0;
}