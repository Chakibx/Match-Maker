#include <stdio.h>
#include <stdlib.h>
#define MAX_TEAMS 32
#define DURATION 90 // default match duration is 90 minutes, equivalent to 5400sec
#include <string.h>
#include <time.h>
#define MAX_TEAM_NAME_LEN 50
#define MAX_NUM_TEAMS 32
void shuffle(char teams[][MAX_TEAM_NAME_LEN], int n);

char** read_team_names(char* filename, int* num_teams, int* match_duration) {
    FILE *file;
    char **team_name = (char**) malloc(MAX_NUM_TEAMS * sizeof(char*));
    char line[MAX_TEAM_NAME_LEN];

    for (int i = 0; i < MAX_NUM_TEAMS; i++) {
        team_name[i] = (char*) malloc(MAX_TEAM_NAME_LEN * sizeof(char));
    }

    file = fopen(filename, "r");
    printf("filename '%s'\n",filename);
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        exit(1);
    }

    // Lecture de la durée du match sur la première ligne
    fgets(line, sizeof(line), file);
    //fscanf(file,"DUREE=%d",*match_duration);
    *match_duration = atoi(line);

    // Lecture des noms d'équipes sur les lignes suivantes
    while (fgets(line, sizeof(line), file)) {
        // Supprime le caractère de fin de ligne
        line[strcspn(line, "\n")] = 0;

        // Ajoute le nom de l'équipe à la liste
        strcpy(team_name[*num_teams], line);
        (*num_teams)++;
    }

    fclose(file);

    // Mélange les noms d'équipes de manière aléatoire
    srand(time(NULL));
    shuffle(team_name, *num_teams);

    return team_name;
}

void shuffle(char teams[][MAX_TEAM_NAME_LEN], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_TEAM_NAME_LEN];
        strcpy(temp, teams[i]);
        strcpy(teams[i], teams[j]);
        strcpy(teams[j], temp);
    }
}