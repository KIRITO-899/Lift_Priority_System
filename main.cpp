#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<cmath>

#include"Jperson.h"
#include"Jlift.h"

using namespace std;

priority_queue<int> pq_1;

vector<Person> people;


void assign_Lift(Person p1){
    Lift l[4];
    vector<int> lv = {1, 2, 3, 4};
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
                int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                if(diff < curr){
                    curr = diff;
                    lift_fl = l[i].LiftNumber;
                    assignedIndex = i;
                }
            }
            bool found = false;
            for(int i = 0; i < lv.size(); i++){
                if(lv[i] == lift_fl){
                    lv.erase(lv.begin() + i);
                    found = true;
                    break;
                }
            }
            cout<<"Emergency mode assained for lift: "<<lift_fl<<endl;
            cout << "Take Lift " << lift_fl << endl;
        }
        else if(p1.Priority!=EMERGENCY){
            for(int i=0;i<4;i++){
                int diff = abs(p1.currntFloor - l[i].CurrentFloor);
                if(diff < curr){
                    curr = diff;
                    lift_fl = l[i].LiftNumber;
                }
            }
            cout<<"Avaliable Lifts: "<<endl;
            for(int lift: lv){
                cout<<lift<<" ";
            }
            cout<<endl;
            cout << "Take Lift " << lift_fl << endl;
        }
        cout<<"Still Emergency??... ";
        int em;
        cin>>em;
        if(em==0){
            break;
        }
    }
}

int main(){
    while(true){
        Person p;
        assign_Lift(p);
        int c;
        cout<<endl;
        cout<<"New Person?? (1/0): ";
        cin>>c;
        if(c==0){
            break;
        }
    }
    return 0;
}