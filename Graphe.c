#include <stdlib.h>
#include "Graphe.h"
#include "File.h"

Graphe* creerGraphe(Reseau* R) {
    if (!R) return NULL;

    Graphe* G = (Graphe*)malloc(sizeof(Graphe));
    if (!G) return NULL;

    G->nbsom = R->nbNoeuds;
    G->gamma = R->gamma;
    G->nbcommod = 0;  // initialement à zéro, pourra être rempli après
    G->T_commod = NULL;

    // Création des sommets
    G->T_som = (Sommet**)malloc(G->nbsom * sizeof(Sommet*));
    for (int i = 0; i < G->nbsom; i++) {
        G->T_som[i] = (Sommet*)malloc(sizeof(Sommet));
        G->T_som[i]->num = i + 1;  // correspond au numéro du noeud
        // On doit récupérer le nœud correspondant dans R->noeuds
        CellNoeud* cn = R->noeuds;
        while (cn && cn->nd->num != i + 1) cn = cn->suiv;
        if (cn) {
            G->T_som[i]->x = cn->nd->x;
            G->T_som[i]->y = cn->nd->y;
        }
        G->T_som[i]->L_voisin = NULL;
    }

    // Création des arêtes (chaque arête une seule fois)
    for (int i = 0; i < G->nbsom; i++) {
        // Récupération du noeud i+1 dans le réseau
        CellNoeud* cn = R->noeuds;
        while (cn && cn->nd->num != i + 1) cn = cn->suiv;
        if (!cn) continue;

        CellNoeud* voisin = cn->nd->voisins;
        while (voisin) {
            int u = i;  // sommet courant
            int v = voisin->nd->num - 1; // sommet voisin (index tableau)

            if (v > u) { // ajouter une seule fois pour éviter duplication
                Arete* a = (Arete*)malloc(sizeof(Arete));
                a->u = u;
                a->v = v;

                // ajout dans la liste des voisins de u
                Cellule_arete* cu = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                cu->a = a;
                cu->suiv = G->T_som[u]->L_voisin;
                G->T_som[u]->L_voisin = cu;

                // ajout dans la liste des voisins de v
                Cellule_arete* cv = (Cellule_arete*)malloc(sizeof(Cellule_arete));
                cv->a = a;
                cv->suiv = G->T_som[v]->L_voisin;
                G->T_som[v]->L_voisin = cv;
            }

            voisin = voisin->suiv;
        }
    }

    return G;
}

/* Fonction BFS pour retourner le chemin le plus court de u à v sous forme de liste d'entiers */
ListeEntiers* BFS_Chemin(Graphe* G, int u, int v) {
    int n = G->nbsom;
    int* pred = malloc(n * sizeof(int));
    int* vis = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) pred[i] = -1;

    File* Q = creerFile();
    enfiler(Q, u);
    vis[u] = 1;

    while (!fileVide(Q)) {
        int courant = defiler(Q);
        Cellule_arete* c = G->T_som[courant]->L_voisin;
        while (c) {
            int w = (c->a->u == courant) ? c->a->v : c->a->u;
            if (!vis[w]) {
                vis[w] = 1;
                pred[w] = courant;
                enfiler(Q, w);
            }
            c = c->suiv;
        }
    }

    libererFile(Q);
    free(vis);

    // Reconstitution du chemin
    ListeEntiers* chemin = creerListe();
    int courant = v;
    while (courant != -1) {
        ajoutTete(chemin, courant);
        courant = pred[courant];
    }
    free(pred);

    return chemin;
}

int reorganiseReseau(Reseau* r) {
    if (!r) return 0;

    Graphe* G = creerGraphe(r);
    if (!G) return 0;

    int n = G->nbsom;

    // Matrice de comptage des passages par arêtes
    int** nbPassages = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        nbPassages[i] = (int*)calloc(n, sizeof(int));
    }

    // Pour chaque commodité
    for (int k = 0; k < G->nbcommod; k++) {
        int u = G->T_commod[k].e1 - 1; // indices 0..n-1
        int v = G->T_commod[k].e2 - 1;

        ListeEntiers* chemin = BFS_Chemin(G, u, v);
        CelluleEntier* cel = chemin->tete;
        while (cel && cel->suiv) {
            int a = cel->val;
            int b = cel->suiv->val;
            nbPassages[a][b]++;
            nbPassages[b][a]++;
            cel = cel->suiv;
        }

        libererListe(chemin);
    }

    // Vérification contrainte gamma
    int ok = 1;
    for (int i = 0; i < n && ok; i++) {
        for (int j = 0; j < n; j++) {
            if (nbPassages[i][j] > G->gamma) {
                ok = 0;
                break;
            }
        }
    }

    // Libération mémoire
    for (int i = 0; i < n; i++) free(nbPassages[i]);
    free(nbPassages);

    return ok;
}