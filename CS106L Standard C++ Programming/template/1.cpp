#include <vector>
#include <iostream>

using namespace std;

template <typename T>

pair<T,T> my_minMax(T a,T b){
    return a<b?make_pair(a,b):make_pair(b,a);
}
template <typename T>

void print_pairs(T min,T max){
    cout<<min<<' '<<max<<endl;
}


class origin{
    public:
    int firstFun(){

    }
    struct firstStruct{
        int ele_1;
        int ele_2;
    };
    private:
    int private_fun(){

    }
    protected:
    int protected_fun(){

    }
};

class second_class:public origin{
    public:
    int secondFun(){

    }
};


int main(){
    auto[min,max]=my_minMax(111,3);
    print_pairs(min,max);
    return 0;
}

