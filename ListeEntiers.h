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

ListeEntiers* creerListe();
void ajoutTete(ListeEntiers* L, int val);
void afficherListe(ListeEntiers* L);
void libererListe(ListeEntiers* L);

#endif
