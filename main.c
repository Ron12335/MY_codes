#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "database.h"

// Global Variables
pthread_mutex_t active_producers_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t all_producers_done_cond = PTHREAD_COND_INITIALIZER;
int active_producers = PRODUCER_COUNT;

pthread_mutex_t salary_mutex = PTHREAD_MUTEX_INITIALIZER;
int total_salary = 0;
int salary_entries = 0;

pthread_mutex_t histogram_mutex = PTHREAD_MUTEX_INITIALIZER;
int month_histogram[12] = {0};

pthread_mutex_t found_flag_mutex = PTHREAD_MUTEX_INITIALIZER;
int found_flag = 0;
Data_entry_t found_entry;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n DD-MM\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int target_day, target_month;
    if (sscanf(argv[2], "%2d-%2d", &target_day, &target_month) != 2) {
        fprintf(stderr, "Invalid date format. Use DD-MM.\n");
        return 1;
    }

    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];
    buffer_t buffer = { .count = 0, .head = 0, .tail = 0 };
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.not_full, NULL);
    pthread_cond_init(&buffer.not_empty, NULL);

    thread_data_t producer_data[PRODUCER_COUNT];
    Consumer_data_t consumer_data[CONSUMER_COUNT];

    // Create producer threads
    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producer_data[i].producer_id = i;
        producer_data[i].buffer = &buffer;
        pthread_create(&producers[i], NULL, producer_function, &producer_data[i]);
    }

     // Create consumer threads
    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumer_data[i].buffer = &buffer;
        consumer_data[i].n = n;
        consumer_data[i].target_day = target_day;
        consumer_data[i].target_month = target_month;
        pthread_create(&consumers[i], NULL, consumer_function, &consumer_data[i]);
    }


    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumers[i], NULL);
    }

    int average_salary = (salary_entries > 0) ? (total_salary / salary_entries) : 0;
    printf("#Average salary: %d\n", average_salary);

    printf("#Month histogram:\n");
    printf("+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    printf("| Jan | Feb | Mar | Apr | May | Jun | Jul | Aug | Sep | Oct | Nov | Dec |\n");
    printf("+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    printf("|");
    for (int i = 0; i < 12; i++) {
        printf(" %3d |", month_histogram[i]);
    }
    printf("\n");
    printf("+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+\n");

    pthread_mutex_lock(&found_flag_mutex);
    if (found_flag) {
        printf("#Found entry:\n");
        printf("Name: %s\n", found_entry.name);
        printf("Address: %s\n", found_entry.address);
        printf("Phone: %s\n", found_entry.phone);
        printf("Date of birth: %s\n", found_entry.date_of_birth);
        printf("Occupation: %s\n", found_entry.occupation);
        printf("Salary: %d\n", found_entry.salary);
    } else {
        printf("No entries found!\n");
    }
    pthread_mutex_unlock(&found_flag_mutex);

    //Destroy.
    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.not_full);
    pthread_cond_destroy(&buffer.not_empty);
    pthread_mutex_destroy(&salary_mutex);
    pthread_mutex_destroy(&histogram_mutex);
    pthread_mutex_destroy(&found_flag_mutex);

    return 0;
}
