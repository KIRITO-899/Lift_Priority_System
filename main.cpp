#include<iostream>
/*#include<algorithm>
#include<vector>
#include<queue>
#include<cmath>
*/
#include"Jperson.h"
#include"Jlift.h"
#include"Controller.h"

using namespace std;

priority_queue<int> pq_1;

vector<Person> people;

int main(){
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