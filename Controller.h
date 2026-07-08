#pragma once
#include "Person.h"
#include "Lift.h"
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

class Controller{
    public:
    priority_queue<int> pq_1;
    vector<Person> people;

    Lift l[4] = {
        Lift(1, 6),
        Lift(2, 2),
        Lift(3, 0),
        Lift(4, 3)
    };

    vector<int> lv = {1, 2, 3, 4};
    int emLift = -1;

    void assign_Lift(Person p1){
        p1.input();
        people.push_back(p1);
        pq_1.push(p1.Priority);

        int lift_fl;
        int curr=9999;
        int assignedIndex=-1;

        if(p1.Priority==EMERGENCY){
            for(int i=0;i<4;i++){
                if(l[i].isAvailable()==true){
                    int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                    if(diff < curr){
                        curr = diff;
                        lift_fl = l[i].LiftID;
                        assignedIndex = i;
                    }
                }
            }

            if(assignedIndex == -1){
                return;
            }

            emLift=lift_fl;
            for(int i = 0; i < (int)lv.size(); i++){
                if(lv[i] == emLift){
                    lv.erase(lv.begin() + i);
                    break;
                }
            }

            l[assignedIndex].activateEmergency(p1.DestFloor);
            cout<<"Emergency mode assained for lift: "<<emLift<<endl;
            cout << "Take Lift " << lift_fl << endl;

            l[assignedIndex].servePassengers(p1.currntFloor, p1.DestFloor, PersonType::EmergencyPatient);
            releaseEmergency();
        }

        else if(p1.Priority==DOCTOR || p1.Priority==STAFF){
            PersonType ptype = (p1.Priority==DOCTOR) ? PersonType::Doctor : PersonType::Staff;

            if(emLift==-1){
                for(int i=0;i<4;i++){
                    if(l[i].isAvailable()==true){
                        int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                        if(diff < curr){
                            curr = diff;
                            lift_fl = l[i].LiftID;
                            assignedIndex = i;
                        }
                    }
                }
                if(assignedIndex == -1){
                    return;
                }
                cout << "Take Lift " << lift_fl << endl;
            }
            else{
                lift_fl = emLift;
                assignedIndex = -1;
                for(int i=0;i<4;i++){
                    if(l[i].LiftID == emLift){ assignedIndex = i; break; }
                }
                cout<<"Take Emergency Lift!!!"<<endl;
                cout << "Take Lift " << emLift << endl;
            }

            l[assignedIndex].servePassengers(p1.currntFloor, p1.DestFloor, ptype);
        }

        else{
            for(int i=0;i<4;i++){
                if(l[i].isAvailable()==true){
                    int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                    if(diff < curr){
                        curr = diff;
                        lift_fl = l[i].LiftID;
                        assignedIndex = i;
                    }
                }
            }
            if(assignedIndex == -1){
                return;
            }
            cout<<"Avaliable Lifts: "<<endl;
            for(int lift: lv){
                cout<<lift<<" ";
            }
            cout<<endl;
            cout << "Take Lift " << lift_fl << endl;

            l[assignedIndex].servePassengers(p1.currntFloor, p1.DestFloor, PersonType::Normal);
        }
    }

    void releaseEmergency(){
        if(emLift == -1) return;
        lv.push_back(emLift);
        emLift = -1;
    }

    void displayAllLifts() const {
        for(int i=0;i<4;i++) l[i].displayStatus();
    }
};
