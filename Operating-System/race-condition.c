#include <stdio.h>
#include <pthread.h>

int shared_counter = 0;

void* increment_counter(void* arg) {
    for (int i = 0; i < 1000000; i++) {
        shared_counter++;
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
