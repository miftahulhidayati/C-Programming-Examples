#include <stdio.h>
#include <pthread.h>

int shared_counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* increment_counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        // Lock the mutex before accessing the shared_counter
        pthread_mutex_lock(&mutex);
        shared_counter++;
        // Unlock the mutex after modifying the shared_counter
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, increment_counter, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    if (pthread_create(&thread2, NULL, increment_counter, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    if (pthread_join(thread1, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    if (pthread_join(thread2, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    printf("Final shared_counter value: %d\n", shared_counter);

    return 0;
}
