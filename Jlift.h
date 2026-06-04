#ifndef LIFT_H
#define LIFT_H

#include<iostream>
#include<vector>

#define LIFT_A 6
#define LIFT_B 2
#define LIFT_C 0
#define LIFT_D 3

using namespace std;

class Lift{
    public:
        int LiftNumber;
        int CurrentFloor;
        int NextFloor;
        int ReachFloor;
        int Mode;

        vector<int> v;

        void Display(){
            cout<<"Lift: "<<LiftNumber<<endl;
        }
};

/*bool compareLC(Lift a,Lift b){
    return(a.CurrentFloor<b.CurrentFloor);
}*/

#endif