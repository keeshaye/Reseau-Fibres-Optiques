#include <stdlib.h>
#include "ArbreQuat.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){

    *xmin = *ymin = 1e9;
    *xmax = *ymax = -1e9;

    CellChaine *c = C->chaines;
    while (c){
        CellPoint *p = c->points;
        while (p){
            if (p->x < *xmin) *xmin = p->x;
            if (p->y < *ymin) *ymin = p->y;
            if (p->x > *xmax) *xmax = p->x;
            if (p->y > *ymax) *ymax = p->y;
            p = p->suiv; 
        }
        c = c->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    ArbreQuat *aq = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    if (aq == NULL) return NULL;

    aq->xc = xc;
    aq->yc = yc;
    aq->coteX = coteX;
    aq->coteY = coteY;
    aq->noeud = NULL;
    aq->se = NULL;
    aq->so = NULL;
    aq->ne = NULL;
    aq->no = NULL;

    return aq;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    // Arbre vide
    if (*a == NULL){
        double xc = parent->xc;
        double yc = parent->yc;
        double coteX = parent->coteX / 2.0;
        double coteY = parent->coteY / 2.0;

        if (n->x < parent->xc && n->y < parent->yc){
            *a = creerArbreQuat(xc - coteX/2, yc - coteY/2, coteX, coteY);
        } else if (n->x >= parent->xc && n->y < parent->yc){
            *a = creerArbreQuat(xc + coteX/2, yc - coteY/2, coteX, coteY);
        } else if (n->x < parent->xc && n->y >= parent->yc){
            *a = creerArbreQuat(xc - coteX/2, yc + coteY/2, coteX, coteY);
        } else {
            *a = creerArbreQuat(xc + coteX/2, yc + coteY/2, coteX, coteY);
        }

        (*a)->noeud = n;
        return ;
    }

    //feuille
    if ((*a)->noeud != NULL){
        Noeud *ancien = (*a)->noeud;
        (*a)->noeud = NULL;

        insererNoeudArbre(ancien, a, *a);
        insererNoeudArbre(n, a, *a);
        return;
    }

    // Cellule interne
    if (n->x < (*a)->xc && n->y < (*a)->yc){
        insererNoeudArbre(n, &((*a)->so), *a);
    } else if (n->x >= (*a)->xc && n->y < (*a)->yc){
        insererNoeudArbre(n, &((*a)->se), *a);
    } else if (n->x < (*a)->xc && n->y >= (*a)->yc){
        insererNoeudArbre(n, &((*a)->no), *a);
    } else {
        insererNoeudArbre(n, &((*a)->ne), *a);
    }
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    // Arbre vide
    if (*a == NULL){
        Noeud *n = (Noeud*)malloc(sizeof(Noeud));
        n->x = x;
        n->y = y;
        n->voisins = NULL;
        n->num = ++(R->nbNoeuds);

        CellNoeud *cn = (CellNoeud*)malloc(sizeof(CellNoeud));
        cn->nd = n;
        cn->suiv = R->noeuds;
        R->noeuds = cn;

        insererNoeudArbre(n, a, parent);
        return n;
    }

    // Feuille
    if ((*a)->noeud != NULL){
        if ((*a)->noeud->x == x && (*a)->noeud->y == y){
            return (*a)->noeud; //trouvé
        } 

        // pas trouvé -> créer
        Noeud *n = (Noeud*)malloc(sizeof(Noeud));
        n->x = x;
        n->y = y;
        n->voisins = NULL;
        n->num = ++(R->nbNoeuds);

        CellNoeud *cn = (CellNoeud*)malloc(sizeof(CellNoeud));
        cn->nd = n;
        cn->suiv = R->noeuds;
        R->noeuds = cn;

        insererNoeudArbre(n, a, parent);
        return n;
    }

    //Cellule interne
    if (x < (*a)->xc){
        if (y < (*a)->yc){
            return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
        } else {
            return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
        }
    } else {
        if (y < (*a)->yc){
            return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
        } else {
            return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
        }
    }
}

Reseau* reconstitueReseauArbre(Chaines* C) {
    if (!C) return NULL;

    /* 1. Création du réseau */
    Reseau* R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    /* 2. Bornes min/max */
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);

    /* 3. Racine de l’arbre quaternaire */
    ArbreQuat* arbre = creerArbreQuat((xmin + xmax) / 2.0, (ymin + ymax) / 2.0, xmax - xmin, ymax - ymin);

    /* 4. Parcours des chaînes */
    CellChaine* ch = C->chaines;
    while (ch){
        CellPoint* p = ch->points;
        Noeud *premier = NULL, *prec = NULL, *cour = NULL;

        while (p){
            cour = rechercheCreeNoeudArbre(R, &arbre, arbre, p->x, p->y);

            if (!premier)
                premier = cour;

            if (prec && prec != cour){
                /* vérifier si voisin déjà existant */
                CellNoeud *v = prec->voisins;
                int existe = 0;
                while (v){
                    if (v->nd == cour){
                        existe = 1;
                        break;
                    }
                    v = v->suiv;
                }

                if (!existe){
                    CellNoeud* v1 = malloc(sizeof(CellNoeud));
                    v1->nd = cour;
                    v1->suiv = prec->voisins;
                    prec->voisins = v1;

                    CellNoeud* v2 = malloc(sizeof(CellNoeud));
                    v2->nd = prec;
                    v2->suiv = cour->voisins;
                    cour->voisins = v2;
                }
            }
            
            prec = cour;
            p = p->suiv;
        }

        /* 5. Commodité */
        if (premier && prec && premier != prec){
            CellCommodite* com = malloc(sizeof(CellCommodite));
            com->extrA = premier;
            com->extrB = prec;
            com->suiv = R->commodites;
            R->commodites = com;
        }

        ch = ch->suiv;
    }

    return R;
}