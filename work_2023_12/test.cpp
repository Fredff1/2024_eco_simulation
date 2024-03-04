#include <iostream>
class main_menu{
    private:
    void print(const char* text){
        std::cout<<text;
    }

    public:
    void usr_option(const char input){
        switch(input){
            case 'i':
            print("Hello world\n");
            break;
            default:
            std::cout<<"Failed\n"<<std::endl;
            break;
        }
    }
    
    struct {
        char input1;
        int flag;
    }myInput;
};

int main(){
    main_menu menu1;
    menu1.myInput.flag=0;
    menu1.myInput.input1='i';
    menu1.usr_option(menu1.myInput.input1);
    menu1.myInput.input1='y';
    menu1.usr_option(menu1.myInput.input1);
    return 0;
}