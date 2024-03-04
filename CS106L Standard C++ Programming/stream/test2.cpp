#include <iostream>
#include <sstream>
using namespace std;

int main(){
    ostringstream oss("Hello world");
    cout << oss.str() <<endl;
    oss<<16.9<<" Ounce ";
    cout << oss.str() <<endl;
    oss<<"(pack of "<<12<< ")";
    cout << oss.str() <<endl;
    return 0;
}