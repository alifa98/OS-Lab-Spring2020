
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main()
{
    key_t key = ftok("sharedAli", 65); //generate unique key

    int shared_memory_id = shmget(key, 1024, 0666 | IPC_CREAT);

    char *shared_mem = (char *)shmat(shared_memory_id, 0, 0);

    printf("MEMORY CONTENT: %s\n", shared_mem);

    shmdt(shared_mem);

    shmctl(shared_memory_id, IPC_RMID, NULL); // destroy the shared memory

    return 0;
}
