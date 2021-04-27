#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int main() {

    sem_t noSearcher, searcherSwitch, noWriter, writerMutex, writerSwitchMutex;

    sem_init(&noSearcher, 0, 1);
    sem_init(&searcherSwitch, 0, 1);
    sem_init(&noWriter, 0, 1);
    sem_init(&writerMutex, 0, 1);
    sem_init(&writerSwitchMutex, 0, 1);

    return 0;
}