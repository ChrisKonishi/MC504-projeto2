#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#include "monks.h"

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
    Writer_args w_args;
    Deleter_args d_args;
    int *searcherCount = malloc(sizeof(int*));
    int *writerCount = malloc(sizeof(int*));

    *searcherCount = 0;
    *writerCount = 0;

    for (int i = 0; i < 5; i++) {
      s_args[i].list = list;
      s_args[i].searcherCount = searcherCount;
      s_args[i].index = i;
    }

    w_args.list = list;
    w_args.writerCount = writerCount;
    w_args.text = "ESCRITA";

    d_args.list = list;
    d_args.index = 4;

    pthread_create(&thr[0], NULL, searcher, (void*) &s_args[4]);
    sleep(1);
    pthread_create(&thr[1], NULL, searcher, (void*) &s_args[1]);
    pthread_create(&thr[2], NULL, searcher, (void*) &s_args[2]);
    pthread_create(&thr[3], NULL, writer, (void*) &w_args);
    pthread_create(&thr[4], NULL, writer, (void*) &w_args);
    pthread_create(&thr[5], NULL, deleter, (void*) &d_args);
    sleep(3);
    pthread_create(&thr[6], NULL, searcher, (void*) &s_args[4]);
    pthread_create(&thr[7], NULL, searcher, (void*) &s_args[4]);

    for (int i=0; i<8; i++)
      pthread_join(thr[i], NULL);

    print_list(list);
    del_list(list);

    return 0;
}
