#pragma once
#include "Jperson.h"
#include"Jlift.h"
#include <vector>
#include <queue>

using namespace std;

// TODO: Jyothiradithya - Implement Controller class
// Responsibilities:
//   - Store multiple lift definitions
//   - Accept passenger input
//   - Find nearest available lift
//   - Assign requests to lifts
//   - Emergency queue logic (priority queue)

/*class Controller {
public:
    int numLifts;
    int numFloors;

    Controller(int lifts, int floors);

    void addPassengerRequest(Person p);
    int findNearestLift(int floor);
    void assignRequest(Person p);
    void displayQueue() const;
};*/
class Controller{
    public:
    priority_queue<int> pq_1;
    vector<Person> people;
    void assign_Lift(Person p1){
        vector<int> lv = {1, 2, 3, 4};
        Lift l[4];
        bool emergency=true;
        int emLift=-1;
        while(true){
            p1.input();
            int lift_fl;
            l[0].CurrentFloor=LIFT_A;
            l[1].CurrentFloor=LIFT_B;
            l[2].CurrentFloor=LIFT_C;
            l[3].CurrentFloor=LIFT_D;
            l[0].LiftNumber=1;
            l[1].LiftNumber=2;
            l[2].LiftNumber=3;
            l[3].LiftNumber=4;
            vector<int> v;
            people.push_back(p1);
            for(int i=0;i<4;i++){
                v.push_back(l[i].CurrentFloor);
            }
            int curr=9999;
            int assignedIndex=-1;
            if(p1.Priority==EMERGENCY){
                for(int i=0;i<4;i++){
                    if(l[i].Available==true){
                        int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                        if(diff < curr){
                            curr = diff;
                            lift_fl = l[i].LiftNumber;
                            assignedIndex = i;
                        }
                    }
                }
                for(int i=0;i<4;i++){
                    if(lift_fl==l[i].LiftNumber){
                        l[i].Available=false;
                    }
                }
                emLift=lift_fl;
                bool found = false;
                for(int i = 0; i < lv.size(); i++){
                    if(lv[i] == emLift){
                        lv.erase(lv.begin() + i);
                        found = true;
                        break;
                    }
                }
                cout<<"Emergency mode assained for lift: "<<emLift<<endl;
                cout << "Take Lift " << lift_fl << endl;
            }
            else if(p1.Priority==DOCTOR || p1.Priority==STAFF){
                if(emLift==-1){
                    for(int i=0;i<4;i++){
                        if(l[i].Available==true){
                            int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                            if(diff < curr){
                                curr = diff;
                                lift_fl = l[i].LiftNumber;
                            }
                        }
                    }
                    cout << "Take Lift " << lift_fl << endl;
                }

                else{
                    cout<<"Take Emergency Lift!!!"<<endl;
                    cout << "Take Lift " << emLift << endl;
                }
            }
            else if(p1.Priority!=EMERGENCY){
                for(int i=0;i<4;i++){
                    if(l[i].Available==true){
                        int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                        if(diff < curr){
                            curr = diff;
                            lift_fl = l[i].LiftNumber;
                        }
                    }
                }
                cout<<"Avaliable Lifts: "<<endl;
                for(int lift: lv){
                    cout<<lift<<" ";
                }
                cout<<endl;
                cout << "Take Lift " << lift_fl << endl;
            }
            cout<<"Still Emergency??... (1/0) ";
            int em;
            cin>>em;
            if(em==0){
                break;
            }
        }
    }
};
