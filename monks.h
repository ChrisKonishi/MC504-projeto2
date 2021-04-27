#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

#include "linked_list.h"

typedef struct Searcher_args {
    LinkedList list;
    sem_t noSearcher, searcherSwitch;
    int *searcher_count;
} Searcher_args;

typedef struct Writer_args {
    LinkedList list;
    sem_t noWriter, writerMutex, writerSwitchMutex;
    char *text;
} Writer_args;

typedef struct Deleter_args {
    LinkedList list;
    sem_t noSearcher, noWriter;
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
*/
void *searcher(void *args) {
    Searcher_args *args = (Searcher_args*) args;
    LinkedList list = args->list;
    sem_t noSearcher = args->noSearcher;
    sem_t searcherSwitchMutex = args->searcherSwitchMutex;
    int *searcherCount = args->searcherCount;

    /* If there is at least one active seacher, locks the noSearcher semaphore (used by the deleter) */
    sem_wait(&searcherSwitchMutex);
        *searcherCount += 1;
        if *searcherCount == 1
            sem_wait(&noSearcher);
    sem_post(&searcherSwitchMutex);

    /* Critical session */

    sem_wait(&searcherSwitchMutex);
        *searcherCount -= 1;
        if *searcherCount == 0 
            sem_post(&noSearcher);
    sem_post(&searcherSwitchMutex);

    return;
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
    Writer_args *args = (Writer_args*) args;
    LinkedList list = args->list;
    sem_t noWriter = args->noWriter;
    sem_t writerMutex = args->writerMutex;
    sem_t writerSwitchMutex = args->writerSwitchMutex;
    char *text = args->text;

    sem_wait(&writerSwitchMutex);
        *writerCount += 1;
        if *writerCount == 1
            sem_wait(&noSearcher);
    sem_post(&writerSwitchMutex);
    sem_wait(&writerMutex);

    /* Critical session */
    append(list, text);

    sem_post(&writerMutex);
    sem_wait(&writerSwitchMutex);
        *writerCount -= 1;
        if *writerCount == 0 
            sem_post(&noSearcher);
    sem_post(&writerSwitchMutex);   

    return;
}

/* deleter
    Arguments: 
        list: linked list;
        noWriter: writer mutex;
        noSearcher: searcher mutex;
        index: list index to be deleted;
*/
void *deleter(void *args) {
    Deleter_args *args = (Deleter_args*) args;
    LinkedList list = args->list;
    sem_t noWriter = args->noWriter;
    sem_t noSearcher = args->noSearcher;
    int index = args->index;

    /* If there is at least one active seacher, locks the noSearcher semaphore (used by the deleter) */
    sem_wait(&noWriter);
    sem_wait(&noSearcher);

    /* Critical session */
    del_item(list, index);

    sem_post(&noSearcher);
    sem_post(&noWriter);

    return;
}
