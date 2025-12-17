#include <stdlib.h>
#include <math.h>
#include "Hachage.h"

long long clef(double x, double y){
    long long ix = (long long)x;
    long long iy = (long long)y;
    return iy + (ix + iy) * (ix + iy + 1) / 2;
}

int hachage(long long k, int M){
    double A = (sqrt(5.0) - 1.0) / 2.0;
    return (int)(M * (k * A - floor(k * A)));
}

Noeud * rechercheCreeNoeudHachage(Reseau *R, TableHachage *H, double x, double y){
    long long k = clef(x,y);
    int indice = hachage(k, H->tailleMax);

    CellNoeud *cour = H->T[indice];

    //Recherche
    while (cour != NULL){
        if (cour->nd->x == x && cour->nd->y == y){
            return cour->nd;
        }
        cour = cour->suiv;
    }

    //Création du noeud
    Noeud *n = (Noeud*)malloc(sizeof(Noeud));
    n->x = x;
    n->y = y;
    n->voisins = NULL;
    n->num = ++R->nbNoeuds;

    // Ajout dans le réseau
    CellNoeud *cnR = (CellNoeud*)malloc(sizeof(CellNoeud));
    cnR->nd = n;
    cnR->suiv = R->noeuds;
    R->noeuds = cnR;

    // Ajout dans la table de hachage
    CellNoeud *cnH = (CellNoeud*)malloc(sizeof(CellNoeud));
    cnH->nd = n;
    cnH->suiv = H->T[indice];
    H->T[indice] = cnH;

    return n;
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if (!C) return NULL;

    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    // Création de la table de hachage
    TableHachage *H = (TableHachage*)malloc(sizeof(TableHachage));
    H->tailleMax = M;
    H->T = (CellNoeud**)malloc(sizeof(CellNoeud*) * M);
    for (int i = 0; i < M; i++){
        H->T[i] = NULL;
    }

    // Parcours des chaines
    CellChaine *cc = C->chaines;
    while (cc != NULL){
        CellPoint *cp = cc->points;
        Noeud *prec = NULL;
        Noeud *premier = NULL;

        while (cp != NULL){
            Noeud *nd = rechercheCreeNoeudHachage(R, H, cp->x, cp->y);

            if (!premier) premier = nd;
            if (prec) {
                // Ajout des voisins si nécessaire
                CellNoeud *vois1 = (CellNoeud*)malloc(sizeof(CellNoeud));
                vois1->nd = nd;
                vois1->suiv = prec->voisins;
                prec->voisins = vois1;

                CellNoeud *vois2 = (CellNoeud*)malloc(sizeof(CellNoeud));
                vois2->nd = prec;
                vois2->suiv = nd->voisins;
                nd->voisins = vois2;
            }
            prec = nd;
            cp = cp->suiv;
        }

        // Ajouter commodité entre premier et dernier point de la chaine
        if (premier && prec && premier != prec){
            CellCommodite *k = (CellCommodite*)malloc(sizeof(CellCommodite));
            k->extrA = premier;
            k->extrB = prec;
            k->suiv = R->commodites;
            R->commodites = k;
        }

        cc = cc->suiv;
    }

    return R;
}