//database.h
#ifndef DATABASE_H
#define DATABASE_H

#include <pthread.h>

#define PRODUCER_COUNT 2
#define CONSUMER_COUNT 4
#define BUFFER_SIZE 1024
#define DATABASE_DIR "database"
#define DATABASE_SIZE 514

// Structure Definitions
typedef struct Data_entry {
    char name[128];
    char address[256];
    char phone[16];
    char date_of_birth[12];
    char occupation[64];
    int salary;
} Data_entry_t;

typedef struct buffer_item {
    Data_entry_t *Data_entry;
} buffer_item_t;

typedef struct buffer {
    buffer_item_t items[BUFFER_SIZE];
    int head;
    int tail;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
    pthread_cond_t found;
} buffer_t;

typedef struct thread_data {
    int producer_id;
    buffer_t *buffer;
    pthread_t thread_id;
} thread_data_t;

typedef struct Consumer_data {
    buffer_t *buffer;
    int n;
    int target_day;
    int target_month;
} Consumer_data_t;

// Global Variables
extern pthread_mutex_t active_producers_mutex;
extern pthread_cond_t all_producers_done_cond;
extern int active_producers;

extern pthread_mutex_t salary_mutex;
extern int total_salary;
extern int salary_entries;

extern pthread_mutex_t histogram_mutex;
extern int month_histogram[12];

extern pthread_mutex_t found_flag_mutex;
extern int found_flag;
extern Data_entry_t found_entry;

// Function Declarations
int search_for_person(Data_entry_t *entry, int n, int target_day, int target_month);
void calculate_average_salary(int salary);
void calculate_month_histogram(char *date_of_birth);
void *producer_function(void *data);
void *consumer_function(void *data);

#endif 
