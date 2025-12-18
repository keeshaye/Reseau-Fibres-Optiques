#ifndef __GRAPHE_H__
#define __GRAPHE_H__

#include <stdlib.h>
#include <stdio.h>
#include "Reseau.h"
#include "ListeEntiers.h"

/* Structure représentant une arête entre deux sommets u et v */
typedef struct {
    int u, v;  /* Numéros des sommets extrémités */
} Arete;

/* Liste chaînée d'arêtes */
typedef struct cellule_arete {
    Arete *a;                   /* Pointeur sur l'arête */
    struct cellule_arete *suiv; /* Suivant dans la liste */
} Cellule_arete;

/* Structure représentant un sommet */
typedef struct {
    int num;                  /* Numéro du sommet (index dans T_som) */
    double x, y;              /* Coordonnées */
    Cellule_arete *L_voisin;  /* Liste chaînée des arêtes voisines */
} Sommet;

/* Structure représentant une commodité (paire de sommets à relier) */
typedef struct {
    int e1, e2; /* Numéros des extrémités de la commodité */
} Commod;

/* Structure représentant le graphe complet */
typedef struct {
    int nbsom;        /* Nombre de sommets */
    Sommet **T_som;   /* Tableau de pointeurs sur sommets */
    int gamma;        /* Paramètre gamma (copié depuis le réseau) */
    int nbcommod;     /* Nombre de commodités */
    Commod *T_commod; /* Tableau des commodités */
} Graphe;

/* Fonction qui crée un graphe à partir d'un réseau */
Graphe* creerGraphe(Reseau* R);
ListeEntiers* BFS_Chemin(Graphe* G, int u, int v);
int reorganiseReseau(Reseau* r);

#endif
