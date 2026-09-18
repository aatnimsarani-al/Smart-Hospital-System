#include "hospital.h"


int patientCount = 0;

char patientID[MAX_PATIENTS][15];
char patientName[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int patientUrgency[MAX_PATIENTS]; 
int patientSpecialty[MAX_PATIENTS];
int patientAdmitted[MAX_PATIENTS];
int patientWard[MAX_PATIENTS]; 
int patientDays[MAX_PATIENTS];
int patientBedNo[MAX_PATIENTS];


float patientBaseFee[MAX_PATIENTS];
float patientSurcharge[MAX_PATIENTS];
float patientWardCost[MAX_PATIENTS];
float patientGrossTotal[MAX_PATIENTS];
float patientDiscount[MAX_PATIENTS];
float patientFinalBill[MAX_PATIENTS];
float patientWaitTime[MAX_PATIENTS];


int specialtyQueueCounts[NUM_SPECIALTIES] = {0, 0, 0, 0};


const char* SPECIALTY_NAMES[NUM_SPECIALTIES] = {
    "General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"
};
const float BASE_FEES[NUM_SPECIALTIES] = {1500.00, 2500.00, 4500.00, 5500.00};
const int AVG_TIMES[NUM_SPECIALTIES] = {10, 25, 30, 30};
const int DAILY_CAPS[NUM_SPECIALTIES] = {30, 20, 12, 10};

const char* WARD_NAMES[NUM_WARDS] = {
    "General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"
};
const float WARD_RATES[NUM_WARDS] = {3000.00, 6000.00, 12000.00, 25000.00};
const int WARD_CAPACITIES[NUM_WARDS] = {20, 10, 10, 5};


int bedOccupancy[NUM_WARDS][20] = {0};


void loadBedsFromFile(void) {
    FILE *file = fopen("bed_status.txt", "r");
    if (file == NULL) {
        return; // File doesn't exist yet, start with default empty beds
    }
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            fscanf(file, "%d", &bedOccupancy[w][b]);
        }
    }
    fclose(file);
}

