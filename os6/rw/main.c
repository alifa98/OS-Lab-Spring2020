#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>

#define  NUMBER_OF_READERS 3
#define  NUMBER_OF_WRITERS 2
#define  MAXIMUM_VALUE 10

void writerCode();

void readerCode();

sem_t mutex;
sem_t writeSemaphor;

int sharedMemoryId;

int main() {

    sem_init(&mutex, 0, 1); //initialize mutex semaphore
    sem_init(&writeSemaphor, 0, 1); //initialize writer semaphore
    pid_t pid;

    sharedMemoryId = shmget(ftok("Ali", 65), sizeof(int), 0666 | IPC_CREAT);

    int *memory = (int *) shmat(sharedMemoryId, (void *) 0, 0);

    *memory = 0;



    for (int i = 0; i < NUMBER_OF_READERS; ++i) {
        pid = fork();
        if (pid == 0) {
            readerCode();
        } else if (pid == -1) {
            printf("Error in forking a process");
        }
    }

    for (int i = 0; i < NUMBER_OF_WRITERS; ++i) {
        pid = fork();
        if (pid == 0) {
            writerCode();
        } else if (pid == -1) {
            printf("Error in forking a process");
        }
    }

    int status;
    while ((pid = waitpid(-1, &status, 0)) != -1) {
        printf("Child process %d terminated\n", pid);
    }
    printf("-----End-----\n");
}


void readerCode() {

    int *memory = (int *) shmat(sharedMemoryId, (void *) 0, 0);
    int readerCount = 0;

    bool ok = true;
    while (ok) {


        sem_wait(&mutex);
        readerCount++;
        if (readerCount == 1)
            sem_wait(&writeSemaphor);
        sem_post(&mutex);

        printf("Reader read %d from the shared memory, pid= %d\n", *memory, getpid());
        if (*memory >= MAXIMUM_VALUE)
            ok = false;


        sem_wait(&mutex);
        readerCount--;
        if (readerCount == 0)
            sem_post(&writeSemaphor);
        sem_post(&mutex);

    }
    exit(0); // important: to avoid running after readerCode() call codes in child;

}

void writerCode() {
    int *memory = (int *) shmat(sharedMemoryId, (void *) 0, 0);

    bool ok = true;
    while (ok) {
        sem_wait(&writeSemaphor);
        *memory += 1;
        printf("Writer write %d in to the shared memory, pid= %d\n", *memory, getpid());
        if (*memory >= MAXIMUM_VALUE)
            ok = false;

        sem_post(&writeSemaphor);
    }
    exit(0);// important: to avoid running after readerCode() call codes in child;
}