#include "queue.h"
#include <iostream>
using namespace std;

AttendanceQueue::AttendanceQueue() : front(nullptr), rear(nullptr) {}

bool AttendanceQueue::isEmpty() {
    return front == nullptr;
}

bool AttendanceQueue::recordIDExists(const std::string& id) {
    AttendanceRecord* temp = front;
    while (temp != nullptr) {
        if (temp->recordID == id)
            return true;
        temp = temp->next;
    }
    return false;
}

void AttendanceQueue::displayQueueIDs() {
    AttendanceRecord* temp = front;
    while (temp) {
        cout << temp->recordID;
        if (temp->next) cout << ", ";
        temp = temp->next;
    }
    if (!front) cout << "(none)";
}

void AttendanceQueue::enqueue(AttendanceRecord* record) {
    record->next = nullptr;
    if (rear == nullptr) {
        front = rear = record;
    } else {
        rear->next = record;
        rear = record;
    }
    // cout << "\nCheck-in queued for: " << record->childName << endl;
}

AttendanceRecord* AttendanceQueue::dequeue() {
    if (isEmpty()) return nullptr;
    AttendanceRecord* temp = front;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    temp->next = nullptr;
    return temp;
}

void AttendanceQueue::displayQueue() {
    AttendanceRecord* temp = front;
    cout << "\nQueue for Today's Check-ins" << endl;
    while (temp != nullptr) {
        cout << temp->recordID << " | " << temp->childName << endl;
        temp = temp->next;
    }
    if (isEmpty()) cout << "\nQueue is empty." << endl;
}
