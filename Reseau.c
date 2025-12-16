#include "Reseau.h"
#include "SVGwriter.h"
#include <stdlib.h>
#include <stdio.h>

Noeud *rechercheCreeNoeudListe(Reseau *R, double x, double y){
    //cherche si le noeud existe deja
    CellNoeud *cn = R->noeuds;
    while (cn){
        if (cn->nd->x == x && cn->nd->y == y){
            return cn->nd;
        }
        cn = cn->suiv;
    }

    //n'existe pas on le cree
    Noeud *n = (Noeud*)malloc(sizeof(Noeud));
    n->x = x;
    n->y = y;
    n->voisins = NULL;

    R->nbNoeuds++;
    n->num = R->nbNoeuds;

    //insertion dans la liste des noeuds(en tete)
    CellNoeud *nouv = (CellNoeud*)malloc(sizeof(CellNoeud));
    nouv->nd = n;
    nouv->suiv = R->noeuds;

    R->noeuds = nouv;

    return n;
}

Reseau *reconstitueReseauListe(Chaines *C){
    //Creation du reseau vide
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    R->nbNoeuds = 0;
    R->gamma = C->gamma;
    R->noeuds = NULL;
    R->commodites = NULL;

    //parcours des chaines
    CellChaine *ch = C->chaines;

    while (ch != NULL){
        CellPoint *p = ch->points;
        if (!p){
            ch = ch->suiv;
            continue;
        }
        //Premier noeud de la chaine
        Noeud *A = rechercheCreeNoeudListe(R, p->x, p->y);

        CellPoint *q = p->suiv;

        while (q != NULL){
            //Noeud suivant
            Noeud *B = rechercheCreeNoeudListe(R, q->x, q->y);

            //Ajout des voisins A -> B
            CellNoeud *vA = A->voisins;
            int existe = 0;
            while (vA != NULL){
                if (vA->nd == B){
                    existe = 1;
                    break;
                }
                vA = vA->suiv;
            }
            if (!existe){
                CellNoeud *nv = (CellNoeud*)malloc(sizeof(CellNoeud));
                nv->nd = B;
                nv->suiv = A->voisins;
                A->voisins = nv;
            }

            //Ajout des voisin B -> A
            CellNoeud *vB = B->voisins;
            existe = 0;
            while (vB != NULL){
                if (vB->nd == A){
                    existe = 1;
                    break;
                }
                vB = vB->suiv;
            }
            if (!existe){
                CellNoeud *nv = (CellNoeud*)malloc(sizeof(CellNoeud));
                nv->nd = A;
                nv->suiv = B->voisins;
                B->voisins = nv;
            }

            //Passer au segment suivant
            A = B;
            q = q->suiv;
        }

        //Ajout de la commodite
        CellPoint *extr1 = ch->points;
        CellPoint *extr2 = ch->points;
        while (extr2->suiv != NULL){
            extr2 = extr2->suiv;
        }

        Noeud *n1 = rechercheCreeNoeudListe(R, extr1->x, extr1->y);
        Noeud *n2 = rechercheCreeNoeudListe(R, extr2->x, extr2->y);

        CellCommodite *com = (CellCommodite*)malloc(sizeof(CellCommodite));
        com->extrA = n1;
        com->extrB = n2;
        com->suiv = R->commodites;
        R->commodites = com;

        ch = ch->suiv;
    }
    return R;
}

int nbCommodites(Reseau *R){
    int cpt = 0;
    CellCommodite *c = R->commodites;

    while (c != NULL){
        cpt++;
        c = c->suiv;
    }
    return cpt;
}

int nbLiaisons(Reseau *R){
    int total = 0;
    CellNoeud *cn = R->noeuds;

    while (cn != NULL){
        CellNoeud *v = cn->nd->voisins;
        while (v != NULL){
            total++;
            v = v->suiv;
        }
        cn = cn->suiv;
    }
    return total / 2;
}

void ecrireReseau(Reseau *R, FILE *f){
    if (!R || !f) return;

    /* Entete */
    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n\n", R->gamma);

    /* Ecriture des noeuds */
    CellNoeud *cn = R->noeuds;
    while (cn != NULL){
        Noeud *n = cn->nd;
        fprintf(f, "v %d %lf %lf\n", n->num, n->x, n->y);
        cn = cn->suiv;
    }
    fprintf(f, "\n");

    /* Ecriture des liaisons */
    cn = R->noeuds;
    while (cn != NULL){
        Noeud *n = cn->nd;
        CellNoeud *v = n->voisins;
        while (v != NULL){
            if (n->num < v->nd->num){
                fprintf(f, "l %d %d\n", n->num, v->nd->num);
            }
            v = v->suiv;
        }
        cn = cn->suiv;  
    }
    fprintf(f, "\n");

    /* Ecriture des commodités */
    CellCommodite *c = R->commodites;
    while (c != NULL){
        fprintf(f, "k %d %d\n", c->extrA->num, c->extrB->num);
        c = c->suiv;
    }
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}

