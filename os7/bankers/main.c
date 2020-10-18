//Bankers Algorithm
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define NUMBER_OF_RESOURCES 5
#define NUMBER_OF_CUSTOMERS 5

void *customer(void *id);

int captureResource(int customerId, int resources[]);

void releaseResource(int customerId, int resources[]);

int totalResources[NUMBER_OF_RESOURCES];
int available[NUMBER_OF_RESOURCES];
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
int customersNumber[NUMBER_OF_CUSTOMERS] = {0, 1, 2, 3, 4};
sem_t mutex;
bool runAllThreads = false; // this flag change to true after all threads declared their needs and writes values in need matrices

void printMatrices() {
    printf("Available: [");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        printf("%d ", available[i]);
    }
    printf("]\n");

    printf("Maximum:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        printf("%d:", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            printf(" %d", maximum[i][j]);
        }
        printf("\n");
    }

    printf("Allocation:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        printf("%d:", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            printf(" %d", allocation[i][j]);
        }
        printf("\n");
    }

    printf("Need:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        printf("%d:", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            printf(" %d", need[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {

    if (argc <= NUMBER_OF_RESOURCES) {
        printf("Not enough arguments\n");
        return EXIT_FAILURE;
    }

    for (int index = 0; index < NUMBER_OF_RESOURCES; ++index)
        totalResources[index] = available[index] = strtol(argv[index + 1], NULL, 10);

    printf("available[ ");
    for (int index = 0; index < NUMBER_OF_RESOURCES; ++index)
        printf("%d ", available[index]);
    printf("]\n");

    srand(time(NULL));

    pthread_t thread_id[NUMBER_OF_CUSTOMERS];
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        pthread_create(&thread_id[i], NULL, customer, &customersNumber[i]);
    }
    sleep(1);
    runAllThreads = true;

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
        pthread_join(thread_id[i], NULL);

    return EXIT_SUCCESS;
}


void *customer(void *customerId) {

    int *id = customerId;

    sem_wait(&mutex);
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; ++resource) {
        need[*id][resource] = maximum[*id][resource] = (rand() % (totalResources[resource] + 1));
    }
    sem_post(&mutex);

    int resourceRequest[NUMBER_OF_RESOURCES];
    while (1) {
        if (runAllThreads) {

            sem_wait(&mutex);
            for (int resource = 0; resource < NUMBER_OF_RESOURCES; ++resource)
                resourceRequest[resource] = (rand() % (need[*id][resource] + 1));
            sem_post(&mutex);

            if (captureResource(*id, resourceRequest) == 0) {
                printf("Allocated to %d\n\n", *id);
                sleep(rand() % 10);
                releaseResource(*id, resourceRequest);
            } else {
                printf("Did not allocated to %d.\n\n", *id);
                sleep(1);
            }

        }
    }

}

void releaseResource(int customerId, int requestedResources[]) {
    sem_wait(&mutex);
    for (int resource = 0; resource < NUMBER_OF_RESOURCES; ++resource) {
        available[resource] += requestedResources[resource];
        allocation[customerId][resource] -= requestedResources[resource];
        need[customerId][resource] += requestedResources[resource];
    }

    printf("Release: \n");
    printMatrices();

    sem_post(&mutex);
}

int captureResource(int customerId, int requestedResources[]) {

    printf("\nResource Request from %d is [ ", customerId);
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        printf("%d ", requestedResources[i]);
    }
    printf("]\n");

    sem_wait(&mutex);

    for (int resource = 0; resource < NUMBER_OF_RESOURCES; ++resource) {
        available[resource] -= requestedResources[resource];
        allocation[customerId][resource] += requestedResources[resource];
        need[customerId][resource] -= requestedResources[resource];
    }


    int allocationStatus = -1;

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        bool rowIsOk = true;
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            if (need[i][j] > available[j])
                rowIsOk = false;
        }
        if (rowIsOk) {
            allocationStatus = 0;
            break;
        }
    }

    if (allocationStatus == -1) {
        for (int resource = 0; resource < NUMBER_OF_RESOURCES; ++resource) {
            available[resource] += requestedResources[resource];
            allocation[customerId][resource] -= requestedResources[resource];
            need[customerId][resource] += requestedResources[resource];
        }
    } else {
        printf("Capture: \n");
        printMatrices();
    }

    sem_post(&mutex);
    return allocationStatus;
}

