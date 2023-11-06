#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

// Deklarasi variabel spin-lock
atomic_flag lock = ATOMIC_FLAG_INIT;

// Fungsi untuk mengunci spin-lock
void lock_spin()
{
    while (atomic_flag_test_and_set(&lock))
        ; // putar sampai lock dilepaskan
}

// Fungsi untuk membuka spin-lock
void unlock_spin()
{
    atomic_flag_clear(&lock); // lepaskan lock
}

// Fungsi yang akan dijalankan oleh thread
void *thread_func(void *arg)
{
    int id = *(int *)arg; // ambil id thread
    lock_spin(); // kunci spin-lock
    printf("Thread %d memasuki kawasan kritis\n", id);
    // Lakukan sesuatu di kawasan kritis
    printf("Thread %d meninggalkan kawasan kritis\n", id);
    unlock_spin(); // buka spin-lock
    return NULL;
}

// Fungsi utama
int main()
{
    const int N = 4; // jumlah thread
    pthread_t threads[N]; // array untuk menyimpan id thread
    int ids[N]; // array untuk menyimpan id numerik thread

    // Buat thread dan jalankan fungsi thread_func
    for (int i = 0; i < N; i++)
    {
        ids[i] = i + 1; // set id numerik thread
        pthread_create(&threads[i], NULL, thread_func, &ids[i]); // buat thread
    }

    // Tunggu semua thread selesai
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL); // tunggu thread
    }

    return 0;
}
