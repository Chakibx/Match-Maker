#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define TAILLE_TABLEAU 100
char * tableau[TAILLE_TABLEAU];

int readCmd() {
    int nombreDeMots = 0;
    char ligne[TAILLE_TABLEAU];


    printf("$ ");
    scanf("%[^\n]", ligne);

    tableau[nombreDeMots] = strtok(ligne, " ");

    while ( tableau[nombreDeMots] != NULL ) {
        nombreDeMots ++;
        tableau[nombreDeMots] = strtok ( NULL, " " );
    }
    return nombreDeMots;
}


int main() {
    int n = readCmd();
    while ( n>0) {
        pid_t pid;
        pid = fork();
        if (pid=0){
            execvp(tableau[0],tableau);
        } else {
            printf("Erreur lors de la création du processus fils \n");
        }
    }
}
