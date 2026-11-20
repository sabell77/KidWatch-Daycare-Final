#include <iostream>
#include "menu.h"
#include "attendance.h"
#include "queue.h"

using namespace std;

// Global variables accessible in all modules
AttendanceList tempAttendance;      // Temporary list for today
AttendanceList permanentAttendance; // Permanent, confirmed records
AttendanceQueue checkinQueue;

int main() {
    cout << "\n";
    // ====== Preset Attendance Records ======
    tempAttendance.addRecord(new AttendanceRecord{
        "001", "Amanda Lim", "3-4 years", "2024-06-10", "08:10", "15:15", "Mrs. Lim", "012-3456789", "Present",
        { "Reading", "Art", "Rest", "Writing", "Outdoor" }, nullptr
    });
    tempAttendance.addRecord(new AttendanceRecord{
        "002", "Daniel Lee", "5-6 years", "2024-06-10", "08:20", "15:30", "Mrs. Lee", "013-4567890", "Present",
        { "Reading", "Art", "Rest", "Outdoor" }, nullptr // 4 activities
    });
    tempAttendance.addRecord(new AttendanceRecord{
        "003", "Chloe Tan", "5-6 years", "2024-06-10", "09:00", "-", "Mr. Tan", "014-5678901", "Present",
        { "Reading", "Rest", "Outdoor" }, nullptr // 3 activities
    });
    tempAttendance.addRecord(new AttendanceRecord{
        "004", "Ethan Goh", "4-5 years", "2024-06-10", "08:05", "-", "Mr. Goh", "015-6789012", "Absent (Sick)",
        {}, nullptr // No activities, absent
    });
    tempAttendance.addRecord(new AttendanceRecord{
        "005", "Nur Izzah", "3-4 years", "2024-06-10", "08:15", "15:25", "Pn. Siti", "016-7890123", "Present",
        { "Reading", "Art", "Rest", "Writing", "Outdoor" }, nullptr
    });

    // ====== Preset Queue Records (optional, for testing queue) ======
    checkinQueue.enqueue(new AttendanceRecord{
        "006", "Ali Baba", "5-6 years", "2024-06-10", "08:40", "-", "Mr. Baba", "017-8901234", "Present", {}, nullptr
    });
    checkinQueue.enqueue(new AttendanceRecord{
        "007", "Siti Aminah", "4-5 years", "2024-06-10", "08:45", "-", "Mrs. Aminah", "018-9012345", "Present", {}, nullptr
    });

    // ====== Run Main Menu ======
    showMenu();

    // ====== Exit Program ======
    cout << "\n";
    cout << "Thank you for using KidWatch!" << endl;
    return 0;
}