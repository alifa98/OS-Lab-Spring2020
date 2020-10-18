#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define PHILOSOPHER_NUMBER 5

enum philosopherState {
    THINKING, HUNGRY, EATING
};

int state[PHILOSOPHER_NUMBER];
int phil[PHILOSOPHER_NUMBER] = {0, 1, 2, 3, 4};

sem_t mutex;
sem_t ph[PHILOSOPHER_NUMBER];

int getLeftPhilosopherNumber(int philosopherNumber) {
    return (philosopherNumber + 1) % PHILOSOPHER_NUMBER;
}

int getRightPhilosopherNumber(int philosopherNumber) {
    return ((philosopherNumber - 1) < 0) ? PHILOSOPHER_NUMBER - 1 : (philosopherNumber - 1);
}

void test(int philosopherNumber) {
    if (state[philosopherNumber] == HUNGRY
        && state[getLeftPhilosopherNumber(philosopherNumber)] != EATING
        && state[getRightPhilosopherNumber(philosopherNumber)] != EATING) {

        state[philosopherNumber] = EATING;

        sleep(1);

        printf("Philosopher %d is in Eating state.\n", philosopherNumber);

        sem_post(&ph[philosopherNumber]);
    }
}


void take_forks(int philosopherNumber) {

    sem_wait(&mutex);


    state[philosopherNumber] = HUNGRY;

    printf("Philosopher %d is now in Hungry state.\n", philosopherNumber);

    test(philosopherNumber);

    sem_post(&mutex);

    sem_wait(&ph[philosopherNumber]);

    sleep(1);
}


void put_down_forks(int philosopherNumber) {

    sem_wait(&mutex);

    state[philosopherNumber] = THINKING;

    printf("Philosopher %d is in Thinking state.\n", philosopherNumber);

    test(getLeftPhilosopherNumber(philosopherNumber));
    test(getRightPhilosopherNumber(philosopherNumber));

    sem_post(&mutex);
}

void *philospher(void *num) {

    while (1) {

        int *i = num;
        sleep(1);
        take_forks(*i);
        sleep(0);
        put_down_forks(*i);
    }
}

int main() {

    int i;
    pthread_t thread_id[PHILOSOPHER_NUMBER];

    sem_init(&mutex, 0, 1);

    for (i = 0; i < PHILOSOPHER_NUMBER; ++i)

        sem_init(&ph[i], 0, 0);

    for (i = 0; i < PHILOSOPHER_NUMBER; ++i) {

        pthread_create(&thread_id[i], NULL, philospher, &phil[i]);

        printf("Philosopher %d is in Thinking state.\n", i + 1);
    }

    for (i = 0; i < PHILOSOPHER_NUMBER; ++i)
        pthread_join(thread_id[i], NULL);
}
