# Smart-Hospital-System
# Smart Hospital & Resource Allocation System

A modular, menu-driven C program designed to manage patient registration, bed allocations, emergency triage sorting, billing calculations, and hospital performance analytics.

## Features
- **Patient Intake & Registration:** Collects patient demographics, urgency levels, and ward requirements.
- **Automated Billing Engine:** Calculates consultation fees, emergency surcharges, ward stay costs, and age subsidy discounts (15% for ages < 5 or > 65).
- **Bed Occupancy Matrix:** Real-time tracking and file persistence (`bed_status.txt`) for ward capacities.
- **Emergency Triage Priority Queue:** Sorts patients using priority order (Critical → Urgent → Normal).
- **Analytics & Reporting:** Summary metrics for total revenue, discounts, ward occupancy rates, and top-paying patients.
- **Data Persistence:** Persistent logging of patient billing receipts (`patient_records.txt`).

## Technical Details
- **Language:** C
- **Data Structures:** Parallel Arrays
- **IDE:** Code::Blocks / GCC Compiler

## How to Build and Run

### Using GCC (Terminal)
1. Clone the repository:
   ```bash
   git clone [https://github.com/YOUR_USERNAME/YOUR_REPOSITORY_NAME.git](https://github.com/YOUR_USERNAME/YOUR_REPOSITORY_NAME.git)
   cd YOUR_REPOSITORY_NAME
