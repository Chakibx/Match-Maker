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
    printf("num_teams %d\n",(*num_teams));
    if (((*num_teams) & ((*num_teams) - 1)) != 0 || (*num_teams) > MAX_TEAMS)
    {
        printf("Number of teams must be a power of 2 and less than %d\n", MAX_TEAMS);
        exit(EXIT_FAILURE);
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
void *simulate_match(void *ma)
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
void play_match(Match match){ //fonction qui permet de
    int res = -1;
    int valid = 0;
    int sleep = 1000000; //duree entre
    int action;
    int mode ; //Choisir entre soit de simuler un match, deroulement en temps reel avec possibilite d'interruption OU choisir un score

    printf("Match %s VS %s [TOUR %d]\n",team_names[match->team1],team_names[match->team2],match->tour);
    do {
        printf("Choisir le mode de jeu : [1]:Simuler | [2]:Choisir un score \n");
        scanf("%d",&mode);
    }while(mode != 1 && mode != 2);

    if(mode==1){
        printf("DEBUT %s %d - %d %s [TOUR %d]\n",team_names[match->team1],match->score1, match->score2,team_names[match->team2],match->tour);
        for (int duration = 0; duration <= 90; duration++) {
            usleep(sleep);
            // Check for input
            fd_set set;
            FD_ZERO(&set);
            FD_SET(STDIN_FILENO, &set);
            struct timeval timeout = {0, 0};
            int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
            if (rv == 1) {
                getchar();
                printf("Que voulez-vous faire ? (0: Accelerer le match, 1: L'equipe 1 marque, 2: L'equipe 2 marque) ");
                duration++;
                valid=0;
                res=-1;
                while(valid==0){
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    scanf("%d",&res);
                    if(res==0){
                        sleep = 0;
                        valid = 1;
                    }else if(res==1){
                        match->score1++;
                        printf("(%d') %.20s %d - %d %-20s\n",duration, team_names[match->team1], match->score1, match->score2,
                               team_names[match->team2]);
                        valid = 1;
                    }else if(res==2){
                        match->score2++;
                        printf("(%d') %.20s %d - %d %-20s\n",duration, team_names[match->team1], match->score1, match->score2,
                               team_names[match->team2]);
                        valid = 1;
                    }else{printf("Veuillez choisir entre 0, 1 et 2\n");}
                }
            }else{
                action = rand() % 100; // simulate random action
                if (action < 98) { // 98% de chance de ne pas marquer
                    //duration++;
                    printf("(%d')\n", duration);
                } else if (action == 99) { // 1% de chance de marquer pour l'equipe 1
                    match->score1++;
                    //duration++;
                    printf("(%d') %.20s %d - %d %-20s\n", duration, team_names[match->team1], match->score1, match->score2,
                           team_names[match->team2]);
                } else //action == 100
                { // 1% de chance de marquer pour l'equipe 2
                    match->score2++;
                    //duration++;
                    printf("(%d') %.20s %d - %d %-20s\n", duration, team_names[match->team1], match->score1, match->score2,
                           team_names[match->team2]);
                }
            }
        }

        int nTab = 5;
        while (match->score1 == match->score2) { //SI MATCH NUL, SEANCE DE TAB
            int tab;
            while (nTab > 0) {
                tab = rand() % 100;
                if (tab > 20) { //80% de chance de marquer
                    match->score1++;
                    printf("%.20s (%d) - (%d) %-20s\n", team_names[match->team1], match->score1, match->score2,
                           team_names[match->team2]);
                }
                tab = rand() % 100;
                if (tab < 40) {//60% de chance de marquer
                    match->score2++;
                    printf("%.20s (%d) - (%d) %-20s\n", team_names[match->team1], match->score1, match->score2,
                           team_names[match->team2]);
                }
                nTab--;
            }
            nTab = 1;
        }
    }

    if(mode==2){
        int score1 = 0, score2 = 0;
        do {
            fflush(stdin);
            printf("Score %s: ",team_names[match->team1]);
            scanf("%d",&score1);
            fflush(stdin);
            printf("Score %s: ",team_names[match->team2]);
            scanf("%d",&score2);
            fflush(stdin);
            if(score1 == score2){
                printf("Veuillez choisir un score non nul \n");
            }
        }while(score1 <= 0 || score2 <= 0 || score1==score2);
        match->score1 = score1;
        match->score2 = score2;
    }

    if(match->score1 > match->score2){
        teams_remaining[match->team1] = match->tour+1;
        teams_remaining[match->team2] = -1;
        printf("FIN %s* %d - %d %s\n",team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
    }
    else{
        teams_remaining[match->team2] = match->tour+1;
        teams_remaining[match->team1] = -1;
        printf("FIN %s %d - %d %s*\n",team_names[match->team1], match->score1, match->score2, team_names[match->team2]);
    }
}
void enregistrer_matchs(char **team_names, Match *matchs, int num_match) {
    // Ouverture du fichier en mode écriture
    FILE *fp = fopen("matchs.txt", "w");
    if (fp == NULL) {
        printf("Erreur d'ouverture du fichier.");
        return;
    }

    // Écriture des informations de chaque match dans le fichier texte
    for (int i = 0; i < num_match; i++) {
        fprintf(fp, "Match %d : %s [%d] : [%d] %s | Tour %d\n", i + 1, team_names[matchs[i]->team1], matchs[i]->score1, matchs[i]->score2, team_names[matchs[i]->team2], matchs[i]->tour);
    }

    fclose(fp); // Fermeture du fichier
}

