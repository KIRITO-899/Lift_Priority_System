#include<iostream>
/*#include<vector>
#include<queue>
#include<cmath>*/
#include<cstdlib>
#include<ctime>

#include"Person.h"
#include"Controller.h"


using namespace std;

int main(){
    srand(time(0));
    Controller c;
    while(true){
        Person p;
        c.assign_Lift(p);
        int s;
        cout<<endl;
        cout<<"New Person?? (1/0): ";
        cin>>s;
        if(s==0){
            break;
        }
    }
    return 0;
}