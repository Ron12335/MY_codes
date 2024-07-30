#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "database.h"

/**
 * It processes data items added to a shared buffer by producer threads.
 * @param data 
 * @return NULL after completion.
 */
void *consumer_function(void *data) {
    Consumer_data_t *consumer_data = (Consumer_data_t *)data;
    // Access the shared buffer from the thread data.
    buffer_t *buffer = consumer_data->buffer;
    // Store the target age and birthday for searching.
    int n = consumer_data->n;
    int target_day = consumer_data->target_day;
    int target_month = consumer_data->target_month;

    // Continuously process data until a matching entry is found or the buffer is empty and no producers are active.
    while (1) {
        // Lock the buffer mutex to safely access the buffer.
        pthread_mutex_lock(&buffer->mutex);

        // If the buffer is empty, wait for producers to add more items or for all producers to finish.
        while (buffer->count == 0) {
            // Lock the mutex to check the state of producer threads.
            pthread_mutex_lock(&active_producers_mutex);
            bool noProducersLeft = (active_producers == 0);
            // Unlock the mutex once the check is done.
            pthread_mutex_unlock(&active_producers_mutex);

            // If there are no active producers and the buffer is empty, exit the thread.
            if (noProducersLeft) {
                pthread_mutex_unlock(&buffer->mutex);
                return NULL;
            }

            // Wait for the buffer to become non-empty.
            pthread_cond_wait(&buffer->not_empty, &buffer->mutex);
        }

        // Retrieve the next item from the buffer.
        buffer_item_t item = buffer->items[buffer->head];
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        buffer->count--;
        // Signal to any waiting producers that there is space in the buffer.
        pthread_cond_signal(&buffer->not_full);
        // Unlock the buffer mutex.
        pthread_mutex_unlock(&buffer->mutex);

        // Lock the mutex to check if a matching entry has been found.
        pthread_mutex_lock(&found_flag_mutex);
        if (found_flag) {
            // If a match has been found, unlock the mutex, free the current item, and exit the thread.
            pthread_mutex_unlock(&found_flag_mutex);
            free(item.Data_entry);
            return NULL;
        }
        // Unlock the found flag mutex.
        pthread_mutex_unlock(&found_flag_mutex);

        // Process the data 
        calculate_average_salary(item.Data_entry->salary);
        calculate_month_histogram(item.Data_entry->date_of_birth);

        // Check if the current data item matches the search criteria.
        if (search_for_person(item.Data_entry, n, target_day, target_month)) {
            // If a match is found, set the found flag and copy the data to the found entry.
            pthread_mutex_lock(&found_flag_mutex);
            found_flag = 1;
            found_entry = *item.Data_entry;
            // Unlock the mutex after updating the found entry.
            pthread_mutex_unlock(&found_flag_mutex);
            // Free the memory allocated for the current data item.
            free(item.Data_entry);
            // Break out of the loop as the search is complete.
            break;
        }
        // If not a match, free the memory allocated for the current data item.
        free(item.Data_entry);
    }
    // Return NULL to indicate the thread has finished execution.
    return NULL;
}
