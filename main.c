#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>

#include "monks.h"

#define DELAY 100000

int y_state[MAX_THREAD];
int x_pos[MAX_THREAD];
int y_pos[MAX_THREAD];
int run = 1;

sem_t noSearcher, searcherSwitchMutex, noWriter, writerMutex, writerSwitchMutex, turnStile;

int main() {
    sem_init(&noSearcher, 0, 1);
    sem_init(&searcherSwitchMutex, 0, 1);
    sem_init(&noWriter, 0, 1);
    sem_init(&writerMutex, 0, 1);
    sem_init(&writerSwitchMutex, 0, 1);
    sem_init(&turnStile, 0, 1);

    for (int i = 0; i < MAX_THREAD; i++){
        y_state[i] = 0;
    }
    LinkedList *list = create_list();
    append(list, "Isso");
    append(list, "eh");
    append(list, "um");
    append(list, "teste");
    append(list, "ERRO");

    pthread_t thr[MAX_THREAD];
    /*Searcher_args s_args[5];
    Writer_args w_args[4];
    Deleter_args d_args[4];*/
    General_args  g_args[MAX_THREAD];
    int *searcherCount = malloc(sizeof(int*));
    int *writerCount = malloc(sizeof(int*));
    // Change two arrays bellow to change program entry
    char types[MAX_THREAD] = {'S','S','S','W','W','D','D','S','S','W','W','D','D'};
    int indexes[MAX_THREAD] = {4,1,2,-1,-1,0,0,4,4,-1,-1,0,0};
    *searcherCount = 0;
    *writerCount = 0;

    for (int i = 0; i < MAX_THREAD; i++) {
        if (types[i] == 'S'){
            g_args[i].list = list;
            g_args[i].searcherCount = searcherCount;
            g_args[i].writerCount = NULL;
            g_args[i].text = "";
            g_args[i].index = indexes[i];
        } else if (types[i] == 'W') {
            g_args[i].list = list;
            g_args[i].writerCount = writerCount;
            g_args[i].text = "ESCRITA";
            g_args[i].index = -1;
        } else {
            g_args[i].list = list;
            g_args[i].searcherCount = NULL;
            g_args[i].writerCount = NULL;
            g_args[i].text = "";
            g_args[i].index = indexes[i];
        }
        g_args[i].id = i;
        g_args[i].type = types[i];
        y_pos[i] = 10;
        x_pos[i] = i + 20;

      //s_args[i].id = i;
    }

    pthread_create(&thr[0], NULL, monk, (void *)&g_args[0]);
    sleep(1);
    for (int i=1; i<MAX_THREAD; i++){
        pthread_create(&thr[i], NULL, monk, (void *)&g_args[i]);
        if (i == 6){
            sleep(3);
        }
    }
    
    // begin game
    char *string = (char *)malloc(2 * sizeof(char));
    initscr();
    noecho();
    curs_set(FALSE);
    while(1){
    	clear();
        mvprintw(1, 0, "Aperte ctrl+c para parar");
        mvprintw(7, 0, "Lendo");
        mvprintw(8, 0, "Travando/Liberando");
        mvprintw(9, 0, "Entrando/Saindo");
        mvprintw(10, 0, "Esperando");
        for (int j=0; j<MAX_THREAD; j++){
            string[0] = g_args[j].type;
            string[1] = '\0';
            mvprintw(y_pos[j], x_pos[j] + (2 * j), string);
        }
        run = 0;
    	refresh();

     	usleep(DELAY);
 	}
    free(string);
    endwin();
    // end game

    for (int i=0; i<8; i++)
      pthread_join(thr[i], NULL);

    print_list(list);
    del_list(list);

    return 0;
}
