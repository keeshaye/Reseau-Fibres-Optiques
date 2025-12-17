#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Chaine.h"
#include "SVGwriter.h"

Chaines *lectureChaines(FILE *f){

     Chaines *chs = (Chaines*) malloc(sizeof(Chaines));

      /* Lecture du nombre de chaînes et de gamma */
     fscanf(f, "NbChain: %d\n", &(chs->nbChaines));
     fscanf(f, "Gamma: %d\n", &(chs->gamma));

     chs->chaines = NULL;

     for (int i=0; i<chs->nbChaines; i++){
          int num, nbPoints;
          fscanf(f, "%d %d", &num, &nbPoints);
          
          /* Nouvelle cellule chaîne */
          CellChaine *ch = (CellChaine*)malloc(sizeof(CellChaine));
          ch->numero = num;
          ch->points = NULL;

          // insertion en fin
          if (chs->chaines == NULL){
               chs->chaines = ch;
               ch->suiv = NULL;
          } else {
               CellChaine *tmp = chs->chaines;
               while (tmp->suiv != NULL){
                    tmp = tmp->suiv;
               }
               tmp->suiv = ch;
               ch->suiv = NULL;
          }
          
          /* Lecture des points */
          for (int j = 0; j < nbPoints; j++){
               double x, y;
               fscanf(f, "%lf %lf", &x, &y);

               CellPoint *pt = (CellPoint*)malloc(sizeof(CellPoint));
               pt->x = x;
               pt->y = y;

               /* insertion en tete de la liste de points */
               pt->suiv = ch->points;
               ch->points = pt;
          }
     }
     return chs;
}

void ecrireChaines(Chaines *C, FILE *fic){

     if (!C || !fic) return;

     /* Ecriture du nombre de chaînes et de gamma */
     fprintf(fic, "NbChain: %d\n", C->nbChaines);
     fprintf(fic, "Gamma: %d\n", C->gamma);

     CellChaine *cc = C->chaines;

     while (cc != NULL){
          // Compter le nombre de points
          int nbPoints = 0;
          CellPoint *pt = cc->points;
          while (pt != NULL){
               nbPoints++;
               pt = pt->suiv;
          }

          //Ecrire le numero et le nombre de points
          fprintf(fic, "%d %d", cc->numero, nbPoints);

          // Mettre les pointeurs dans un tableau pour restaurer l'ordre
          CellPoint **T = (CellPoint**)malloc(nbPoints * sizeof(CellPoint*));
          pt = cc->points;
          for (int i = 0; i < nbPoints; i++){
               T[i] = pt;
               pt = pt->suiv;
          }

          // Écrire du premier point lu → dernier dans la liste chaînée
          for (int i = nbPoints - 1; i >= 0; i--){
               fprintf(fic, " %.2lf %.2lf", T[i]->x, T[i]->y);
          }
          fprintf(fic, "\n");

          free(T);
          cc = cc->suiv;
     }
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurChaine(CellChaine *c){
     if (!c || !c->points) return 0.0;

     double total = 0.0;
     CellPoint *p = c->points;
     CellPoint *prev = p;
     p = p->suiv;

     while (p){
          double dx = p->x - prev->x;
          double dy = p->y - prev->y;
          total += sqrt(dx*dx + dy*dy);

          prev = p;
          p = p->suiv;
     }

     return total;
}

double longueurTotale(Chaines *C){
     if (!C) return 0.0;
     double total = 0.0;
     CellChaine *ch = C->chaines;

     while (ch != NULL){
          total += longueurChaine(ch);
          ch = ch->suiv; 
     }
     return total;
}

int comptePointsTotal(Chaines *C){
     int total = 0;
     CellChaine *ch = C->chaines;

     while (ch != NULL){
          CellPoint *p = ch->points;
          while (p != NULL){
               total++;
               p = p->suiv;
          }
          ch = ch->suiv;
     }
     return total;
}