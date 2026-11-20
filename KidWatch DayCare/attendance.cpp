#include "attendance.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <set>
#include <chrono>
#include <thread>

std::map<std::string, std::string> lastDeletedActivityMap; // key: recordID, value: activity

using namespace std;

// Helper comparator for sorting
bool compareByID(AttendanceRecord* a, AttendanceRecord* b) {
    return a->recordID < b->recordID;
}

// Helper struct for sorting by completion status
struct SummaryRecord {
    AttendanceRecord* rec;
    int activitiesDone;
};

bool AttendanceList::isPresentRecord(const std::string& id) {
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->recordID == id)
            return (temp->status == "Present");
        temp = temp->next;
    }
    return false;
}

std::string getAgeGroup(int age) {
	if (age >= 1 && age <= 2)
		return "1-2 years";
    else if (age >= 3 && age <= 4)
        return "3-4 years";
    else if (age == 5 || age == 6)
        return "5-6 years";
    else
        return "Other";
}

std::vector<AttendanceRecord*> AttendanceList::studentsWithUndoActivity() {
    std::vector<AttendanceRecord*> undoable;
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->status == "Present" &&
            (temp->timeOut.empty() || temp->timeOut == "-") &&
            lastDeletedActivityMap.count(temp->recordID)) {
            undoable.push_back(temp);
        }
        temp = temp->next;
    }
    return undoable;
}

AttendanceList::AttendanceList() {
    head = nullptr;
}

bool AttendanceList::recordIDExists(const string& id) {
    AttendanceRecord* temp = head;
    while (temp != nullptr) {
        if (temp->recordID == id)
            return true;
        temp = temp->next;
    }
    return false;
}

void AttendanceList::addRecord(AttendanceRecord* record) {
    record->next = head;
    head = record;
}

void AttendanceList::displayRecordIDs() {
    AttendanceRecord* temp = head;
    while (temp) {
        cout << temp->recordID;
        if (temp->next) cout << ", ";
        temp = temp->next;
    }
    if (!head) cout << "(none)";
}

void AttendanceList::display() {
    // Collect all records in a vector
    std::vector<AttendanceRecord*> records;
    AttendanceRecord* temp = head;
    while (temp != nullptr) {
        records.push_back(temp);
        temp = temp->next;
    }

    // Sort the vector by recordID (ascending)
    std::sort(records.begin(), records.end(), compareByID);

    // Print table header
    std::cout << std::left 
        << std::setw(5) << "ID"
        << std::setw(15) << "Name"
        << std::setw(12) << "AgeGrp"
        << std::setw(12) << "Date"
        << std::setw(10) << "Time In"
        << std::setw(10) << "Time Out"
        << std::setw(15) << "Guardian"
        << std::setw(15) << "Contact No"
        << std::setw(10) << "Status" << std::endl;

    // Print each sorted record
    for (auto* rec : records) {
        std::cout << std::left
            << std::setw(5) << rec->recordID
            << std::setw(15) << rec->childName
            << std::setw(12) << rec->ageGroup
            << std::setw(12) << rec->date
            << std::setw(10) << rec->timeIn
            << std::setw(10) << rec->timeOut
            << std::setw(15) << rec->guardian
            << std::setw(15) << rec->contactNumber
            << std::setw(10) << rec->status << std::endl;
    }
}

