#include "ListeEntiers.h"

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