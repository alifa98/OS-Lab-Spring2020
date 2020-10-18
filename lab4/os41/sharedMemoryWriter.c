#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main()
{
    key_t key = ftok("sharedAli", 65); //generate unique key

    int shared_memory_id = shmget(key, 1024, 0666 | IPC_CREAT);
    printf("Shared Memory Created.\n");

    char *shared_mem = (char *)shmat(shared_memory_id, 0, 0);

    sprintf(shared_mem, "Shreeeeeeeeed MEM.");
    printf("Witing Done.\n");

    shmdt(shared_mem);

    return 0;
}