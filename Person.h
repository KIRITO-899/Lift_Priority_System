#ifndef PERSON_H
#define PERSON_H

#include<iostream>
#include<vector>
#include<queue>

#define EMERGENCY 100
#define DOCTOR 90
#define STAFF 70
#define NORMAL_CHECK_UP 30
#define VISITOR 20

#define EMERGENCY_ID 213
#define DOCTOR_ID 214
#define STAFF_ID 215
#define NORMAL_CHECK_UP_ID 216 
#define VISITOR_ID 217

using namespace std;

extern priority_queue<int> pq_1;

class Person{
    public:
        int id;
        int currntFloor;
        int DestFloor;
        int Priority;

        void display(){
            cout<<"ID: "<<id<<endl;
            cout<<"Current Floor: "<<currntFloor<<endl;
            cout<<"Destination Floor: "<<DestFloor<<endl;
        }

        void input(){
            cout<<"Your ID: ";
            cin>>id;

            cout<<"Your Current Floor: ";
            cin>>currntFloor;

            cout<<"Your Destination Floor: ";
            cin>>DestFloor;

            if(id==EMERGENCY_ID){
                Priority=EMERGENCY;
            }
            else if(id==DOCTOR_ID){
                Priority=DOCTOR;
            }
            else if(id==STAFF_ID){
                Priority=STAFF;
            }
            else if(id==NORMAL_CHECK_UP_ID){
                Priority=NORMAL_CHECK_UP;
            }
            else{
                Priority=VISITOR;
            }

            pq_1.push(Priority);
        }
};

extern vector<Person> people;

#endif