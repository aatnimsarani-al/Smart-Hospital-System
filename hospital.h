#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

// Function Declarations
void loadBedsFromFile(void);
void saveBedsToFile(void);
void logPatientRecord(int index);

void registerPatient(void);
void displayBedMatrix(void);
void displayPriorityQueue(void);
void generateAnalyticsReport(void);

#endif
