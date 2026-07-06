#include<iostream>
#include<vector>
#include<queue>
#include<cmath>
#include<cstdlib>
#include<ctime>

#include"Person.h"
#include"Lift.h"

using namespace std;

priority_queue<int> pq_1;

vector<Person> people;

void assign_Lift(Person p1){
    vector<int> lv = {1, 2, 3, 4};
    Lift l[] = {Lift(1,rand()%10), Lift(2,rand()%10), Lift(3,rand()%10), Lift(4,rand()%10)};
    int emLift=-1;
    while(true){
        p1.input();
        int lift_fl = -1; 
        people.push_back(p1);
        int curr=9999;
        if(p1.Priority==EMERGENCY){
            for(int i=0;i<4;i++){
                int diff = abs(p1.currntFloor - l[i].currentFloor);
                if(diff < curr){
                    curr = diff;
                    lift_fl = l[i].lift_id;
                }
            }
            emLift=lift_fl;
            for(int i = 0; i < (int)lv.size(); i++){
                if(lv[i] == emLift){
                    lv.erase(lv.begin() + i);
                    break;
                }
            }
            cout<<"Emergency mode assigned for lift: "<<emLift<<endl;
            cout << "Take Lift " << lift_fl << endl;
        }
        else if(p1.Priority==DOCTOR || p1.Priority==STAFF){
            if(emLift==-1){
                //was looping all 4 lifts — doctor/staff could be assigned the locked emergency lift
                for(int id : lv){
                    for(int i=0;i<4;i++){
                        if(l[i].lift_id==id){
                            int diff = abs(p1.currntFloor - l[i].currentFloor);
                            if(diff < curr){ curr = diff; lift_fl = id; }
                            break;
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
        else{
            //was looping all 4 lifts — normal visitors could be assigned the locked emergency lift
            for(int id : lv){
                for(int i=0;i<4;i++){
                    if(l[i].lift_id==id){
                        int diff = abs(p1.currntFloor - l[i].currentFloor);
                        if(diff < curr){ curr = diff; lift_fl = id; }
                        break;
                    }
                }
            }
            cout<<"Available Lifts: "<<endl;
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

int main(){
    srand(time(0));
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
