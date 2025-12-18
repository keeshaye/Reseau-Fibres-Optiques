#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage : %s <fichier.cha> <methode>\n", argv[0]);
        printf("Methode : 1 = Liste, 2 = Hachage, 3 = Arbre quaternaire\n");
        return 1;
    }

    /* Lecture du fichier */
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        return 1;
    }

    Chaines *C = lectureChaines(f);
    fclose(f);

    if (!C) {
        printf("Erreur lors de la lecture du fichier .cha\n");
        return 1;
    }

    int methode = atoi(argv[2]);
    Reseau *R = NULL;

    /* Choix de la méthode */
    switch (methode) {
        case 1:
            printf("Méthode : Liste chaînée\n");
            R = reconstitueReseauListe(C);
            afficheReseauSVG(R, "reseau_liste"); // génère reseau_liste.html
            printf("Generation du svg reseau_liste.html\n");
            break;

        case 2:
            printf("Méthode : Table de hachage\n");
            R = reconstitueReseauHachage(C, 10007);
            afficheReseauSVG(R, "reseau_hash"); // génère reseau_hash.html
            printf("Generation du svg reseau_hash.html\n");
            break;

        case 3:
            printf("Méthode : Arbre quaternaire\n");
            R = reconstitueReseauArbre(C);
            afficheReseauSVG(R, "reseau_arbre"); // génère reseau_arbre.html
            printf("Generation du svg reseau_arbre.html\n");
            break;

        default:
            printf("Erreur : méthode inconnue (%d)\n", methode);
            return 1;
    }

    if (!R) {
        printf("Erreur : réseau non créé\n");
        return 1;
    }

    /* Résumé */
    printf("Réseau reconstruit avec succès !\n");
    printf("Nombre de noeuds : %d\n", R->nbNoeuds);
    printf("NbLiaisons: %d\n", nbLiaisons(R));
    printf("NbCommodites: %d\n", nbCommodites(R));
    printf("Gamma : %d\n", R->gamma);

    // --- Ecriture du fichier .res ---
    FILE *f_res = fopen("00014_burma_write.res", "w");
    if (!f_res){
        perror("Erreur creation fichier 00014_burma_write.res");
        return 1;
    }

    ecrireReseau(R, f_res);
    fclose(f_res);
    printf("Fichier 00014_burma_write.res écrit avec succès.\n");

    return 0;
}
