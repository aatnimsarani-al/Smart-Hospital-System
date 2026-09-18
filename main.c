#include "hospital.h"

int main(void) {
    loadBedsFromFile(); // Load saved bed occupancy data

    int choice;
    do {
        printf("\n======================================================\n");
        printf("    SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM      \n");
        printf("======================================================\n");
        printf("1. Register Patient & Process Bill\n");
        printf("2. Display Bed Occupancy Matrix\n");
        printf("3. Display Emergency Triage Priority Queue\n");
        printf("4. Generate Performance Reports & Analytics\n");
        printf("5. Save Matrix & Exit System\n");
        printf("------------------------------------------------------\n");
        printf("Enter Choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayBedMatrix();
                break;
            case 3:
                displayPriorityQueue();
                break;
            case 4:
                generateAnalyticsReport();
                break;
           case 5:
                saveBedsToFile();
                printf("\nBed status matrix saved successfully to bed_status.txt.\nExiting system...\n");
                break;
           
            default:
                printf("\n[ERROR] Invalid option selected! Please enter a choice between 1 and 5.\n");
                break;
        }
    } while (choice != 5);

    return 0;
}