void saveBedsToFile(void) {
    FILE *file = fopen("bed_status.txt", "w");
    if (file == NULL) {
        printf("Error: Unable to save bed statuses.\n");
        return;
    }
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            fprintf(file, "%d ", bedOccupancy[w][b]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void logPatientRecord(int index) {
    FILE *file = fopen("patient_records.txt", "a");
    if (file == NULL) {
        printf("Error writing to record log.\n");
        return;
    }

    fprintf(file, "===============================================\n");
    fprintf(file, "ID: %s | Name: %s | Age: %d | Urgency: %d\n",
            patientID[index], patientName[index], patientAge[index], patientUrgency[index]);
    fprintf(file, "Specialty: %s\n", SPECIALTY_NAMES[patientSpecialty[index] - 1]);
    if (patientAdmitted[index]) {
        fprintf(file, "Ward: %s (Bed #%02d) | Days: %d\n",
                WARD_NAMES[patientWard[index] - 1], patientBedNo[index], patientDays[index]);
    } else {
        fprintf(file, "Ward: Outpatient (OPD)\n");
    }
    fprintf(file, "Gross Total: LKR %.2f | Discount: LKR %.2f | Final Bill: LKR %.2f\n",
            patientGrossTotal[index], patientDiscount[index], patientFinalBill[index]);
    fprintf(file, "===============================================\n\n");

    fclose(file);
}


void registerPatient(void) {
    if (patientCount >= MAX_PATIENTS) {
        printf("\nSystem register full! Unable to add more patients.\n");
        return;
    }

    int i = patientCount;
    sprintf(patientID[i], "PAT-%d", 1001 + i);

    printf("\n--- NEW PATIENT REGISTRATION [%s] ---\n", patientID[i]);

   
    printf("Enter Patient Name: ");
    getchar();
    fgets(patientName[i], sizeof(patientName[i]), stdin);
    patientName[i][strcspn(patientName[i], "\n")] = 0; 

    printf("Enter Patient Age: ");
    scanf("%d", &patientAge[i]);

    do {
        printf("Enter Urgency Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &patientUrgency[i]);
    } while (patientUrgency[i] < 1 || patientUrgency[i] > 3);

   
    printf("\nAvailable Specialties:\n");
    for (int s = 0; s < NUM_SPECIALTIES; s++) {
        printf("  %d. %-25s (Fee: LKR %.2f, Daily Cap: %d)\n",
               s + 1, SPECIALTY_NAMES[s], BASE_FEES[s], DAILY_CAPS[s]);
    }

    do {
        printf("Select Specialty (1-%d): ", NUM_SPECIALTIES);
        scanf("%d", &patientSpecialty[i]);
    } while (patientSpecialty[i] < 1 || patientSpecialty[i] > NUM_SPECIALTIES);

    int specIdx = patientSpecialty[i] - 1;

   
    if (specialtyQueueCounts[specIdx] >= DAILY_CAPS[specIdx]) {
        printf("Warning: Daily patient capacity reached for %s!\n", SPECIALTY_NAMES[specIdx]);
    }

    
    patientWaitTime[i] = specialtyQueueCounts[specIdx] * AVG_TIMES[specIdx];
    specialtyQueueCounts[specIdx]++; // Increment queue count

  
    printf("\nIs patient admitted to ward? (1 = Yes, 0 = No): ");
    scanf("%d", &patientAdmitted[i]);

    patientBedNo[i] = 0;
    if (patientAdmitted[i] == 1) {
        printf("Available Wards:\n");
        for (int w = 0; w < NUM_WARDS; w++) {
            printf("  %d. %-25s (Rate: LKR %.2f/day)\n",
                   w + 1, WARD_NAMES[w], WARD_RATES[w]);
        }

        do {
            printf("Select Ward ID (1-%d): ", NUM_WARDS);
            scanf("%d", &patientWard[i]);
        } while (patientWard[i] < 1 || patientWard[i] > NUM_WARDS);

        printf("Enter Days Admitted: ");
        scanf("%d", &patientDays[i]);

       
        int wardIdx = patientWard[i] - 1;
        int bedAssigned = 0;

        for (int b = 0; b < WARD_CAPACITIES[wardIdx]; b++) {
            if (bedOccupancy[wardIdx][b] == 0) {
                bedOccupancy[wardIdx][b] = 1; 
                patientBedNo[i] = b + 1;     
                bedAssigned = 1;
                break;
            }
        }

        if (!bedAssigned) {
            printf("Warning: Selected ward is full! Patient marked for temporary hold.\n");
        }
    } else {
        patientWard[i] = 0;
        patientDays[i] = 0;
    }

   
    patientBaseFee[i] = BASE_FEES[specIdx];

    
    if (patientUrgency[i] == 2) {
        patientSurcharge[i] = patientBaseFee[i] * 0.20f;
    } else if (patientUrgency[i] == 3) {
        patientSurcharge[i] = patientBaseFee[i] * 0.50f;
    } else {
        patientSurcharge[i] = 0.0f;
    }

   
    if (patientAdmitted[i] == 1 && patientWard[i] > 0) {
        patientWardCost[i] = patientDays[i] * WARD_RATES[patientWard[i] - 1];
    } else {
        patientWardCost[i] = 0.0f;
    }

   
    patientGrossTotal[i] = patientBaseFee[i] + patientSurcharge[i] + patientWardCost[i];

    
    if (patientAge[i] < 5 || patientAge[i] > 65) {
        patientDiscount[i] = patientGrossTotal[i] * 0.15f;
    } else {
        patientDiscount[i] = 0.0f;
    }

    
    patientFinalBill[i] = patientGrossTotal[i] - patientDiscount[i];

   
    printf("\n======================================================\n");
    printf("           SMART HOSPITAL ADMISSION & BILL           \n");
    printf("======================================================\n");
    printf("Patient ID             : %s\n", patientID[i]);
    printf("Patient Name           : %s\n", patientName[i]);
    printf("Age                    : %d Years %s\n", patientAge[i],
           (patientAge[i] < 5 || patientAge[i] > 65) ? "(15% Subsidy Eligible)" : "");
    printf("Specialty              : %s\n", SPECIALTY_NAMES[specIdx]);

    if (patientAdmitted[i] == 1) {
        printf("Assigned Ward          : %s (Bed #%02d)\n",
               WARD_NAMES[patientWard[i] - 1], patientBedNo[i]);
    } else {
        printf("Assigned Ward          : Outpatient (OPD)\n");
    }

    printf("Urgency Level          : Level %d (%s)\n", patientUrgency[i],
           (patientUrgency[i] == 3) ? "Critical" : (patientUrgency[i] == 2) ? "Urgent" : "Normal");
    printf("------------------------------------------------------\n");
    printf("Base Consultation Fee  : LKR %10.2f\n", patientBaseFee[i]);
    printf("Emergency Surcharge    : LKR %10.2f\n", patientSurcharge[i]);
    printf("Ward Stay Cost (%d Days): LKR %10.2f\n", patientDays[i], patientWardCost[i]);
    printf("Gross Total Bill       : LKR %10.2f\n", patientGrossTotal[i]);
    printf("Age Subsidy Discount   : LKR %10.2f\n", -patientDiscount[i]);
    printf("------------------------------------------------------\n");
    printf("Final Payable Amount   : LKR %10.2f\n", patientFinalBill[i]);
    if (patientUrgency[i] == 3) {
        printf("Estimated Waiting Time : 0.00 mins (Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time : %.2f mins\n", patientWaitTime[i]);
    }
    printf("======================================================\n");

   
    logPatientRecord(i);
    patientCount++;
}

void displayBedMatrix(void) {
    printf("\n======================================================\n");
    printf("               HOSPITAL BED OCCUPANCY MATRIX           \n");
    printf("======================================================\n");
    printf("Status: [1 = Occupied | 0 = Available]\n\n");

    for (int w = 0; w < NUM_WARDS; w++) {
        printf("%-26s | ", WARD_NAMES[w]);
        int occupied = 0;
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            printf("%d ", bedOccupancy[w][b]);
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        printf("| Occupancy: %d/%d (%.1f%%)\n",
               occupied, WARD_CAPACITIES[w],
               ((float)occupied / WARD_CAPACITIES[w]) * 100.0f);
    }
    printf("======================================================\n");
}

void displayPriorityQueue(void) {
    if (patientCount == 0) {
        printf("\nNo patient records available to display.\n");
        return;
    }

  
    int indices[MAX_PATIENTS];
    for (int i = 0; i < patientCount; i++) {
        indices[i] = i;
    }

    
    for (int i = 0; i < patientCount - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < patientCount; j++) {
            if (patientUrgency[indices[j]] > patientUrgency[indices[maxIdx]]) {
                maxIdx = j;
            }
        }
        int temp = indices[i];
        indices[i] = indices[maxIdx];
        indices[maxIdx] = temp;
    }

    printf("\n=======================================================================\n");
    printf("                      EMERGENCY TRIAGE PRIORITY QUEUE                 \n");
    printf("=======================================================================\n");
    printf("%-10s | %-20s | %-10s | %-22s\n", "ID", "Name", "Urgency", "Specialty");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < patientCount; i++) {
        int idx = indices[i];
        char urgencyStr[15];
        if (patientUrgency[idx] == 3) strcpy(urgencyStr, "L3 (Critical)");
        else if (patientUrgency[idx] == 2) strcpy(urgencyStr, "L2 (Urgent)");
        else strcpy(urgencyStr, "L1 (Normal)");

        printf("%-10s | %-20s | %-10s | %-22s\n",
               patientID[idx], patientName[idx], urgencyStr,
               SPECIALTY_NAMES[patientSpecialty[idx] - 1]);
    }
    printf("=======================================================================\n");
}

void generateAnalyticsReport(void) {
    if (patientCount == 0) {
        printf("\nNo system data available for analytics.\n");
        return;
    }

    int countNormal = 0, countUrgent = 0, countCritical = 0;
    float totalRevenue = 0.0f, totalDiscounts = 0.0f;
    int highestIndex = 0;
    float maxBill = -1.0f;

    for (int i = 0; i < patientCount; i++) {
       
        if (patientUrgency[i] == 1) countNormal++;
        else if (patientUrgency[i] == 2) countUrgent++;
        else if (patientUrgency[i] == 3) countCritical++;

        
        totalRevenue += patientFinalBill[i];
        totalDiscounts += patientDiscount[i];

       
        if (patientFinalBill[i] > maxBill) {
            maxBill = patientFinalBill[i];
            highestIndex = i;
        }
    }

    printf("\n======================================================\n");
    printf("            PERFORMANCE REPORTS & ANALYTICS            \n");
    printf("======================================================\n");
    printf("1. Patient Intake Breakdown:\n");
    printf("   - Total Patients Registered : %d\n", patientCount);
    printf("   - Level 1 (Normal)          : %d\n", countNormal);
    printf("   - Level 2 (Urgent)          : %d\n", countUrgent);
    printf("   - Level 3 (Critical)        : %d\n", countCritical);

    printf("\n2. Financial Summary:\n");
    printf("   - Total Revenue Earned      : LKR %.2f\n", totalRevenue);
    printf("   - Total Discounts Granted   : LKR %.2f\n", totalDiscounts);

    printf("\n3. Ward Bed Occupancy:\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupied = 0;
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupied++;
        }
        printf("   - %-25s : %.2f%%\n", WARD_NAMES[w],
               ((float)occupied / WARD_CAPACITIES[w]) * 100.0f);
    }

    printf("\n4. Highest Paying Patient:\n");
    printf("   - Name       : %s\n", patientName[highestIndex]);
    printf("   - ID         : %s\n", patientID[highestIndex]);
    printf("   - Total Bill : LKR %.2f\n", patientFinalBill[highestIndex]);
    printf("======================================================\n");
}
