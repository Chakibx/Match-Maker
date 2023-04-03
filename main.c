#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_TEAMS 16 // nombre max des equipes
#define MAX_TEAM_NAME_LEN 50 //taille de nom d'equipes
#define DURATION 90 // default match duration is 90 minutes, equivalent to 5400sec
#define FILENAME "equipe.txt"

int match_duration;
typedef struct Match{
    int team1;
    int team2;
    int score1;
    int score2;
    int tour;
}*Match;
int num_teams;
char **team_names;
int teams_remaining[MAX_TEAMS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


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
    int line_len = strlen(line);

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

void printMatch(Match match){
    printf("Match %s %d - %d %s\n",team_names[match->team1],match->score1, match->score2,team_names[match->team2]);
}

int main(int argc, char *argv[])
{
    char * filename;
    pthread_mutex_init(&mutex,NULL);
    if (argc != 2) {
        filename = FILENAME;
    }else {
        filename = argv[1];
    }


    //Creation d'une liste randomise avec les equipes
    //Fin lecture des equipes
    read_team_names(filename, &num_teams, &team_names);

    if (num_teams % 2 != 0 || num_teams > MAX_TEAMS)
    {
        printf("Number of teams must be a power of 2 and less than %d\n", MAX_TEAMS);
        exit(EXIT_FAILURE);
    }

    //Initialisation du tableau a 1 qui corresspond au numero du tour
    for (int i = 0; i < num_teams; i++)
    {
        teams_remaining[i] = 1;
    }

    int num_matchs_total = num_teams-1;

    // Create threads to play matches
    pthread_t threads[num_matchs_total];
    Match matchs[num_matchs_total];

    int team2 = 0;
    int num_match = 0;
    int t;
    while(num_match < num_matchs_total){
        for (int team1 = 0; team1 < num_teams; team1++)
        {
            pthread_mutex_lock(&mutex);
            if(teams_remaining[team1] > 0){
                t = teams_remaining[team1]; //le tour
                team2 = team1+1;
                while(team2 < num_teams){
                    if(teams_remaining[team2] == t){
                        matchs[num_match] = (Match)malloc(sizeof(struct Match));
                        matchs[num_match]->team1 = team1;
                        matchs[num_match]->team2 = team2;
                        matchs[num_match]->score1 = 0;
                        matchs[num_match]->score2 = 0;
                        matchs[num_match]->tour = teams_remaining[team2];
                        teams_remaining[team1] = 0;
                        teams_remaining[team2] = 0;
                        pthread_create(&threads[num_match], NULL, play_match, matchs[num_match]);
                        num_match++;
                        team2++;
                        break;
                    }
                    team2++;
                }
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    for (int i = 0; i < num_matchs_total; i++)
    {
        pthread_join(threads[i], NULL);
    }

    //Enregistrement des matchs dans un fichier texte
    // écriture des matchs dans un fichier texte
    FILE* fp;
    fp = fopen("matchs.txt", "w"); // ouverture du fichier en mode écriture
    if (fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return 1;
    }

    // écriture des informations de chaque match dans le fichier texte
    for (int i = 0; i < num_match; i++) {
        fprintf(fp, "Match %d : %s [%d] : [%d] %s | Tour %d\n", i + 1, team_names[matchs[i]->team1],matchs[i]->score1,matchs[i]->score2, team_names[matchs[i]->team2], matchs[i]->tour);
    }

    fclose(fp); // fermeture du fichier
    //free(matchs); // libération de la mémoire allouée pour le tableau de matchs
    for (int i = 0; i < num_teams; i++) {
        free(team_names[i]);
    }
    free(team_names);

    pthread_mutex_destroy(&mutex);

    return 1;
}
