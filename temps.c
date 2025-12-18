#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

/* Génère des chaînes aléatoires */
Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax) {
    if (nbChaines <= 0 || nbPointsChaine <= 0 || xmax <= 0 || ymax <= 0) return NULL;

    // Initialisation du générateur aléatoire
    srand(time(NULL));

    Chaines *C = (Chaines*)malloc(sizeof(Chaines));
    if (!C) return NULL;

    C->nbChaines = nbChaines;
    C->gamma = 1; 
    C->chaines = NULL;

    for (int i = 0; i < nbChaines; i++) {
        CellChaine *ch = (CellChaine*)malloc(sizeof(CellChaine));
        ch->numero = i + 1;

        ch->points = NULL;
        CellPoint *dernier = NULL;

        for (int j = 0; j < nbPointsChaine; j++) {
            CellPoint *pt = (CellPoint*)malloc(sizeof(CellPoint));
            pt->x = (double)(rand() % (xmax + 1));
            pt->y = (double)(rand() % (ymax + 1));
            pt->suiv = NULL;

            if (!ch->points) {
                ch->points = pt;
                dernier = pt;
            } else {
                dernier->suiv = pt;
                dernier = pt;
            }
        }

        // Insertion en tête dans C->chaines
        ch->suiv = C->chaines;
        C->chaines = ch;
    }

    return C;
}

int main(int argc, char *argv[]) {

    int nbPointsChaine = 50;
    int xmax = 5000, ymax = 5000;

    FILE *out = fopen("temps_calcul.txt", "a");
    if (!out) {
        printf("Erreur ouverture temps_calcul.txt\n");
        return 1;
    }

    fprintf(out, "nbPointsTotal Liste Hachage Arbre\n");

    for (int nbChaines = 1000; nbChaines <= 5000; nbChaines += 1000) {

        Chaines *C = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);
        int nbPointsTotal = nbChaines * nbPointsChaine;

        clock_t debut, fin;
        double t_liste, t_hash, t_arbre;

        /* Liste chaînée */
        debut = clock();
        Reseau *R_liste = reconstitueReseauListe(C);
        fin = clock();
        t_liste = (double)(fin - debut) / CLOCKS_PER_SEC;
        
        /* Table de hachage */
        int M = nbPointsTotal;
        debut = clock();
        Reseau *R_hash = reconstitueReseauHachage(C, M);
        fin = clock();
        t_hash = (double)(fin - debut) / CLOCKS_PER_SEC;
        
        /* Arbre quaternaire */
        debut = clock();
        Reseau *R_arbre = reconstitueReseauArbre(C);
        fin = clock();
        t_arbre = (double)(fin - debut) / CLOCKS_PER_SEC;
        
        fprintf(out, "%d %.6f %.6f %.6f\n",
                nbPointsTotal, t_liste, t_hash, t_arbre);

        printf("Points=%d | Liste=%.4f | Hachage=%.4f | Arbre=%.4f\n",
               nbPointsTotal, t_liste, t_hash, t_arbre);
    }

    fclose(out);
    return 0;
}
