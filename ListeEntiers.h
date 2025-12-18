#ifndef _LISTE_ENTIERS_H_
#define _LISTE_ENTIERS_H_

#include <stdlib.h>
#include <stdio.h>

/* Structure pour un élément de la liste */
typedef struct celluleEntier {
    int val;                    /* Valeur stockée */
    struct celluleEntier *suiv; /* Pointeur vers l’élément suivant */
} CelluleEntier;

/* Liste d’entiers */
typedef struct {
    CelluleEntier *tete;        /* Pointeur vers le premier élément */
} ListeEntiers;

/* Création d’une liste vide */
ListeEntiers* creerListe() {
    ListeEntiers* L = (ListeEntiers*) malloc(sizeof(ListeEntiers));
    if (!L) return NULL;
    L->tete = NULL;
    return L;
}

/* Ajout d’un entier en tête */
void ajoutTete(ListeEntiers* L, int val) {
    if (!L) return;
    CelluleEntier* ce = (CelluleEntier*) malloc(sizeof(CelluleEntier));
    if (!ce) return;
    ce->val = val;
    ce->suiv = L->tete;
    L->tete = ce;
}

/* Affichage de la liste */
void afficherListe(ListeEntiers* L) {
    if (!L) return;
    CelluleEntier* c = L->tete;
    while (c) {
        printf("%d ", c->val);
        c = c->suiv;
    }
    printf("\n");
}

/* Libération mémoire */
void libererListe(ListeEntiers* L) {
    if (!L) return;
    CelluleEntier* c = L->tete;
    while (c) {
        CelluleEntier* tmp = c;
        c = c->suiv;
        free(tmp);
    }
    free(L);
}

#endif
