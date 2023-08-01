#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <semaphore.h>

using namespace std;

int main(int argc, char **argv){
    int flags;
    sem_t *sem;
    unsigned int value;

    flags = O_CREAT | O_RDWR;
    value = atoi(argv[2]);

    sem = sem_open(argv[1], flags, 0777, value);
    sem_getvalue(sem, (int *)&value);
    printf("semaphore value = %d\n", value);
    sem_close(sem);
    exit(0);
}