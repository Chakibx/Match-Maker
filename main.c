#include "fonctions.h"
int match_duration;
int num_teams;
char **team_names;
int teams_remaining[MAX_TEAMS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
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
