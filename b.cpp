#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

int main() {
    sem_t *sem;
    int counter = 0;

    sem = sem_open("/aos", O_CREAT, 0777, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    while (counter <= 10) {
        sem_wait(sem);
        printf("Pid = pid1, counter = %d\n", counter);
        counter++;
        sem_post(sem);
        sleep(1);
    }

    sem_close(sem);
    sem_unlink("/aos");
    return 0;
}
