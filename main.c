#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>

#include "monks.h"

#define DELAY 100000

int y_state[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int x_pos[8];
int y_pos[8];
int run = 1;

sem_t noSearcher, searcherSwitchMutex, noWriter, writerMutex, writerSwitchMutex, turnStile;

int main() {
    sem_init(&noSearcher, 0, 1);
    sem_init(&searcherSwitchMutex, 0, 1);
    sem_init(&noWriter, 0, 1);
    sem_init(&writerMutex, 0, 1);
    sem_init(&writerSwitchMutex, 0, 1);
    sem_init(&turnStile, 0, 1);

    LinkedList *list = create_list();
    append(list, "Isso");
    append(list, "eh");
    append(list, "um");
    append(list, "teste");
    append(list, "ERRO");

    pthread_t thr[8];
    Searcher_args s_args[5];
    Writer_args w_args[2];
    Deleter_args d_args;
    int *searcherCount = malloc(sizeof(int*));
    int *writerCount = malloc(sizeof(int*));

    *searcherCount = 0;
    *writerCount = 0;

    for (int i = 0; i < 5; i++) {
      s_args[i].list = list;
      s_args[i].searcherCount = searcherCount;
      s_args[i].index = i;
      s_args[i].id = i;
    }
    s_args[0].index = 4;
    s_args[1].index = 1;
    s_args[2].index = 2;
    s_args[3].index = 4;
    s_args[4].index = 4;

    for (int i = 0; i < 2; i++) {
        w_args[i].list = list;
        w_args[i].writerCount = writerCount;
        w_args[i].text = "ESCRITA";
        w_args[i].id = i + 5;
    }

    d_args.list = list;
    d_args.index = 0;
    d_args.id = 7;

    for (int i=0; i<8; i++){
        y_pos[i] = 10;
        x_pos[i] = i + 20;
    }

    pthread_create(&thr[0], NULL, searcher, (void*) &s_args[0]);
    sleep(1);
    pthread_create(&thr[1], NULL, searcher, (void*) &s_args[1]);
    pthread_create(&thr[2], NULL, searcher, (void*) &s_args[2]);
    pthread_create(&thr[3], NULL, writer, (void*) &w_args[0]);
    pthread_create(&thr[4], NULL, writer, (void*) &w_args[0]);
    pthread_create(&thr[5], NULL, deleter, (void*) &d_args);
    sleep(3);
    pthread_create(&thr[6], NULL, searcher, (void*) &s_args[3]);
    pthread_create(&thr[7], NULL, searcher, (void*) &s_args[4]);

    // begin game
    initscr();
    noecho();
    curs_set(FALSE);

    while(1){
    	clear();
        mvprintw(7, 0, "Lendo");
        mvprintw(8, 0, "Travando/Liberando");
        mvprintw(9, 0, "Entrando/Saindo");
        mvprintw(10, 0, "Esperando");
        for (int j=0; j<5; j++){
            mvprintw(y_pos[j],x_pos[j] + (2 * j), "S");
        }
        for (int j=5; j<7; j++){
            mvprintw(y_pos[j],x_pos[j] + (2 * j), "W");
        }
        mvprintw(y_pos[7],x_pos[7] + (2 * 7), "D");
        run = 0;
    	refresh();

     	usleep(DELAY);
 	}

 	endwin();
    // end game

    for (int i=0; i<8; i++)
      pthread_join(thr[i], NULL);

    print_list(list);
    del_list(list);

    return 0;
}
