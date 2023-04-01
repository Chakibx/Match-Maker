#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter une équipe
typedef struct equipe {
    char nom[100];
} equipe;

// Structure pour représenter un nœud de la liste chaînée
typedef struct noeud {
    equipe e;
    struct noeud* suivant;
} noeud;

int main() {
    FILE *fp;
    int duration;
    char equipe_nom[100];
    noeud* tete = NULL;
    noeud* dernier = NULL;
    int count = 0;

    fp = fopen("mon_fichier.txt", "r");
    if(fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return 1;
    }

    // Lecture de la durée du match
    fscanf(fp, "%d", &duration);
    printf("Durée du match : %d minutes\n", duration);

    // Lecture des équipes
    while(fscanf(fp, "%s", equipe_nom) == 1) {
        // Créer une nouvelle équipe
        equipe e;
        strcpy(e.nom, equipe_nom);

        // Créer un nouveau nœud pour l'équipe
        noeud* n = (noeud*) malloc(sizeof(noeud));
        n->e = e;
        n->suivant = NULL;

        // Lier le nouveau nœud au nœud précédent de la liste chaînée
        if(tete == NULL) {
            tete = n;
            dernier = n;
        }
        else {
            dernier->suivant = n;
            dernier = n;
        }

        count++; // Incrémenter le nombre d'équipes lues
    }

    fclose(fp);

    printf("Nombre d'équipes : %d\n", count);

    // Parcourir la liste chaînée et afficher les équipes
    noeud* courant = tete;
    while(courant != NULL) {
        printf("Equipe : %s\n", courant->e.nom);
        courant = courant->suivant;
    }

    // Libérer la mémoire allouée pour les nœuds
    courant = tete;
    while(courant != NULL) {
        noeud* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }

    return 0;
}



/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter une équipe
typedef struct equipe {
    char nom[100];
} equipe;

// Structure pour représenter un nœud de la liste chaînée
typedef struct noeud {
    equipe e;
    struct noeud* suivant;
} noeud;

int main() {
    FILE *fp;
    int duration;
    char equipe_nom[100];
    noeud* tete = NULL;
    noeud* dernier = NULL;

    fp = fopen("mon_fichier.txt", "r");
    if(fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return 1;
    }

    // Lecture de la durée du match
    fscanf(fp, "%d", &duration);
    printf("Durée du match : %d minutes\n", duration);

    // Lecture des équipes
    while(fscanf(fp, "%s", equipe_nom) == 1) {
        // Créer une nouvelle équipe
        equipe e;
        strcpy(e.nom, equipe_nom);

        // Créer un nouveau nœud pour l'équipe
        noeud* n = (noeud*) malloc(sizeof(noeud));
        n->e = e;
        n->suivant = NULL;

        // Lier le nouveau nœud au nœud précédent de la liste chaînée
        if(tete == NULL) {
            tete = n;
            dernier = n;
        }
        else {
            dernier->suivant = n;
            dernier = n;
        }
    }

    fclose(fp);

    // Parcourir la liste chaînée et afficher les équipes
    noeud* courant = tete;
    while(courant != NULL) {
        printf("Equipe : %s\n", courant->e.nom);
        courant = courant->suivant;
    }

    // Libérer la mémoire allouée pour les nœuds
    courant = tete;
    while(courant != NULL) {
        noeud* suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }

    return 0;
}*/