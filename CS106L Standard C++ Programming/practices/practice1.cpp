#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

const int elementToFind =5;

void printVec(const vector<int>&vec){
    for(auto elem:vec){
        cout<<elem<<' ';
    }
    cout<<endl;
}



int main(){
    vector<int> target{3,2,4,2,4,6,4,5,2,2,4,5,3,2,6,7,8,2,5,6,4,2,};
    cout<<"before sort"<<endl;
    printVec(target);
    std::sort(target.begin(),target.end());
    cout<<"after sort"<<endl;
    printVec(target);

    auto ite=std::find(target.begin(),target.end(),elementToFind);
    if(ite==target.end()){
        cout<<"element not found"<<endl;
    }else{
        cout<<"find element"<<' '<<elementToFind<<endl;
    }

    set<int> mySet{1,44,24,53,2};
    set<int>::iterator iter=mySet.lower_bound(-1);
    set<int>::iterator end=mySet.upper_bound(44);
    while(iter!=end){
        cout<<*iter<<endl;
        iter++;
    }
    return 0;
}