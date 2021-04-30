#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#include "linked_list.h"

extern sem_t noSearcher, searcherSwitchMutex, noWriter, writerMutex, writerSwitchMutex, turnStile;

/* Thread arguments structs */

typedef struct Searcher_args {
    LinkedList *list;
    int *searcherCount, index;
} Searcher_args;

typedef struct Writer_args {
    LinkedList *list;
    char *text;
    int *writerCount;
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
        noSearcher: mutex used by the deleter;
        searcherSwitchMutex: mutex used to safely manipulate searcherCount;
        searcherCount: number of running searchers
        index: item to be searched
*/
void *searcher(void *args) {
    Searcher_args *arg = (Searcher_args*) args;
    LinkedList *list = arg->list;
    int *searcherCount = arg->searcherCount;
    int index = arg->index;

    /* turnStile is used to avoid deleter starvation: searcher and writers will wait once a deleter is queued */
    sem_wait(&turnStile);
    sem_post(&turnStile);

    /* If there is at least one active seacher, locks the noSearcher semaphore (used by the deleter) */
    sem_wait(&searcherSwitchMutex);
        *searcherCount = *searcherCount + 1;
        printf("Entrando: %d\n", *searcherCount);
        if (*searcherCount == 1) {
          sem_wait(&noSearcher);
          printf("Travou noSearcher\n");
        }
    sem_post(&searcherSwitchMutex);

    /* Critical session */
    sleep(5);
    char *text = get_item(list, index);
    //printf("Palavra revisada: %s\n", text);

    sem_wait(&searcherSwitchMutex);
        *searcherCount = *searcherCount - 1;
        printf("Saindo: %d\n", *searcherCount);
        if (*searcherCount == 0) {
          printf("Destravou noSearcher\n");
          sem_post(&noSearcher);
        }
    sem_post(&searcherSwitchMutex);

}

/* writer
    Arguments:
        list: linked list;
        noWriter: mutex used by the deleter;
        writerMutex: self explanatory;
        writerSwitchMutex: mutex used to safely manipulate searcher_count;
        writerCount: number of writers running/queued;
        text: text to be appended to the list
*/
void *writer(void *args) {
    Writer_args *arg = (Writer_args*) args;
    LinkedList *list = arg->list;
    int *writerCount = arg->writerCount;
    char *text = arg->text;

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
    printf("Writer entrando\n");
    append(list, text);
    //printf("Palavra escrita: %s\n", text);

    sem_post(&writerMutex);
    sem_wait(&writerSwitchMutex);
        *writerCount = *writerCount - 1;
        if (*writerCount == 0) {
          sem_post(&noWriter);
        }
    sem_post(&writerSwitchMutex);
    printf("Writer saindo\n");

}

/* deleter
    Arguments:
        list: linked list;
        noWriter: writer mutex;
        noSearcher: searcher mutex;
        index: list index to be deleted;
*/
void *deleter(void *args) {
    Deleter_args *arg = (Deleter_args*) args;
    LinkedList *list = arg->list;
    int index = arg->index;

    sem_wait(&turnStile);
    printf("Esperando sinal\n");

    sem_wait(&noWriter);
    sem_wait(&noSearcher);

    char *text = get_item(list, index);
    printf("Deleter entrou\n");

    /* Critical session */
    //printf("Palavra deletada: %s\n", text);
    del_item(list, index);

    printf("Deleter saindo\n");

    sem_post(&noSearcher);
    sem_post(&noWriter);
    sem_post(&turnStile);


}
