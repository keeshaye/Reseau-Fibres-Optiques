#include <stdio.h>
#include <stdlib.h>
#include "Chaine.h"

int main(int argc, char *argv[]){
    if (argc != 4){
        printf("Usage: %s <lecture.cha> <ecriture.cha> <nomSVG>\n", argv[0]);
        return 1;
    }

    FILE *f_lec = fopen(argv[1], "r");
    if (f_lec == NULL){
        printf("Erreur  impossible d'ouvrir le fichier\n");
        return 1;
    }

    Chaines *C = lectureChaines(f_lec);
    fclose(f_lec);

    if (!C){
        printf("Erreur lors de la lecture\n");
        return 1;
    }

    FILE *f_eri = fopen(argv[2], "w");
    if (f_eri == NULL){
        printf("Erreur de fichier\n");
        return 1;
    }

    ecrireChaines(C,f_eri);
    fclose(f_eri);
    
    printf("Ecriture faites dans le fichier %s\n", argv[2]);

    //Affichage SVG
    printf("Generation du svg (%s.html)\n", argv[3]);
    afficheChainesSVG(C, argv[3]);

    printf("Nombre totals des points : %d\n", comptePointsTotal(C));

    return 0;
}