#include <stdio.h>
#include "Reseau.h"
#include "Graphe.h" 
#include "ArbreQuat.h"

int main() {

    // Charger ou générer un réseau
    FILE *f = fopen("00014_burma.cha", "r");
    if (!f) {
        printf("Erreur ouverture fichier\n");
        return 1;
    }

    Chaines* C = lectureChaines(f);
    fclose(f);

    if (!C) {
        printf("Erreur lecture des chaines\n");
        return 1;
    }

    Reseau* R = reconstitueReseauArbre(C);
    if (!R) {
        printf("Erreur reconstruction réseau\n");
        return 1;
    }

    // Tester la fonction
    int ok = reorganiseReseau(R);
    if (ok) {
        printf("Toutes les arêtes respectent la contrainte gamma.\n");
    } else {
        printf("Certaines arêtes dépassent la contrainte gamma.\n");
    }
    return 0;
}
