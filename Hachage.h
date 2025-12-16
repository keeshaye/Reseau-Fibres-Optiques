#ifndef _HACHAGE_H_
#define _HACHAGE_H_

#include "Reseau.h"

typedef struct{
  int tailleMax; // Nombre de cases de la table (M)
  CellNoeud** T; // Tableau de listes chainées de noeuds
} TableHachage ;

Reseau* reconstitueReseauHachage(Chaines *C, int M);
Noeud * rechercheCreeNoeudHachage(Reseau *R, TableHachage *H, double x, double y);

#endif