#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage : %s fichier.cha\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Erreur ouverture fichier\n");
        return 1;
    }

    Chaines *C = lectureChaines(f);
    fclose(f);

    clock_t t1, t2;
    double temps;

    /* -------- Liste chaînée -------- */
    t1 = clock();
    Reseau *R1 = reconstitueReseauListe(C);
    t2 = clock();
    temps = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Liste chaînée : %f s\n", temps);

    /* -------- Table de hachage -------- */
    t1 = clock();
    Reseau *R2 = reconstitueReseauHachage(C, 1000);
    t2 = clock();
    temps = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Hachage : %f s\n", temps);

    /* -------- Arbre quaternaire -------- */
    t1 = clock();
    Reseau *R3 = reconstitueReseauArbre(C);
    t2 = clock();
    temps = (double)(t2 - t1) / CLOCKS_PER_SEC;
    printf("Arbre quaternaire : %f s\n", temps);

    return 0;
}
