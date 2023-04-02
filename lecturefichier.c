/*
//version 1
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

    fp = fopen("/home/ayoub/university/equipe.txt", "r");
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

*/
/*
//Mise des equipe dans une liste chainnée 
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

    fp = fopen("/home/ayoub/university/equipe.txt", "r");
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
}
*/

/*
//Resolution probleme :(EQUIPE COMPOSE DEUX MOTS = DEUX EQUIPES)
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
    char ligne[100];
    char* equipe_nom;
    noeud* tete = NULL;
    noeud* dernier = NULL;

    fp = fopen("/home/ayoub/university/equipe.txt", "r");
    if(fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return 1;
    }

    // Lecture de la durée du match
    fscanf(fp, "%d", &duration);
    printf("Durée du match : %d minutes\n", duration);

    // Lecture des équipes
    while(fgets(ligne, sizeof(ligne), fp) != NULL) {
        // Extraire le nom de l'équipe
        equipe_nom = strtok(ligne, "\n");
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
}

*/
/* erreur de segmentation equipe_nom
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un nœud de la liste chaînée
typedef struct noeud {
    char* equipe_nom;
    struct noeud* suivant;
} noeud;

int main() {
    FILE *fp;
    int duration;
    char ligne[100];
    char* equipe_nom;
    noeud* tete = NULL;
    noeud* dernier = NULL;

    fp = fopen("/home/ayoub/university/equipe.txt", "r");
    if(fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return 1;
    }

    // Lecture de la durée du match
    fscanf(fp, "%d", &duration);
    printf("Durée du match : %d minutes\n", duration);

    // Lecture des équipes
    while(fgets(ligne, sizeof(ligne), fp) != NULL) {
        // Extraire le nom de l'équipe
        equipe_nom = strtok(ligne, "\n");
        
        // Allouer de la mémoire pour stocker le nom de l'équipe
        char* nom = malloc(strlen(equipe_nom) + 1); //************************************************************************************
        strcpy(nom, equipe_nom);

        // Créer un nouveau nœud pour l'équipe
        noeud* n = (noeud*) malloc(sizeof(noeud));
        n->equipe_nom = nom;
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
        printf("Equipe : %s\n", courant->equipe_nom);
        courant = courant->suivant;
    }

    // Libérer la mémoire allouée pour les nœuds et les noms des équipes
    courant = tete;
    while(courant != NULL) {
        noeud* suivant = courant->suivant;
        free(courant->equipe_nom);
        free(courant);
        courant = suivant;
    }

    return 0;
}
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un nœud de la liste chaînée
typedef struct noeud {
    char* nom;
    struct noeud* suivant;
} noeud;

int main() {
    FILE *fp;
    int duration;
    char ligne[100];
    char* equipe_nom;
    noeud* tete = NULL;
    noeud* dernier = NULL;

    fp = fopen("/home/ayoub/university/equipe.txt", "r");
    if(fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return 1;
    }

    // Lecture de la durée du match
    fscanf(fp, "%d", &duration);
    printf("Durée du match : %d minutes\n", duration);

    // Lecture des équipes
    while(fgets(ligne, sizeof(ligne), fp) != NULL) {
        // Enlever le saut de ligne à la fin de la ligne
        ligne[strcspn(ligne, "\n")] = 0;
        // Créer une nouvelle équipe
        char* nom = strdup(ligne);
        // Créer un nouveau nœud pour l'équipe
        noeud* n = (noeud*) malloc(sizeof(noeud));
        n->nom = nom;
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
        printf("Equipe : %s\n", courant->nom);
        courant = courant->suivant;
    }

    // Libérer la mémoire allouée pour les nœuds et les noms d'équipe
    courant = tete;
    while(courant != NULL) {
        noeud* suivant = courant->suivant;
        free(courant->nom);
        free(courant);
        courant = suivant;
    }

    return 0;
}