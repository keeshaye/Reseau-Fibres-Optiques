#include <stdlib.h>
#include "File.h"

File* creerFile() {
    File* f = (File*)malloc(sizeof(File));
    if (!f) return NULL;
    f->tete = NULL;
    f->queue = NULL;
    return f;
}

int fileVide(File* f) {
    return (f->tete == NULL);
}

void enfiler(File* f, int x) {
    Cellule* c = (Cellule*)malloc(sizeof(Cellule));
    c->val = x;
    c->suiv = NULL;
    if (f->queue) {
        f->queue->suiv = c;
        f->queue = c;
    } else {
        f->tete = f->queue = c;
    }
}

int defiler(File* f) {
    if (fileVide(f)) return -1; // ou autre valeur d’erreur
    Cellule* tmp = f->tete;
    int val = tmp->val;
    f->tete = tmp->suiv;
    if (!f->tete) f->queue = NULL;
    free(tmp);
    return val;
}

void libererFile(File* f) {
    while (!fileVide(f)) defiler(f);
    free(f);
}
