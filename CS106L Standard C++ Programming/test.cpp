#include <string>
#include <iostream>
using namespace std;

int main(){
    string A="hello";
    const char B[]="hello";
    if(A==B){
        cout<<"yes";
    }
    return 0;
}
