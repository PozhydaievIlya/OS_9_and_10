#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE 1024

sem_t conductor_semaphore;
sem_t consumer_semaphore;
sem_t complite_semaphore;

int *arr, curN = 0, n, min, max;
char work_area[SIZE];

void *consumer_thread(void *arg) {
    while (1) {
        sleep(2);
        semaphore_wait(&conductor_semaphore);
        semaphore_wait(&complite_semaphore);

        arr[curN] = 0;
        curN--;

        semaphore_post(&complite_semaphore);
        semaphore_post(&consumer_semaphore);
        
        int i;
        for (i = 0; i < curN; i++) {
            fprintf(stderr, "%d ", arr[i]);
        }

        fprintf(stderr, " - Consume\n");
    }

    pthread_exit(NULL);
}

void *conductor_thread(void *arg) {
    while (1) {
        sleep(1);
        int numb = rand() % (max + min) - min; 

        semaphore_wait(&consumer_semaphore);
        semaphore_wait(&complite_semaphore);

        arr[curN] = numb;
        curN++;

        semaphore_post(&conductor_semaphore);
        semaphore_post(&complite_semaphore);

        int i;
        for (i = 0; i < curN; i++) {
            fprintf(stderr, "%d ", arr[i]);
        }

        fprintf(stderr, " - Conduct\n");
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 5) 
	{
		fprintf(stderr, "Too few arguments\n");
		return 1;
	}
    int seconds = atoi(argv[1]);
    n = atoi(argv[2]); min = atoi(argv[3]); max = atoi(argv[4]);
    arr = malloc(sizeof(int) * n);

    pthread_t conductor, consumer;
    void *thread_result;

    semaphore_init(&conductor_semaphore, 1, 0);
    semaphore_init(&consumer_semaphore, 1, n);
    semaphore_init(&complite_semaphore, 1, 1);

    pthread_create(&consumer, NULL, consumer_thread, NULL);
    pthread_create(&conductor, NULL, conductor_thread, NULL);

    sleep(seconds);

    pthread_cancel(conductor);
    pthread_cancel(consumer);

    exit(EXIT_SUCCESS);
}

