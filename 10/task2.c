#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024
char work_area[SIZE];

int *arr, currentN = 0, n, min, max, thread_flag;
pthread_mutex_t thread_flag_mutex;


void *consumer_thread(void *arg) {
    while (1) {
        int flag;
        pthread_mutex_lock(&thread_flag_mutex);
        flag = thread_flag;
        pthread_mutex_unlock(&thread_flag_mutex);

        if (flag) {
            arr[currentN] = 0;
            currentN--;

            int i;
            for (i = 0; i < currentN; i++) {
                fprintf(stderr, "%d ", arr[i]);
            }

            fprintf(stderr, " - Consume\n");

            set_flag(0);
        }
    }

    pthread_exit(NULL);
}

void *conductor_thread(void *arg) {
    while (1) {
        sleep(2);
        int numb = rand() % (max + min) - min; 

        arr[currentN] = numb;
        currentN++;

        int i;
        for (i = 0; i < currentN; i++) {
            fprintf(stderr, "%d ", arr[i]);
        }

        fprintf(stderr, " - Conduct\n");

        set_flag(1);
    }

    pthread_exit(NULL);
}

void set_flag (int flag_value) {
    pthread_mutex_lock (&thread_flag_mutex);
    thread_flag = flag_value;
    pthread_mutex_unlock (&thread_flag_mutex);
}

void init_flag () {
    pthread_mutex_init (&thread_flag_mutex, NULL);
    thread_flag = 0;
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

    init_flag();

    pthread_create(&consumer, NULL, consumer_thread, NULL);
    pthread_create(&conductor, NULL, conductor_thread, NULL);

    sleep(seconds);

    pthread_cancel(conductor);
    pthread_cancel(consumer);

    exit(EXIT_SUCCESS);
}
