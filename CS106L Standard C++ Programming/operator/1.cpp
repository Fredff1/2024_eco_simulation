#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class myClass{
    public:
    int height,width;
    myClass(int height_input,int width_input):height(height_input),width(width_input){
    }

    int areaOfRect(){
        return height*width;
    }

    

};