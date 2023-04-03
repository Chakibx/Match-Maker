#include "fonctions.h"

void read_team_names(char* filename, int* num_teams, char *** team_names) {
    FILE *file;
    *team_names = (char**) malloc(MAX_TEAMS * sizeof(char*));
    char line[MAX_TEAM_NAME_LEN];

    for (int i = 0; i < MAX_TEAMS; i++) {
        (*team_names)[i] = (char*) malloc(MAX_TEAM_NAME_LEN * sizeof(char));
    }

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        exit(1);
    }

    // Lecture de la durée du match sur la première ligne
    fgets(line, sizeof(line), file);

    //Verification si une duree est introduite
    int num;
    if (sscanf(line, "%d", &num) == 1) {
        match_duration = atoi(line);
    }else {
        match_duration = DURATION;
    }

    // Lecture des noms d'équipes sur les lignes suivantes
    while (fgets(line, sizeof(line), file)) {
        // Supprime le caractère de fin de ligne
        line[strcspn(line, "\r\n")] = 0;

        // Supprime les espaces inutiles à la fin de la chaîne de caractères
        int len = strlen(line);
        while (len > 0 && line[len-1] == ' ') {
            line[len-1] = '\0';
            len--;
        }

        // Vérifie si la ligne contient que des vides
        int only_spaces = 1;
        for (int i = 0; i < len; i++) {
            if (line[i] != ' ') {
                only_spaces = 0;
                break;
            }
        }

        // Si la ligne ne contient pas que des vides, ajoute le nom de l'équipe à la liste
        if (!only_spaces) {
            strcpy((*team_names)[*num_teams], line);
            (*num_teams)++;
        }
    }

    fclose(file);

    // Mélange les noms d'équipes de manière aléatoire
    srand(time(NULL));

    // Shuffle
    for (int i = *num_teams - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_TEAM_NAME_LEN];
        strcpy(temp, (*team_names)[i]);
        strcpy((*team_names)[i], (*team_names)[j]);
        strcpy((*team_names)[j], temp);
    }
}
void *play_match(void *ma)
{
    Match match = (Match) ma;

    int duration = 0;
    int action;

    // Simulate match by randomly generating scores
    printf("DEBUT %s %d - %d %s [TOUR %d]\n",team_names[match->team1],match->score1, match->score2,team_names[match->team2],match->tour);
    while (duration < match_duration)
    {
        action = rand() % 100; // simulate random action
        if (action < 98)
        { // 98% de chance de ne pas marquer
            duration++;
        }
        else if (action == 99 )
        { // 1% de chance de marquer pour l'equipe 1
            match->score1++;
            duration++;
            printf("(%d') %.20s %d - %d %-20s\n",duration, team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
        }
        else //action == 100
        { // 1% de chance de marquer pour l'equipe 2
            match->score2++;
            duration++;
            printf("(%d') %.20s %d - %d %-20s\n",duration, team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
        }
        sleep(0.005);
    }
    int nTab = 5;
    while(match->score1 == match->score2){ //SI MATCH NUL, SEANCE DE TAB
        int tab;
        while(nTab > 0){
            tab = rand() % 100;
            if(tab > 20){ //80% de chance de marquer
                match->score1++;
                printf("%.20s (%d) - (%d) %-20s\n", team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
            }
            tab = rand() % 100;
            if(tab < 40){//60% de chance de marquer
                match->score2++;
                printf("%.20s (%d) - (%d) %-20s\n", team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
            }
            nTab--;
        }
        nTab=1;
    }
    if(match->score1 > match->score2){
        pthread_mutex_lock(&mutex);
        teams_remaining[match->team1] = match->tour+1;
        teams_remaining[match->team2] = -1;
        pthread_mutex_unlock(&mutex);
        printf("FIN %s* %d - %d %s\n",team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
    }
    else{
        pthread_mutex_lock(&mutex);
        teams_remaining[match->team2] = match->tour+1;
        teams_remaining[match->team1] = -1;
        pthread_mutex_unlock(&mutex);
        printf("FIN %s %d - %d %s*\n",team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
    }
    pthread_exit(0);
}
