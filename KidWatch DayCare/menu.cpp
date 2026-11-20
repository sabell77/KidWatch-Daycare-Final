#include "menu.h"
#include "attendance.h"
#include "queue.h"
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

extern AttendanceList tempAttendance;
extern AttendanceList permanentAttendance;
extern AttendanceQueue checkinQueue;

void showMenu() {
    int choice;
    int menuPage = 1; // 1=Attendance & Queue, 2=Search & Admin

    do {
        cout << "\n";
        if (menuPage == 1) {
            cout << "==== KidWatch Daycare - Attendance & Activity Monitoring (Page 1/2) ====\n";
            cout << "1. View Today's Attendance List\n";
            cout << "2. Add Attendance Record (Temporary List)\n";
            cout << "3. Edit Attendance Record (Temporary List)\n";
            cout << "4. Delete Attendance Record (Temporary List)\n";
            cout << "5. Add Check-in to Queue\n";
            cout << "6. View Check-in Queue\n";
            cout << "7. Process Next Check-in (Dequeue to Temporary List)\n";
            cout << "8. Check-out Student Present Today (Set Time Out)\n";
            cout << "9. Confirm & Save All to Permanent List (End of Day)\n";
            cout << "0. >> Go to Page 2\n ";
        } else if (menuPage == 2) {
            cout << "==== KidWatch Daycare - Attendance & Activity Monitoring (Page 2/2) ====\n";
            cout << "10. Search Attendance by Name (Today Only)\n";
            cout << "11. Search Attendance by Record ID (Today Only)\n";
            cout << "12. Check for Duplicate Record IDs (Today Only)\n";
            cout << "13. View Permanent Attendance Records\n";
            cout << "14. Add Student Activity\n";
            cout << "15. Edit Student Activity\n";
            cout << "16. Delete Student Activity\n";
            cout << "17. Undo Last Activity Delete\n";
            cout << "18. View Student's Activity Status\n";
            cout << "19. View Attendance Summary Report (Sort/Filter)\n";
            cout << "20. Exit System\n";
            cout << "0.  << Back to Page 1\n";
        }

        cout << "\nEnter your choice : ";
        cin >> choice;

        if (menuPage == 1) {
            switch (choice) {
                case 1: { // View all attendance records
                    cout << "\n\n";
                    if (tempAttendance.head == nullptr) {
                        cout << "Attendance list is empty.\n";
                    } else {
                        tempAttendance.display();
                    }
                    cout << "\n";
                    break;
                }
				case 2: { // Add Attendance Record
				    cout << "\nToday's attendance records :\n";
				    tempAttendance.listAttendanceSimple();
				    
				    cout << "\nCurrent Check-in Queue (IDs) :\n";
    				checkinQueue.displayQueue();
    				
				    string id, name, date, in, out, guardian, contactNumber, status;
				    int age;
				    while (true) {
				        cout << "\nEnter new Record ID to add (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break;
				        }
				        // Check for duplication in attendance list
				        if (tempAttendance.recordIDExists(id)) {
				            cout << "\nThat Record ID already exists in today's attendance list! Please enter a unique ID." << endl;
				            continue;
				        }
				        // Check for duplication in queue
				        if (checkinQueue.recordIDExists(id)) {
				            cout << "\nThat Record ID already exists in the check-in queue! Please enter a different ID." << endl;
				            continue;
				        }
				        // If no duplicate, collect the rest of the details
				        cin.ignore();
				        cout << "\nEnter Child Name: "; getline(cin, name);
				        cout << "Enter Age (years): "; cin >> age; cin.ignore();
				        string ageGroup = getAgeGroup(age);
				        cout << "Enter Date: "; getline(cin, date);
				        cout << "Enter Time In: "; getline(cin, in);
				        cout << "Enter Time Out: "; getline(cin, out);
				        cout << "Enter Guardian Name: "; getline(cin, guardian);
				        cout << "Enter Status: "; getline(cin, status);
				        cout << "Enter Guardian Contact Number: "; getline(cin, contactNumber);
				        AttendanceRecord* newRec = new AttendanceRecord{
				            id, name, ageGroup, date, in, out, guardian, contactNumber, status, {}, nullptr
				        };
				        tempAttendance.addRecord(newRec);
				        cout << "\nRecord added!\n";
				        break;
				    }
				    break;
				}
				case 3: { // Edit Attendance Record
				    if (tempAttendance.head == nullptr) {
				        cout << "\nNothing to edit, the attendance list is empty.\n";
				        break;
				    }
				    cout << "\nToday's attendance records:\n";
				    tempAttendance.listAttendanceSimple();
				    string id;
				    while (true) {
				        cout << "\nEnter Record ID to edit (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break;
				        }
				        // Optionally: check if the ID exists before proceeding
				        tempAttendance.editRecord(id);
				        cout << "\n";
				        break;
				    }
				    break;
				}
				case 4: { // Delete Attendance Record
				    if (tempAttendance.head == nullptr) {
				        cout << "\nNothing to delete, the attendance list is empty.\n";
				        break;
				    }
				    cout << "\nToday's attendance records:\n";
				    tempAttendance.listAttendanceSimple();
				    string id;
				    while (true) {
				        cout << "\nEnter Record ID to delete (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break;
				        }
				        // Optionally: check if the ID exists before proceeding
				        tempAttendance.deleteRecord(id);
				        cout << "\n";
				        break;
				    }
				    break;
				}
				case 5: {
					cout << "\n";
					checkinQueue.displayQueue();
					
					cout << "\n(Existing Record IDs in Queue: ";
					checkinQueue.displayQueueIDs();
					cout << " | Attendance: ";
					tempAttendance.displayRecordIDs();
					cout << ")\n";
				
				    string id, name, date, in, guardian, status, contactNumber;
				    int age;
				    string out = "-"; // Not checked out yet
				
				    do {
				        cout << "\nEnter Record ID : "; cin >> id;
				        if (checkinQueue.recordIDExists(id)) {
				            cout << "That Record ID already exists in check-in queue! Please enter a unique ID." << endl;
				        } else if (tempAttendance.recordIDExists(id)) {
				            cout << "That Record ID already exists in attendance list! Please enter a unique ID." << endl;
				        }
				    } while (tempAttendance.recordIDExists(id) || checkinQueue.recordIDExists(id));
				
				    cout << "\nEnter Child Name : "; cin.ignore(); getline(cin, name);
				    cout << "Enter Age (years) : "; cin >> age; cin.ignore();
				    string ageGroup = getAgeGroup(age);
				    cout << "Enter Date : "; getline(cin, date);
				    cout << "Enter Time In : "; getline(cin, in);
				    cout << "Enter Guardian Name : "; getline(cin, guardian);
				    cout << "Enter Contact Number : "; getline(cin, contactNumber);
				    cout << "Enter Status : "; getline(cin, status);
				
				    AttendanceRecord* newRec = new AttendanceRecord{
				        id, name, ageGroup, date, in, out, guardian, contactNumber, status, {}, nullptr
				    };
				    checkinQueue.enqueue(newRec);
				    cout << "\nAdded to queue.\n";
				    break;
				}
                case 6: // View Queue
                    cout << "\n";
                    checkinQueue.displayQueue();
                    cout << "\n";
                    break;
                case 7: { // Process Next Check-in
				// Before dequeuing, show the current queue
				checkinQueue.displayQueue();
				
				AttendanceRecord* processed = checkinQueue.dequeue();
					if (processed != nullptr) {
					    // Display details in vertical format
					    cout << "\nReady to check-in this student:\n";
					    cout << "  Record ID : " << processed->recordID << endl;
					    cout << "  Name      : " << processed->childName << endl;
					    cout << "  Status    : " << processed->status << endl;
					    cout << "  Age Group : " << processed->ageGroup << endl;
					    cout << "  Date      : " << processed->date << endl;
					    cout << "  Time In   : " << processed->timeIn << endl;
					    cout << "  Guardian  : " << processed->guardian << endl;
					    cout << "  Contact   : " << processed->contactNumber << endl;
					    cout << "----------------------------------------" << endl;
					
					    // Confirmation prompt
					    char confirm;
					    cout << "\nConfirm check-in for this student? (y/n): ";
					    cin >> confirm;
					    if (confirm == 'y' || confirm == 'Y') {
					        tempAttendance.addRecord(processed);
					        cout << "\nStudent successfully added to attendance list!" << endl;
					    } else {
					        // If not confirmed, put back in queue
					        checkinQueue.enqueue(processed);
					        cout << "\nOperation cancelled. Student remains in the check-in queue." << endl;
					    }
					} else {
					    cout << "\nQueue is empty." << endl;
					}
				break;
				}
                case 8: { // Check-out Student Present Today
				    // Prepare two lists
				    vector<AttendanceRecord*> notCheckedOut, checkedOut;
				    AttendanceRecord* temp = tempAttendance.head;
				    while (temp) {
				        if (temp->status == "Present") {
				            if (temp->timeOut.empty() || temp->timeOut == "-")
				                notCheckedOut.push_back(temp);
				            else
				                checkedOut.push_back(temp);
				        }
				        temp = temp->next;
				    }
				
				    // Show not yet checked out
				    cout << "\nStudents present today (NOT yet checked out):\n";
				    if (notCheckedOut.empty()) {
				        cout << "  (None)\n";
				    } else {
				        cout << left << setw(8) << "ID"
				             << setw(15) << "Name"
				             << setw(12) << "Date"
				             << setw(10) << "Time In"
				             << setw(10) << "Time Out" << endl;
				        for (auto* rec : notCheckedOut) {
				            cout << left << setw(8) << rec->recordID
				                 << setw(15) << rec->childName
				                 << setw(12) << rec->date
				                 << setw(10) << rec->timeIn
				                 << setw(10) << rec->timeOut << endl;
				        }
				    }
				
				    // Show already checked out
				    cout << "\nStudents present today (ALREADY checked out today):\n";
				    if (checkedOut.empty()) {
				        cout << "  (None)\n";
				    } else {
				        cout << left << setw(8) << "ID"
				             << setw(15) << "Name"
				             << setw(12) << "Date"
				             << setw(10) << "Time In"
				             << setw(10) << "Time Out" << endl;
				        for (auto* rec : checkedOut) {
				            cout << left << setw(8) << rec->recordID
				                 << setw(15) << rec->childName
				                 << setw(12) << rec->date
				                 << setw(10) << rec->timeIn
				                 << setw(10) << rec->timeOut << endl;
				        }
				    }
				
				    // Loop until valid Record ID or 0
					string id;
					while (true) {
					    cout << "\nEnter Record ID of student to check out (or 0 to go back): ";
					    cin >> id;
					    if (id == "0") {
					        cout << "Cancelled. Returning to menu.\n";
					        break;
					    }
					
					    // Check if ID exists in notCheckedOut list (valid to check out)
					    bool foundNotCheckedOut = false;
					    for (auto* rec : notCheckedOut) {
					        if (rec->recordID == id) {
					            string newTimeOut;
					            cout << "\nEnter check-out (Time Out) for " << rec->childName << ": ";
					            cin.ignore();
					            getline(cin, newTimeOut);
					            rec->timeOut = newTimeOut;
					            cout << "Check-out time updated for " << rec->childName << "!\n";
					            foundNotCheckedOut = true;
					            break;
					        }
					    }
					    if (foundNotCheckedOut) break;
					
					    // Check if ID exists in checkedOut list (already checked out)
					    bool foundCheckedOut = false;
					    for (auto* rec : checkedOut) {
					        if (rec->recordID == id) {
					            cout << "The student is already checked-out at " << rec->timeOut
					                 << " | " << rec->date << ", Please enter ID again, or enter 0 to go back." << endl;
					            foundCheckedOut = true;
					            break;
					        }
					    }
					    if (foundCheckedOut) continue;
					
					    // If not found in either list
					    cout << "No matching present student with empty check-out time found for that ID. Try again, or enter 0 to go back." << endl;
					}
				    break;
				}
				case 9: { // Confirm & Save to Permanent List (End of Day)
				    // Check if check-in queue is NOT empty ========
					if (!checkinQueue.isEmpty()) {
					    cout << "\nThere are still students in the check-in queue!" << endl;
					    cout << "If you proceed, these students will NOT be saved to the permanent list today." << endl;
					    cout << "\nDo you still want to continue saving? (y/n): ";
					    char proceed;
					    cin >> proceed;
					    cin.ignore(); // Clear any newline left in the buffer
					    if (proceed != 'y' && proceed != 'Y') {
					        cout << "\nPlease process all check-in queue students before saving.\n";
					        break;
					    }
					}
				
				    vector<AttendanceRecord*> notCheckedOut;
				    AttendanceRecord* temp = tempAttendance.head;
				    while (temp != nullptr) {
				        if (temp->status == "Present" && (temp->timeOut.empty() || temp->timeOut == "-"))
				            notCheckedOut.push_back(temp);
				        temp = temp->next;
				    }
				
				    if (notCheckedOut.empty()) {
				        cout << "\nAll students are checked out. Nothing to save.\n";
				        break;
				    }
				
				    cout << "\nStudents present today (NOT yet checked out):\n";
				    cout << left << setw(8) << "ID" << setw(15) << "Name"
				         << setw(12) << "Date" << setw(10) << "Time In" << endl;
				    for (auto* rec : notCheckedOut)
				        cout << setw(8) << rec->recordID << setw(15) << rec->childName
				             << setw(12) << rec->date << setw(10) << rec->timeIn << endl;
				
				    // -- Use a cancel flag --
				    bool cancelled = false;
				    for (auto* rec : notCheckedOut) {
				        cout << "\n-------------------------------------" << endl;
				        cout << "  Ready to check-out this student:" << endl;
				        cout << "    Record ID : " << rec->recordID << endl;
				        cout << "    Name      : " << rec->childName << endl;
				        cout << "    Date      : " << rec->date << endl;
				        cout << "    Time In   : " << rec->timeIn << endl;
				        cout << "-------------------------------------" << endl;
				        cout << "Enter check-out (Time Out) for this student (or 0 to cancel): ";
				        string newTimeOut;
				        getline(cin >> ws, newTimeOut);
				        if (newTimeOut == "0") {
				            cout << "\nOperation cancelled. Returning to menu...\n";
				            cancelled = true;
				            break;  // break out of the for loop
				        }
				        rec->timeOut = newTimeOut;
				    }
				
				    // If cancelled, skip saving and return to menu
				    if (cancelled) break;
				
				    cout << "\nAre you sure you want to save all records to permanent list? (y/n): ";
				    char confirm;
				    cin >> confirm;
				    if (confirm == 'y' || confirm == 'Y') {
				        cout << "\nConfirming all today's records to permanent attendance..." << endl;
				        tempAttendance.moveAllTo(permanentAttendance);
				        std::this_thread::sleep_for(std::chrono::milliseconds(1200));
				        cout << "\nAll records saved to permanent attendance. Temporary list cleared!" << endl;
				    } else {
				        cout << "\nSave cancelled. Returning to menu...\n";
				    }
				    break;
				}
                case 0: // Next Page
                    menuPage = 2;
                    continue;
            }
        } else if (menuPage == 2) {
            switch (choice) {
				case 10: { // Search by Name
				    cin.ignore(); // Only needed before the *first* getline
				    tempAttendance.linearSearchByName();
				    break;
				}
				case 11: { // Search by Record ID (with loop)
				    cin.ignore();
				    while (true) {
				        cout << "\nEnter Record ID to search (or 0 to go back): ";
				        string recordID;
				        getline(cin, recordID);
				
				        if (recordID == "0") {
				            cout << "\nReturning to menu...\n";
				            break;
				        }
				        if (recordID.empty()) {
				            cout << "\nField should not be empty,re-enter record ID or 0 to go back.\n";
				            continue;
				        }
				        tempAttendance.binarySearchByID(recordID);
				    }
				    break;
				}
                case 12: { // Check for Duplicates
                    cout << "\n";
                    tempAttendance.checkAndReportDuplicates();
                    cout << "\n";
                    break;
                }
                case 13: { // View Permanent Attendance Records
                    cout << "\n";
                    if (permanentAttendance.head == nullptr) {
                        cout << "\nPermanent attendance list is empty.\n";
                    } else {
                        permanentAttendance.display();
                    }
                    cout << "\n";
                    break;
                }
				case 14: {
				    cout << "\nPresent students and their activities status:\n";
				    tempAttendance.listPresentAndCheckedOutStudentsWithActivity();
				    string id, activity;
				    AttendanceRecord* student = nullptr;
				
				    while (true) {
				        cout << "\nEnter Record ID of present student (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break; // Back to menu
				        }
				        student = tempAttendance.findRecordByID(id);
				        if (!student) {
				            cout << "\nID not exist, please re-enter record ID.\n";
				            continue;
				        }
				        if (student->status != "Present") {
				            cout << "\nNot-present record ID is not available to add activity.\n";
				            continue;
				        }
				        if (!(student->timeOut.empty() || student->timeOut == "-")) {
				            cout << "\nStudent (" << student->recordID << " - " << student->childName << ") already checked out today.\n";
				            continue;
				        }
				        if (student->activities.size() >= 5) {
				            cout << "\n*** Maximum of 5 activities reached for this student today. ***\n";
				            continue;
				        }
				        break;
				    }
				    if (id == "0") break; // User chose back before activity prompt
				    cin.ignore();
				    cout << "Enter activity to add: ";
				    getline(cin, activity);
				    tempAttendance.addActivity(id, activity);
				    break;
				}
				case 15: {
				    cout << "\nPresent students and their activities status:\n";
				    tempAttendance.listPresentAndCheckedOutStudentsWithActivity();
				    string id, newActivity;
				    int activityNum;
				    AttendanceRecord* student = nullptr;
				
				    while (true) {
				        cout << "\nEnter Record ID of present student (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break;
				        }
				        student = tempAttendance.findRecordByID(id);
				        if (!student) {
				            cout << "\nID not exist, please re-enter record ID.\n";
				            continue;
				        }
				        if (student->status != "Present") {
				            cout << "\nNot-present record ID is not available to edit activity.\n";
				            continue;
				        }
				        if (!(student->timeOut.empty() || student->timeOut == "-")) {
				            cout << "\nStudent (" << student->recordID << " - " << student->childName << ") already checked out today. Cannot edit activities.\n";
				            continue;
				        }
				        if (student->activities.empty()) {
				            cout << "\nNo activities to edit for this student.\n";
				            continue;
				        }
				        break;
				    }
				    if (id == "0") break;
				    cout << "\nActivities for " << student->childName << ":\n";
				    for (size_t i = 0; i < student->activities.size(); ++i) {
				        cout << (i + 1) << ". " << student->activities[i] << endl;
				    }
				    cout << "\nEnter activity number to edit (1-" << student->activities.size() << "): ";
				    cin >> activityNum;
				    cin.ignore();
				    cout << "Enter new activity description: ";
				    getline(cin, newActivity);
				    tempAttendance.editActivity(id, activityNum, newActivity);
				    break;
				}
				case 16: {
				    cout << "\nPresent students and their activities status:\n";
				    tempAttendance.listPresentAndCheckedOutStudentsWithActivity();
				    string id;
				    int activityNum;
				    AttendanceRecord* student = nullptr;
				
				    while (true) {
				        cout << "\nEnter Record ID of present student to delete activity (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break;
				        }
				        student = tempAttendance.findRecordByID(id);
				        if (!student) {
				            cout << "\nID not exist, please re-enter record ID.\n";
				            continue;
				        }
				        if (student->status != "Present") {
				            cout << "\nNot-present record ID is not available to delete activity.\n";
				            continue;
				        }
				        if (!(student->timeOut.empty() || student->timeOut == "-")) {
				            cout << "\nStudent (" << student->recordID << " - " << student->childName << ") already checked out today. Cannot delete activities.\n";
				            continue;
				        }
				        if (student->activities.empty()) {
				            cout << "\nNo activities to delete for this student.\n";
				            continue;
				        }
				        break;
				    }
				    if (id == "0") break;
				    cout << "\nActivities for " << student->childName << ":\n";
				    for (size_t i = 0; i < student->activities.size(); ++i) {
				        cout << (i + 1) << ". " << student->activities[i] << endl;
				    }
				    cout << "\nEnter activity number to delete (1-" << student->activities.size() << "): ";
				    cin >> activityNum;
				    tempAttendance.deleteActivity(id, activityNum);
				    break;
					}
					case 17: {
				    std::vector<AttendanceRecord*> canUndo = tempAttendance.studentsWithUndoActivity();
				    if (canUndo.empty()) {
				        cout << "\nNo present student has a deleted activity to undo.\n";
				        break;
				    }
				    cout << "\nPresent students with deleted activity (can be undone):\n";
				    cout << std::left << std::setw(8) << "ID"
				         << std::setw(15) << "Child Name"
				         << std::setw(8) << "Status"
				         << "Last Deleted Activity\n";
				    for (auto* student : canUndo) {
				        cout << std::left
				             << std::setw(8) << student->recordID
				             << std::setw(15) << student->childName
				             << std::setw(8) << "Present"
				             << lastDeletedActivityMap[student->recordID] << endl;
				    }
				
				    string id;
				    while (true) {
				        cout << "\nEnter Record ID of present student to undo last activity delete (or 0 to go back): ";
				        cin >> id;
				        if (id == "0") {
				            cout << "Cancelled. Returning to menu.\n";
				            break;
				        }
				        AttendanceRecord* student = tempAttendance.findRecordByID(id);
				        if (!student) {
				            cout << "\nID not exist, please re-enter record ID.\n";
				            continue;
				        }
				        if (student->status != "Present") {
				            cout << "\nStudent (" << student->recordID << " - " << student->childName << ") has already checked out. Cannot undo activity delete.\n";
				            continue;
				        }
				        if (!(student->timeOut.empty() || student->timeOut == "-")) {
				            cout << "\nStudent (" << student->recordID << " - " << student->childName << ") has already checked out. Cannot undo activity delete.\n";
				            continue;
				        }
				        if (!lastDeletedActivityMap.count(student->recordID)) {
				            cout << "\nNo activity to undo for this student.\n";
				            continue;
				        }
				        tempAttendance.undoDeleteActivity(id);
				        break;
				    }
				    break;
				}
				case 18: {
				    cout << "\nAll Students' Activity Status :\n";
				    tempAttendance.viewAllActivityStatus();
				    cout << endl;
				    break;
				}
				case 19: {
    				permanentAttendance.displayAttendanceSummaryReport();
				    break;
				}
                case 20: // Exit
                    cout << "\nExiting..." << endl;
                    cout << "\n";
                    break;
                case 0: // Back to Page 1
                    menuPage = 1;
                    continue;
            }
        }

        // Only prompt to continue if not exiting
        if ((menuPage == 1 && choice != 20) || (menuPage == 2 && choice != 20)) {
            cout << "\nPress Enter to continue...";
            cout << "\n";
            cin.ignore();
            cin.get();
        }
    } while (!(menuPage == 2 && choice == 20));
}
