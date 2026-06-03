#include "Person.h"

// TODO: Jyothiradithya - Implement Person class methods

Person::Person(string name, PassengerType type, int src, int dest)
    : name(name), type(type), sourceFloor(src), destFloor(dest) {
    priority = static_cast<int>(type);
}

int Person::getPriority() const {
    return priority;
}

void Person::display() const {
    cout << "Passenger: " << name
         << " | Priority: " << priority
         << " | From: " << sourceFloor
         << " | To: " << destFloor << endl;
}
