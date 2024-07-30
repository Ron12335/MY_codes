#include "database.h"
#include <stdio.h>   
#include <string.h>  
#include <limits.h>  
#include <stdbool.h>


/**
 * Determine if a year is a leap year.
 * @param year The year to check.
 * @return true if the year is a leap year, false otherwise.
 */
bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

/**
 * Calculate the number of days in a given month of a given year.
 * @param month The month (1-12).
 * @param year The year.
 * @return The number of days in the month.
 */
int daysInMonth(int month, int year) {
    switch (month) {
        case 2: return isLeapYear(year) ? 29 : 28;
        case 4: case 6: case 9: case 11: return 30;
        default: return 31;
    }
}

/**
 * Calculate and update the average salary.
 * @param salary The salary to be added to the total.
 */

void calculate_average_salary(int salary) {
    if (salary < 0) {
        printf("Invalid salary: %d. Salary must be non-negative.\n", salary);
        return; 
    }

    pthread_mutex_lock(&salary_mutex);
    if (total_salary > LLONG_MAX - salary) {
        printf("Warning: Total salary overflow. Salary addition skipped.\n");
        pthread_mutex_unlock(&salary_mutex);
        return;
    }
    total_salary += salary;
    salary_entries++;
    pthread_mutex_unlock(&salary_mutex);
}
/**
 * Update the histogram for the birth months of individuals in the database.
 * @param date_of_birth The date of birth string in the format "YYYY-MM-DD".
 */
void calculate_month_histogram(char *date_of_birth) {
    if (date_of_birth == NULL || strlen(date_of_birth) < 7) {
        printf("Invalid date of birth format.\n");
        return;
    }

    int month;
    // Check if the date format is correct and the month is successfully parsed
    if (sscanf(date_of_birth + 5, "%2d", &month) != 1) {
        printf("Failed to parse month from date of birth.\n");
        return; 
    }

    pthread_mutex_lock(&histogram_mutex);
    if (month >= 1 && month <= 12) {
        month_histogram[month - 1]++; // Increment the count for the corresponding month
    } else {
        printf("Month out of range: %d\n", month);
    }
    pthread_mutex_unlock(&histogram_mutex);
}

/**
 * Search for a person in the database who has their n-th birthday on a specific day and month.
 * @param entry A pointer to the Data_entry_t structure containing the person's details.
 * @param n The target age.
 * @param target_day The target day of the month.
 * @param target_month The target month.
 * @return 1 if a matching person is found, or else 0.
 */


int search_for_person(Data_entry_t *entry, int n, int target_day, int target_month) {
   
    if (target_month < 1 || target_month > 12 || target_day < 1 || target_day > 31) {
        return 0; 
    }

    int year, month, day;
    if (sscanf(entry->date_of_birth, "%4d-%2d-%2d", &year, &month, &day) != 3) {
        return 0; 
    }

    if (month < 1 || month > 12 || day < 1 || day > daysInMonth(month, year)) {
        return 0;
    }
    int current_year = 2023;  
    int age = current_year - year;
    if (month > target_month || (month == target_month && day > target_day)) {
        age--; 
    }

    if (age == n && day == target_day && month == target_month) {
        return 1;
    }
    return 0;
}

