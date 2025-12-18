#ifndef _FILE_H_
#define _FILE_H_

typedef struct cellule {
    int val;
    struct cellule* suiv;
} Cellule;

typedef struct {
    Cellule* tete;
    Cellule* queue;
} File;

/* Création d’une file vide */
File* creerFile();

/* Vérifie si la file est vide */
int fileVide(File* f);

/* Enfile un élément */
void enfiler(File* f, int x);

/* Défiler un élément */
int defiler(File* f);

/* Libération mémoire */
void libererFile(File* f);

#endif
