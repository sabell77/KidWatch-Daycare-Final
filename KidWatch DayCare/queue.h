#ifndef QUEUE_H
#define QUEUE_H

#include "attendance.h"

class AttendanceQueue {
private:
    AttendanceRecord* front;
    AttendanceRecord* rear;
public:
    AttendanceQueue();
    bool isEmpty();
    bool recordIDExists(const std::string& id);
    void enqueue(AttendanceRecord* record);
    AttendanceRecord* dequeue();
    void displayQueue();
    void displayQueueIDs();
};

#endif
