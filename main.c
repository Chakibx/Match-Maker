#include "fonctions.h"
int match_duration = DURATION;
int num_teams;
char **team_names;
int teams_remaining[MAX_TEAMS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int main(int argc, char *argv[])
{
    char * filename;
    if (argc != 2) {
        filename = FILENAME;
    }else {
        filename = argv[1];
    }

    //Creation d'une liste randomise avec les equipes
    //Fin lecture des equipes
    read_team_names(filename, &num_teams, &team_names);

    //Initialisation du tableau a 1 qui corresspond au numero du tour
    for (int i = 0; i < num_teams; i++)
    {
        teams_remaining[i] = 1;
    }

    int num_matchs_total = num_teams-1;
    Match matchs[num_matchs_total];
    int num_match = 0;
    int team1 = 0;
    int team2 = 0;
    int tour = 1;
    int manual=1 ;
    do {
        printf("Choisir le mode de jeu : [1]:Mode simulation concurrente | [2]:Mode manuel \n");
        scanf("%d",&manual);
    }while(manual != 1 && manual != 2);

    //******************************************************************************************
    if (manual == 1) {

        pthread_mutex_init(&mutex,NULL);

        // Create threads to play matches
        pthread_t threads[num_matchs_total];
        while(num_match < num_matchs_total){
            for (team1 = 0; team1 < num_teams; team1++)
            {
                pthread_mutex_lock(&mutex);
                if(teams_remaining[team1] > 0){
                    tour = teams_remaining[team1]; //le tour
                    team2 = team1+1;
                    while(team2 < num_teams){
                        if(teams_remaining[team2] == tour){
                            matchs[num_match] = (Match)malloc(sizeof(struct Match));
                            matchs[num_match]->team1 = team1;
                            matchs[num_match]->team2 = team2;
                            matchs[num_match]->score1 = 0;
                            matchs[num_match]->score2 = 0;
                            matchs[num_match]->tour = teams_remaining[team2];
                            teams_remaining[team1] = 0;
                            teams_remaining[team2] = 0;
                            pthread_create(&threads[num_match], NULL, simulate_match, matchs[num_match]);
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
        pthread_mutex_destroy(&mutex);
    }else {//***********************************************************************************
        while(num_match<num_matchs_total) {
            team1=0;
            team2=0;
            while(team1<num_teams && team2 < num_teams) {
                while (teams_remaining[team1] <= 0) { team1++; }
                team2 = team1 + 1;
                while (teams_remaining[team2] <= 0) { team2++; }
                matchs[num_match] = (Match) malloc(sizeof(struct Match));
                matchs[num_match]->team1 = team1;
                matchs[num_match]->team2 = team2;
                matchs[num_match]->score1 = 0;
                matchs[num_match]->score2 = 0;
                matchs[num_match]->tour = tour;
                play_match(matchs[num_match]);
                num_match++;
                team1= team2 + 1;
            }
            tour++;
        }
    }

    save_matchs(team_names, matchs, num_match);

    //free(matchs); // libération de la mémoire allouée pour le tableau de matchs
    for (int i = 0; i < num_teams; i++) {
        free(team_names[i]);
    }
    free(team_names);

    return 1;
}