void AttendanceList::editRecord(const std::string& recordID) {
    AttendanceRecord* temp = head;
    while (temp != nullptr) {
        if (temp->recordID == recordID) {
            int choice;
            do {
                // Display editable fields
                cout << "\nEditing record for " << temp->childName << " (ID : " << temp->recordID << ")\n";
                cout << "\nWhich field would you like to edit?\n";
                cout << "1. Child Name      : " << temp->childName << endl;
                cout << "2. Age Group       : " << temp->ageGroup << endl;
                cout << "3. Time In         : " << temp->timeIn << endl;
                cout << "4. Time Out        : " << temp->timeOut << endl;
                cout << "5. Guardian        : " << temp->guardian << endl;
                cout << "6. Contact Number  : " << temp->contactNumber << endl;
                cout << "7. Status          : " << temp->status << endl;
                cout << "0. Finish Editing\n";
                cout << "\nEnter field number to edit (0 to finish): ";
                cin >> choice;
                cin.ignore();

                switch (choice) {
                    case 1:
                        cout << "\nEnter new Child Name: ";
                        getline(cin, temp->childName);
                        cout << "Updated!\n";
                        break;
                    case 2: {
                        int newAge;
                        cout << "Enter new Age (years): ";
                        cin >> newAge; cin.ignore();
                        temp->ageGroup = getAgeGroup(newAge);
                        cout << "Updated!\n";
                        break;
                    }
                    case 3:
                        cout << "Enter new Time In: ";
                        getline(cin, temp->timeIn);
                        cout << "Updated!\n";
                        break;
                    case 4:
                        cout << "Enter new Time Out: ";
                        getline(cin, temp->timeOut);
                        cout << "Updated!\n";
                        break;
                    case 5:
                        cout << "Enter new Guardian: ";
                        getline(cin, temp->guardian);
                        cout << "Updated!\n";
                        break;
                    case 6:
                        cout << "Enter new Contact Number: ";
                        getline(cin, temp->contactNumber);
                        cout << "Updated!\n";
                        break;
                    case 7:
                        cout << "Enter new Status: ";
                        getline(cin, temp->status);
                        cout << "Updated!\n";
                        break;
                    case 0:
                        cout << "Finished editing.\n";
                        break;
                    default:
                        cout << "Invalid choice. Try again.\n";
                }
            } while (choice != 0);
            return;
        }
        temp = temp->next;
    }
    cout << "\n\nRecord ID not found!" << endl;
}

void AttendanceList::deleteRecord(const std::string& recordID) {
    AttendanceRecord* temp = head;
    AttendanceRecord* prev = nullptr;

    while (temp != nullptr) {
        if (temp->recordID == recordID) {
            if (prev == nullptr) {
                head = temp->next; // Deleting head
            } else {
                prev->next = temp->next;
            }
            delete temp;
            cout << "\nRecord deleted!" << endl;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    cout << "\nRecord ID not found!" << endl;
}

void AttendanceList::linearSearchByName() {
    while (true) {
        cout << "\nEnter Child Name to search (or 0 to go back): ";
        string name;
        getline(cin, name);

        // If empty input, do nothing and prompt again
        if (name.empty()) {
            cout << "Name cannot be empty. Please enter a name or 0 to go back.\n";
            continue;
        }
        if (name == "0") {
            cout << "Returning to menu...\n";
            break;
        }

        // Lowercase search input for case-insensitive matching
        string searchName = name;
        transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

        // Collect matching records
        vector<AttendanceRecord*> exactMatches, partialMatches;
        AttendanceRecord* temp = head;
        while (temp != nullptr) {
            string childNameLower = temp->childName;
            transform(childNameLower.begin(), childNameLower.end(), childNameLower.begin(), ::tolower);

            if (childNameLower == searchName && !searchName.empty()) {
                exactMatches.push_back(temp);
            } else if (childNameLower.find(searchName) != string::npos && !searchName.empty()) {
                partialMatches.push_back(temp);
            }
            temp = temp->next;
        }

        // Output
        if (!exactMatches.empty()) {
            cout << "\nExact match found for: " << name << endl;
            cout << left << setw(5) << "ID"
                 << setw(15) << "Name"
                 << setw(12) << "AgeGrp"
                 << setw(12) << "Date"
                 << setw(10) << "Time In"
                 << setw(10) << "Time Out"
                 << setw(15) << "Guardian"
                 << setw(10) << "Status" << endl;
            for (auto* rec : exactMatches) {
                cout << left << setw(5) << rec->recordID
                     << setw(15) << rec->childName
                     << setw(12) << rec->ageGroup
                     << setw(12) << rec->date
                     << setw(10) << rec->timeIn
                     << setw(10) << rec->timeOut
                     << setw(15) << rec->guardian
                     << setw(10) << rec->status << endl;
            }
        } else if (!partialMatches.empty()) {
            cout << "\nPartial matches for: " << name << endl;
            cout << left << setw(5) << "ID"
                 << setw(15) << "Name"
                 << setw(12) << "AgeGrp"
                 << setw(12) << "Date"
                 << setw(10) << "Time In"
                 << setw(10) << "Time Out"
                 << setw(15) << "Guardian"
                 << setw(10) << "Status" << endl;
            for (auto* rec : partialMatches) {
                cout << left << setw(5) << rec->recordID
                     << setw(15) << rec->childName
                     << setw(12) << rec->ageGroup
                     << setw(12) << rec->date
                     << setw(10) << rec->timeIn
                     << setw(10) << rec->timeOut
                     << setw(15) << rec->guardian
                     << setw(10) << rec->status << endl;
            }
        } else {
            cout << "\nNo records found for name containing: " << name << endl;
        }
    }
}

void AttendanceList::binarySearchByID(const std::string& recordID) {
    std::vector<AttendanceRecord*> records;
    AttendanceRecord* temp = head;
    while (temp != nullptr) {
        records.push_back(temp);
        temp = temp->next;
    }

    // Sort by Record ID
    std::sort(records.begin(), records.end(), compareByID);

    // Binary Search (only match ID exactly, no index column!)
    int left = 0, right = records.size() - 1;
    bool found = false;

    cout << "\nSearch results for Record ID : " << recordID << endl;
    cout << std::setw(5) << "ID"
         << std::setw(15) << "Name"
         << std::setw(12) << "AgeGrp"
         << std::setw(12) << "Date"
         << std::setw(10) << "Time In"
         << std::setw(10) << "Time Out"
         << std::setw(15) << "Guardian"
         << std::setw(10) << "Status" << endl;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (records[mid]->recordID == recordID) {
            // Print ONLY the found record
            cout << std::setw(5) << records[mid]->recordID
                 << std::setw(15) << records[mid]->childName
                 << std::setw(12) << records[mid]->ageGroup
                 << std::setw(12) << records[mid]->date
                 << std::setw(10) << records[mid]->timeIn
                 << std::setw(10) << records[mid]->timeOut
                 << std::setw(15) << records[mid]->guardian
                 << std::setw(10) << records[mid]->status << endl;
            found = true;
            break;
        } else if (records[mid]->recordID < recordID) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    if (!found) {
        cout << "\nRecord ID not found." << endl;
    }
}

void AttendanceList::checkAndReportDuplicates() {
    set<string> seenIDs;
    int duplicates = 0;
    AttendanceRecord* temp = head;

	cout << "\nChecking for duplicate Record IDs in attendance list..." << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1200)); // 1.2 second pause

    while (temp != nullptr) {
        if (seenIDs.find(temp->recordID) != seenIDs.end()) {
            cout << "Duplicate found: Record ID " << temp->recordID << " (Name: " << temp->childName << ")" << endl;
            duplicates++;
        } else {
            seenIDs.insert(temp->recordID);
        }
        temp = temp->next;
    }

    if (duplicates == 0) {
        cout << "\nNo duplicate Record IDs found in the attendance list!" << endl;
    } else {
        cout << "\nTotal duplicates found: " << duplicates << endl;
    }
}

