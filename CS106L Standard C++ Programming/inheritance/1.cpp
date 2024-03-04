#include <iostream>
#include <string>
using namespace std;

class drink{
    public:
    drink()=default;
    drink(string flavor):_flavor(flavor){}
    virtual void make()=0;
    virtual ~drink()=default;
    private:
    string _flavor;
};

class tea :public drink{
    public: tea()=default;
    tea(string flavor):drink(flavor){}
    ~tea()=default;
    void make(){
        cout<<"made tea from "<<endl;
    }
};

int main(){
    tea t("red");
    t.make();
    return 0;
}