#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "carte.h"
#include "joueur.h"
#include "creatures.h"


void initialiser_carte(Carte *carte) {
    // Initialisation de toutes les zones comme inconnues
    for (int i = 0; i < TAILLE_CARTE; i++) {
        for (int j = 0; j < TAILLE_CARTE; j++) {
            carte->zones[i][j].x = i;
            carte->zones[i][j].y = j;
            carte->zones[i][j].type = INCONNU;
            carte->zones[i][j].est_decouverte = 0;
            carte->zones[i][j].est_accessible = 0;
            strcpy(carte->zones[i][j].nom, "Zone Inconnue");
            carte->zones[i][j].profondeur = 0;
            carte->zones[i][j].nb_creatures = 0;
            carte->zones[i][j].a_tresor = 0;
        }
    }

    // Configuration initiale de la carte (exemple)
    // Surface
    carte->zones[0][0].type = SURFACE;
    carte->zones[0][0].est_decouverte = 1;
    carte->zones[0][0].est_accessible = 1;
    strcpy(carte->zones[0][0].nom, "Base Surface");
    carte->zones[0][0].profondeur = 0;

    // Récifs
    carte->zones[0][1].type = RECIF;
    carte->zones[0][1].est_decouverte = 1;
    strcpy(carte->zones[0][1].nom, "Recif Corallien");
    carte->zones[0][1].profondeur = -50;
    carte->zones[0][1].nb_creatures = 3;

    // Épave
    carte->zones[0][2].type = EPAVE;
    strcpy(carte->zones[0][2].nom, "Epave Mysterieuse");
    carte->zones[0][2].profondeur = -80;
    carte->zones[0][2].a_tresor = 1;

    // Position initiale du joueur
    carte->position_x = 0;
    carte->position_y = 0;
    carte->profondeur_max = 0;
}

void afficher_carte(const Carte *carte) {
    printf("\n=== CARTOGRAPHIE OCÉANIQUE ===\n");
    printf("Position actuelle: [X] | Légende: [S]urface [R]écif [E]pave [?]Inconnu\n\n");

    for (int i = 0; i < TAILLE_CARTE; i++) {
        for (int j = 0; j < TAILLE_CARTE; j++) {
            if (i == carte->position_x && j == carte->position_y) {
                printf("[X] ");
            } else if (carte->zones[i][j].est_decouverte) {
                switch(carte->zones[i][j].type) {
                    case SURFACE: printf("[S] "); break;
                    case RECIF: printf("[R] "); break;
                    case EPAVE: printf("[E] "); break;
                    case ALGUES: printf("[A] "); break;
                    case GROTTE: printf("[G] "); break;
                    case FOSSE: printf("[F] "); break;
                    default: printf("[?] "); break;
                }
            } else {
                printf("[?] ");
            }
        }
        printf("\n");
    }
}

void afficher_zone_actuelle(const Carte *carte) {
    Zone *zone_actuelle = &carte->zones[carte->position_x][carte->position_y];

    printf("\n=== ZONE ACTUELLE ===\n");
    printf("Nom: %s\n", zone_actuelle->nom);
    printf("Profondeur: %dm\n", zone_actuelle->profondeur);
    printf("Type: ");

    switch(zone_actuelle->type) {
        case SURFACE:
            printf("Surface (Sécurisée)\n");
            printf("-> Sauvegarde possible\n-> Boutique disponible\n");
            break;
        case RECIF:
            printf("Récif Corallien\n");
            printf("-> Créatures: %d\n", zone_actuelle->nb_creatures);
            printf("-> Visibilité: Bonne\n");
            break;
        case EPAVE:
            printf("Épave Mystérieuse\n");
            if (zone_actuelle->a_tresor) {
                printf("-> TRÉSOR DÉTECTÉ!\n");
            }
            printf("-> Créatures: %d\n", zone_actuelle->nb_creatures);
            break;
        case ALGUES:
            printf("Forêt d'Algues\n");
            printf("-> Labyrinthe naturel\n-> Créatures camouflées\n");
            break;
        case GROTTE:
            printf("Grotte Sous-Marine\n");
            printf("-> Sécurisée\n-> Cristaux d'oxygène\n");
            break;
        case FOSSE:
            printf("Fosse Abyssale\n");
            printf("-> Danger extrême\n-> Créatures légendaires\n");
            break;
        default:
            printf("Zone Inconnue\n");
            break;
    }
}

int peut_se_deplacer(const Carte *carte, int direction) {
    int new_x = carte->position_x;
    int new_y = carte->position_y;

    switch(direction) {
        case DIR_HAUT: new_x--; break;
        case DIR_BAS: new_x++; break;
        case DIR_GAUCHE: new_y--; break;
        case DIR_DROITE: new_y++; break;
    }

    // Vérification des limites
    if (new_x < 0 || new_x >= TAILLE_CARTE || new_y < 0 || new_y >= TAILLE_CARTE) {
        return 0;
    }

    // Vérification si la zone est accessible
    if (!carte->zones[new_x][new_y].est_accessible &&
        !carte->zones[new_x][new_y].est_decouverte) {
        return 0;
    }

    return 1;
}