void AttendanceList::moveAllTo(AttendanceList& dest) {
    AttendanceRecord* temp = head;
    while (temp != nullptr) {
        AttendanceRecord* next = temp->next;
        // Detach node and add to destination list
        temp->next = dest.head;
        dest.head = temp;
        temp = next;
    }
    head = nullptr; // Temporary list is now empty!
}

void AttendanceList::addActivity(const std::string& recordID, const std::string& activity) {
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->recordID == recordID && temp->status == "Present" &&
            !temp->timeIn.empty() && (temp->timeOut.empty() || temp->timeOut == "-")) {
            if (temp->activities.size() < 5) {
                temp->activities.push_back(activity);
                std::cout << "Activity added for " << temp->childName << "!\n";
            } else {
                std::cout << "\nMaximum of 5 activities reached for this student today.\n";
            }
            return;
        }
        temp = temp->next;
    }
    std::cout << "\nNo present student found with that Record ID, or student already checked out.\n";
}

void AttendanceList::editActivity(const std::string& recordID, int activityIndex, const std::string& newActivity) {
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->recordID == recordID && temp->status == "Present" &&
            !temp->timeIn.empty() && (temp->timeOut.empty() || temp->timeOut == "-")) {
            if (activityIndex >= 1 && activityIndex <= (int)temp->activities.size()) {
                temp->activities[activityIndex - 1] = newActivity;
                std::cout << "\nActivity #" << activityIndex << " updated for " << temp->childName << ".\n";
            } else {
                std::cout << "\nInvalid activity number.\n";
            }
            return;
        }
        temp = temp->next;
    }
    std::cout << "\nNo present student found with that Record ID, or student already checked out.\n";
}

