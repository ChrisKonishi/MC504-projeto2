#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include "linked_list.h"

extern int run;
extern int x_pos[8], y_pos[8];
extern sem_t noSearcher, searcherSwitchMutex, noWriter, writerMutex, writerSwitchMutex, turnStile;

/* Thread arguments structs */

typedef struct Searcher_args {
    LinkedList *list;
    int *searcherCount, index;
    int id;
} Searcher_args;

typedef struct Writer_args {
    LinkedList *list;
    char *text;
    int *writerCount;
    int id;
} Writer_args;

typedef struct Deleter_args {
    LinkedList *list;
    int index;
} Deleter_args;


void *searcher(void *args);
void *writer(void *args);
void *deleter(void *args);


/* searcher
    Arguments:
        list: linked list;
        index: item to be searched;
        searcherCount: number of running searchers
*/
void *searcher(void *args) {
    while(run){

    }
    Searcher_args *arg = (Searcher_args*) args;
    LinkedList *list = arg->list;
    int *searcherCount = arg->searcherCount;
    int index = arg->index;
    int id = arg->id;

    /* turnStile is used to avoid deleter starvation: searcher and writers will wait once a deleter is queued */
    sem_wait(&turnStile);
    sem_post(&turnStile);

    /* If there is at least one active seacher, locks the noSearcher semaphore (used by the deleter) */
    sem_wait(&searcherSwitchMutex);
        *searcherCount = *searcherCount + 1;
        //printf("Entrando: %d\n", *searcherCount);
        y_pos[id] = 9;
        if (*searcherCount == 1) {
          y_pos[id] = 8;
          sem_wait(&noSearcher);
          //printf("Travou noSearcher\n");
        }
    sem_post(&searcherSwitchMutex);

    /* Critical session */
    sleep(5);
    char *text = get_item(list, index);
    //printf("Palavra revisada: %s\n", text);
    y_pos[id] = 7;

    sem_wait(&searcherSwitchMutex);
        *searcherCount = *searcherCount - 1;
        //printf("Saindo: %d\n", *searcherCount);
        y_pos[id] = 8;
        if (*searcherCount == 0) {
          //printf("Destravou noSearcher\n");
          y_pos[id] = 7;
          sleep(1);
          sem_post(&noSearcher);
        }
    y_pos[id] = 10;
    sem_post(&searcherSwitchMutex);


}

/* writer
    Arguments:
        list: linked list;
        text: text to be appended to the list;
        writerCount: number of running writers
*/
void *writer(void *args) {
    while(run){

    }
    Writer_args *arg = (Writer_args*) args;
    LinkedList *list = arg->list;
    int *writerCount = arg->writerCount;
    char *text = arg->text;
    int id = arg->id;

    sem_wait(&turnStile);
    sem_post(&turnStile);

    sem_wait(&writerSwitchMutex);
        *writerCount = *writerCount + 1;
        if (*writerCount == 1) {
          sem_wait(&noWriter);
        }
    sem_post(&writerSwitchMutex);
    sem_wait(&writerMutex);

    /* Critical session */
    //printf("Writer entrando\n");
    y_pos[id] = 9;
    append(list, text);
    sleep(1);
    //printf("Palavra escrita: %s\n", text);
    y_pos[id] = 7;
    sleep(1);

    sem_post(&writerMutex);
    sem_wait(&writerSwitchMutex);
        *writerCount = *writerCount - 1;
        if (*writerCount == 0) {
          sem_post(&noWriter);
        }
    y_pos[id] = 9;
    sleep(1);
    sem_post(&writerSwitchMutex);
    //printf("Writer saindo\n");
    y_pos[id] = 10;

}

/* deleter
    Arguments:
        list: linked list;
        index: list index to be deleted
*/
void *deleter(void *args) {
    while(run){

    }
    Deleter_args *arg = (Deleter_args*) args;
    LinkedList *list = arg->list;
    int index = arg->index;
    int id = 7;

    sem_wait(&turnStile);
    //printf("Esperando sinal\n");

    sem_wait(&noWriter);
    sem_wait(&noSearcher);

    char *text = get_item(list, index);
    //printf("Deleter entrou\n");
    y_pos[id] = 9;
    sleep(1);
    /* Critical session */
    //printf("Palavra deletada: %s\n", text);
    y_pos[id] = 7;
    del_item(list, index);
    sleep(1);
    //printf("Deleter saindo\n");
    y_pos[id] = 9;
    sleep(1);
    sem_post(&noSearcher);
    sem_post(&noWriter);
    sem_post(&turnStile);

    y_pos[id] = 10;
    sleep(1);
}
