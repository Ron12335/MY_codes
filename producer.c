#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "database.h"

/**
 * It reads data from files and adds it to a shared buffer for consumer threads to process.
 * @param data
 * @return NULL after completion.
 */
void *producer_function(void *data) {
    thread_data_t *thread_data = (thread_data_t *)data;
    // Access the shared buffer from the thread data.
    buffer_t *buffer = thread_data->buffer;
    // Buffer for reading lines from the file.
    char line[256];

    // Loop through files assigned to this producer based on its ID.
    for (int file_index = thread_data->producer_id; file_index < DATABASE_SIZE; file_index += PRODUCER_COUNT) {
     
        pthread_mutex_lock(&found_flag_mutex);
        bool shouldTerminate = found_flag;
        // Unlock the mutex after reading the flag.
        pthread_mutex_unlock(&found_flag_mutex);
        if (shouldTerminate) {
            break;
        }

        // Create the file name based on the current index.
        char filename[128];
        sprintf(filename, "%s/%03d.dat", DATABASE_DIR, file_index);
        // Open the file for reading.
        FILE *fp = fopen(filename, "r");
        // If the file doesn't open, skip to the next iteration.
        if (fp == NULL) {
            continue;
        }

        Data_entry_t Data_entry;
        // Read the data from the file into the structure, line by line.
        if (fgets(line, sizeof(line), fp) && sscanf(line, "Name: %127[^\n]", Data_entry.name) &&
            fgets(line, sizeof(line), fp) && sscanf(line, "Address: %255[^\n]", Data_entry.address) &&
            fgets(line, sizeof(line), fp) && sscanf(line, "Phone: %15[^\n]", Data_entry.phone) &&
            fgets(line, sizeof(line), fp) && sscanf(line, "Date of birth: %11[^\n]", Data_entry.date_of_birth) &&
            fgets(line, sizeof(line), fp) && sscanf(line, "Occupation: %63[^\n]", Data_entry.occupation) &&
            fgets(line, sizeof(line), fp) && sscanf(line, "Salary: %d", &Data_entry.salary)) {

            buffer_item_t item;
            item.Data_entry = malloc(sizeof(Data_entry_t));
            // If memory allocation fails, close the file and skip to the next iteration.
            if (item.Data_entry == NULL) {
                fclose(fp);
                continue;
            }

            // Copy the read data into the allocated space.
            *item.Data_entry = Data_entry;

            // Lock the buffer mutex to add the item to the buffer.
            pthread_mutex_lock(&buffer->mutex);
            // If the buffer is full, wait until there's space.
            while (buffer->count == BUFFER_SIZE) {
                pthread_cond_wait(&buffer->not_full, &buffer->mutex);
            }

            // Add the item to the buffer and update the tail and count.
            buffer->items[buffer->tail] = item;
            buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
            buffer->count++;
            // Signal that the buffer is not empty anymore.
            pthread_cond_signal(&buffer->not_empty);
            // Unlock the buffer mutex after adding the item.
            pthread_mutex_unlock(&buffer->mutex);
        }

        // Close the file after processing.
        fclose(fp);
    }

    // After all files are processed, or a termination signal is received, decrement the count of active producers.
    pthread_mutex_lock(&active_producers_mutex);
    active_producers--;
    // If this is the last producer, broadcast to all consumers that producers are done.
    if (active_producers == 0) {
        pthread_cond_broadcast(&all_producers_done_cond);
    }
    // Unlock the producers mutex.
    pthread_mutex_unlock(&active_producers_mutex);

    // Return NULL to indicate the thread has finished execution.
    return NULL;
}