void AttendanceList::deleteActivity(const std::string& recordID, int activityIndex) {
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->recordID == recordID && temp->status == "Present" &&
            (temp->timeOut.empty() || temp->timeOut == "-")) {
            if (activityIndex >= 1 && activityIndex <= (int)temp->activities.size()) {
                // Save for undo (use the map)
                lastDeletedActivityMap[recordID] = temp->activities[activityIndex - 1];
                temp->activities.erase(temp->activities.begin() + (activityIndex - 1));
                std::cout << "\nActivity #" << activityIndex << " deleted for " << temp->childName << ".\n";
            } else {
                std::cout << "Invalid activity number.\n";
            }
            return;
        }
        temp = temp->next;
    }
    std::cout << "\nNo present student found with that Record ID, or student already checked out.\n";
}

void AttendanceList::undoDeleteActivity(const std::string& recordID) {
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->recordID == recordID && temp->status == "Present" &&
            (temp->timeOut.empty() || temp->timeOut == "-")) {
            // Only undo if an activity was recently deleted
            if (lastDeletedActivityMap.count(recordID)) {
                temp->activities.push_back(lastDeletedActivityMap[recordID]);
                std::cout << "Undo successful! Restored last deleted activity for " << temp->childName << ".\n";
                lastDeletedActivityMap.erase(recordID);
            } else {
                std::cout << "No activity to undo for this student.\n";
            }
            return;
        }
        temp = temp->next;
    }
    std::cout << "No present student found with that Record ID, or student already checked out.\n";
}

