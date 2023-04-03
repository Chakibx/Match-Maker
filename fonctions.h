#ifndef OS_FONCTIONS_H
#define OS_FONCTIONS_H

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

extern int match_duration;
extern int num_teams;
extern char **team_names;
extern int teams_remaining[MAX_TEAMS];
extern pthread_mutex_t mutex;

typedef struct Match{
    int team1;
    int team2;
    int score1;
    int score2;
    int tour;
}*Match;



void read_team_names(char* filename, int* num_teams, char *** team_names);
void *play_match(void *ma);

#endif

