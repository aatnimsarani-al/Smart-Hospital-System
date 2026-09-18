#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTS & MACROS ---
#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

// --- FUNCTION PROTOTYPES ---
void loadBedsFromFile(void);
void saveBedsToFile(void);
/**
 * @brief Logs the formatted billing summary of a registered patient to a file.
 * @param index The array position index of the target patient.
 */
void logPatientRecord(int index);
void logPatientRecord(int index);

int findPatientIndexByID(const char* id);
void searchPatientRecord(void);

void registerPatient(void);
void displayBedMatrix(void);
void displayPriorityQueue(void);
void generateAnalyticsReport(void);

#endif
