cmake_minimum_required(VERSION 4.0)
project(Projet_Final_en_C C)

set(CMAKE_C_STANDARD 11)

include_directories(Debut-Code/Code/include)

add_executable(Projet_Final_en_C
        Debut-Code/Code/include/carte.h
        Debut-Code/Code/include/combat.h
        Debut-Code/Code/include/creatures.h
        Debut-Code/Code/include/inventaire.h
        Debut-Code/Code/include/joueur.h
        Debut-Code/Code/include/moteur.h
        Debut-Code/Code/include/sauvegarde.h
        Debut-Code/Code/include/strings.h
        Debut-Code/Code/src/carte.c
        Debut-Code/Code/src/combat.c
        Debut-Code/Code/src/creatures.c
        Debut-Code/Code/src/inventaire.c
        Debut-Code/Code/src/joueur.c
        Debut-Code/Code/src/main.c
        Debut-Code/Code/src/moteur.c
        Debut-Code/Code/src/sauvegarde.c
        Debut-Code/Code/src/strings.c)
