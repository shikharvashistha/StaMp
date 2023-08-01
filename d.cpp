#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <future>


using namespace std;

sem_t *sem;

void second(){
    int counter = 0;

    sem = sem_open("/aos", O_CREAT, 0777, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    while (counter <= 10) {
        sem_wait(sem);
        printf("Pid = pid2, counter = %d\n", counter);
        counter++;
        sem_post(sem);
        sleep(1);
    }

    sem_close(sem);
    sem_unlink("/aos");
}

void first(){
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
}

int main(){
    auto a=async(launch::async, first);
    auto b=async(launch::async, second);

    a.get();
    b.get();

    return 0;
}