void AttendanceList::viewAllActivityStatus() {
    // Collect pointers to present students
    std::vector<AttendanceRecord*> presentList;
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->status == "Present") {
            presentList.push_back(temp);
        }
        temp = temp->next;
    }
    // Sort by Record ID (ascending)
    std::sort(presentList.begin(), presentList.end(), compareByID);

    // Table header
    std::cout << std::left
        << std::setw(8) << "ID"
        << std::setw(15) << "Child Name"
        << std::setw(8) << "Status"
        << "Activities Completed" << std::endl;

    // Table rows
    for (auto* rec : presentList) {
        std::cout << std::left
            << std::setw(8) << rec->recordID
            << std::setw(15) << rec->childName
            << std::setw(8) << (std::to_string(rec->activities.size()) + "/5")
            << " ";
        for (size_t i = 0; i < rec->activities.size(); ++i) {
            std::cout << rec->activities[i];
            if (i != rec->activities.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
}

void AttendanceList::listPresentAndCheckedOutStudentsWithActivity() {
    std::vector<AttendanceRecord*> presentList, checkedOutList;
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->status == "Present") {
            // Not checked out yet
            if (temp->timeOut.empty() || temp->timeOut == "-")
                presentList.push_back(temp);
            else
                checkedOutList.push_back(temp);
        }
        temp = temp->next;
    }
    std::sort(presentList.begin(), presentList.end(), compareByID);
    std::sort(checkedOutList.begin(), checkedOutList.end(), compareByID);

    // Display Present (not checked out)
    std::cout << "Students Present (NOT yet checked out) :\n";
    std::cout << std::left
        << std::setw(8) << "ID"
        << std::setw(15) << "Child Name"
        << std::setw(8) << "Status"
        << "Activities\n";
    for (auto* rec : presentList) {
        std::cout << std::left
            << std::setw(8) << rec->recordID
            << std::setw(15) << rec->childName
            << std::setw(8) << (std::to_string(rec->activities.size()) + "/5")
            << " ";
        for (size_t i = 0; i < rec->activities.size(); ++i) {
            std::cout << rec->activities[i];
            if (i != rec->activities.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }
    // Display Checked-Out (still "Present" status)
    if (!checkedOutList.empty()) {
        std::cout << "\nStudents already checked out today :\n";
        std::cout << std::left
            << std::setw(8) << "ID"
            << std::setw(15) << "Child Name"
            << std::setw(8) << "Status"
            << std::setw(12) << "Time Out"
            << "Activities\n";
        for (auto* rec : checkedOutList) {
            std::cout << std::left
                << std::setw(8) << rec->recordID
                << std::setw(15) << rec->childName
                << std::setw(8) << (std::to_string(rec->activities.size()) + "/5")
                << std::setw(12) << rec->timeOut
                << " ";
            for (size_t i = 0; i < rec->activities.size(); ++i) {
                std::cout << rec->activities[i];
                if (i != rec->activities.size() - 1)
                    std::cout << ", ";
            }
            std::cout << std::endl;
        }
    }
}

void AttendanceList::listAttendanceSimple() {
    std::vector<AttendanceRecord*> list;
    AttendanceRecord* temp = head;
    while (temp) {
        list.push_back(temp);
        temp = temp->next;
    }
    std::sort(list.begin(), list.end(), compareByID);
    std::cout << std::left
        << std::setw(8) << "ID"
        << std::setw(15) << "Child Name"
        << std::setw(12) << "Status"
        << std::setw(12) << "Time In"
        << std::setw(12) << "Time Out"
        << std::setw(15) << "Guardian" << std::endl;
    for (auto* rec : list) {
        std::cout << std::left
            << std::setw(8) << rec->recordID
            << std::setw(15) << rec->childName
            << std::setw(12) << rec->status
            << std::setw(12) << rec->timeIn
            << std::setw(12) << rec->timeOut
            << std::setw(15) << rec->guardian << std::endl;
    }
}

AttendanceRecord* AttendanceList::findRecordByID(const std::string& id) {
    AttendanceRecord* temp = head;
    while (temp) {
        if (temp->recordID == id)
            return temp;
        temp = temp->next;
    }
    return nullptr;
}

void AttendanceList::displayAttendanceSummaryReport() {
    using std::cout; using std::endl; using std::setw; using std::left;
    while (true) {
        // 1. Show summary menu
        cout << "\n======= Attendance Summary Report =======\n";
        cout << "Choose sort/filter type:\n";
        cout << " 1. Sort by Record ID (Default)\n";
        cout << " 2. Sort by Activity Completion Status\n";
        cout << " 3. Sort by Age Group\n";
        cout << " 4. Sort by Check-in Time\n";
        cout << " 0. Back to menu\n";
        cout << "Enter choice: ";
        int choice;
        std::cin >> choice;
        if (choice == 0) {
            cout << "Returning to menu...\n";
            break;
        }

        // Only show report if permanent list is not empty!
        std::vector<AttendanceRecord*> records;
        AttendanceRecord* temp = head;
        while (temp != nullptr) {
            records.push_back(temp);
            temp = temp->next;
        }
        if (records.empty()) {
            cout << "\nNo summary available. Please confirm & save records to permanent list first.\n";
            break;
        }

        // Sorting logic
        if (choice == 1) {
            std::sort(records.begin(), records.end(), [](AttendanceRecord* a, AttendanceRecord* b) {
                return a->recordID < b->recordID;
            });
        } else if (choice == 2) {
            std::sort(records.begin(), records.end(), [](AttendanceRecord* a, AttendanceRecord* b) {
                return a->activities.size() > b->activities.size();
            });
        } else if (choice == 3) {
            std::sort(records.begin(), records.end(), [](AttendanceRecord* a, AttendanceRecord* b) {
                return a->ageGroup < b->ageGroup;
            });
        } else if (choice == 4) {
            std::sort(records.begin(), records.end(), [](AttendanceRecord* a, AttendanceRecord* b) {
                return a->timeIn < b->timeIn;
            });
        } else {
            cout << "Invalid choice. Try again.\n";
            continue;
        }

        // Table header (add column if needed)
        cout << "\n";
        cout << setw(8) << "RecordID"
             << setw(15) << "Child Name";
        if (choice == 3) cout << setw(12) << "Age Group";
        if (choice == 4) cout << setw(12) << "Time In";
        cout << setw(10) << "Status"
             << "Activities Completed" << endl;
        cout << "-------------------------------------------------------------------------------\n";

        int present = 0, absent = 0, total = 0;
        for (auto* rec : records) {
            total++;
            if (rec->status == "Present") present++;
            else absent++;
            std::string acts;
            for (size_t i = 0; i < rec->activities.size(); ++i) {
                acts += rec->activities[i];
                if (i != rec->activities.size() - 1) acts += ", ";
            }
            cout << setw(8) << rec->recordID
                 << setw(15) << rec->childName;
            if (choice == 3) cout << setw(12) << rec->ageGroup;
            if (choice == 4) cout << setw(12) << rec->timeIn;
            cout << setw(10) << (std::to_string(rec->activities.size()) + "/5") << "  ";
            cout << acts << endl;
        }
        // Vertical summary
        cout << "\nSummary Report of the Day:" << endl;
        cout << "  Total students  : " << total << endl;
        cout << "  Present         : " << present << endl;
        cout << "  Absent          : " << absent << endl;
        cout << "-------------------------------------------\n";
        // Loop again to let user try another sort or exit
    }
}
