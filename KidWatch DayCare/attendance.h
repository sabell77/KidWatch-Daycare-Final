#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <string>
#include <vector>
#include <map>

    std::string getAgeGroup(int age);
	extern std::map<std::string, std::string> lastDeletedActivityMap;
	
struct AttendanceRecord {
    std::string recordID;
    std::string childName;
    std::string ageGroup;
    std::string date;
    std::string timeIn;
    std::string timeOut;
    std::string guardian;
    std::string contactNumber;
    std::string status; 
    std::vector<std::string> activities;
    AttendanceRecord* next;
};

class AttendanceList {
public:
    AttendanceRecord* head;
    AttendanceRecord* findRecordByID(const std::string& id);    
    AttendanceList();

    void editRecord(const std::string& recordID);
    bool recordIDExists(const std::string& id);
    bool isPresentRecord(const std::string& id);
    void checkAndReportDuplicates();
    void addRecord(AttendanceRecord* record);
    void display();
    void displayRecordIDs();
    void deleteRecord(const std::string& recordID);
	void linearSearchByName();
    void binarySearchByID(const std::string& recordID);
    void moveAllTo(AttendanceList& dest);
    void addActivity(const std::string& recordID, const std::string& activity);
	void editActivity(const std::string& recordID, int activityIndex, const std::string& newActivity);
	void deleteActivity(const std::string& recordID, int activityIndex);
	void undoDeleteActivity(const std::string& recordID);
	void viewAllActivityStatus();
	void listAttendanceSimple();
	void listPresentAndCheckedOutStudentsWithActivity();
	void displayAttendanceSummaryReport();
	std::vector<AttendanceRecord*> studentsWithUndoActivity();
};
#endif