int deplacer_joueur(Carte *carte, int direction) {
    if (!peut_se_deplacer(carte, direction)) {
        printf("Déplacement impossible dans cette direction!\n");
        return 0;
    }

    int new_x = carte->position_x;
    int new_y = carte->position_y;

    switch(direction) {
        case DIR_HAUT: new_x--; break;
        case DIR_BAS: new_x++; break;
        case DIR_GAUCHE: new_y--; break;
        case DIR_DROITE: new_y++; break;
    }

    // Mise à jour de la position
    carte->position_x = new_x;
    carte->position_y = new_y;

    // Découverte automatique de la zone
    carte->zones[new_x][new_y].est_decouverte = 1;

    // Mise à jour de la profondeur max
    int profondeur_actuelle = abs(carte->zones[new_x][new_y].profondeur);
    if (profondeur_actuelle > carte->profondeur_max) {
        carte->profondeur_max = profondeur_actuelle;
    }

    printf("Déplacement réussi vers ");
    switch(carte->zones[new_x][new_y].type) {
        case SURFACE: printf("la Surface"); break;
        case RECIF: printf("le Récif Corallien"); break;
        case EPAVE: printf("l'Épave Mystérieuse"); break;
        default: printf("la zone inconnue"); break;
    }
    printf("!\n");

    return 1;
}

void explorer_zone(Carte *carte) {
    Zone *zone = &carte->zones[carte->position_x][carte->position_y];

    printf("\n=== EXPLORATION DE LA ZONE ===\n");

    if (zone->type == INCONNU) {
        printf("Vous découvrez une nouvelle zone!\n");
        zone->est_decouverte = 1;

        // Génération aléatoire simple pour le prototype
        TypeZone types[] = {RECIF, EPAVE, ALGUES, GROTTE};
        zone->type = types[rand() % 4];
        zone->nb_creatures = rand() % 5 + 1;
        zone->a_tresor = (rand() % 100) < 30; // 30% de chance de trésor

        switch(zone->type) {
            case RECIF:
                strcpy(zone->nom, "Récif Corallien");
                zone->profondeur = -(rand() % 50 + 30);
                break;
            case EPAVE:
                strcpy(zone->nom, "Épave Ancienne");
                zone->profondeur = -(rand() % 100 + 50);
                break;
            case ALGUES:
                strcpy(zone->nom, "Forêt d'Algues");
                zone->profondeur = -(rand() % 80 + 40);
                break;
            case GROTTE:
                strcpy(zone->nom, "Grotte Sous-Marine");
                zone->profondeur = -(rand() % 120 + 60);
                break;
        }
    }

    afficher_zone_actuelle(carte);
}
// Dans src/carte.c - ajoute à la fin
int zone_contient_creatures(const Carte *carte, int x, int y, const CreatureMarine creatures[], int nb_creatures) {
    // Pour l'instant, on suppose que les créatures sont toujours dans la zone actuelle
    return (x == carte->position_x && y == carte->position_y && nb_creatures > 0);
}

void afficher_carte_avancee(const Carte *carte, const Plongeur *joueur, const CreatureMarine creatures[], int nb_creatures) {
    printf("\n=== CARTOGRAPHIE OCÉANIQUE AVANCÉE ===\n");
    printf("Position: (%d,%d) | Vie: %d/%d | Oxygène: %d/%d | Perles: %d\n",
           carte->position_x, carte->position_y,
           joueur->points_de_vie, joueur->points_de_vie_max,
           joueur->niveau_oxygene, joueur->niveau_oxygene_max,
           joueur->perles);

    printf("Légende: [X]Toi [M]Monstres [S]urface [R]écif [E]pave [?]Inconnu\n\n");

    for (int i = 0; i < TAILLE_CARTE; i++) {
        for (int j = 0; j < TAILLE_CARTE; j++) {
            if (i == carte->position_x && j == carte->position_y) {
                if (nb_creatures > 0) {
                    printf("[M] "); // Position avec monstres
                } else {
                    printf("[X] "); // Position sans monstres
                }
            } else if (carte->zones[i][j].est_decouverte) {
                switch(carte->zones[i][j].type) {
                    case SURFACE: printf("[S] "); break;
                    case RECIF: printf("[R] "); break;
                    case EPAVE: printf("[E] "); break;
                    case ALGUES: printf("[A] "); break;
                    case GROTTE: printf("[G] "); break;
                    case FOSSE: printf("[F] "); break;
                    default: printf("[?] "); break;
                }
            } else {
                printf("[?] ");
            }
        }
        printf("\n");
    }

    // Informations sur les créatures présentes
    if (nb_creatures > 0) {
        printf("\n💀 MONSTRES PRÉSENTS: ");
        for (int i = 0; i < nb_creatures; i++) {
            if (creatures[i].est_vivant) {
                printf("%s(%dPV) ", creatures[i].nom, creatures[i].points_de_vie_actuels);
            }
        }
        printf("\n");
    }
}