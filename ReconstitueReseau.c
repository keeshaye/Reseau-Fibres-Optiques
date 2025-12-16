#include "Reseau.h"

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Usage : %s <fichier.cha>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f){
        printf("Erreur : impossible d'ouvrir %s\n", argv[1]);
        return 1;
    }

    Chaines *C = lectureChaines(f);
    fclose(f);

    if (!C){
        printf("Erreur lors de la lecture du fichier .cha\n");
        return 1;
    }

    printf("Lecture .cha OK.\n");
    
    Reseau *R = reconstitueReseauListe(C);

    if (!R){
        printf("Erreur : reseau non cree.\n");
        return 1;
    }

    printf("Reseau reconstruit avec succes !\n");
    printf("Nombre de noeuds : %d\n", R->nbNoeuds);
    printf("Gamma : %d\n", R->gamma);

    return 0;
}
