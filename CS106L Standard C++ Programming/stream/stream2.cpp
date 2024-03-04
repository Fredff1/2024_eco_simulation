#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <string>
using namespace std;



int main(){
    std::string line;
    std::cout << "Enter a line of text:\n";
    std::getline(std::cin, line,'\n');
    std::cout << "You entered: " << line << std::endl;
    return 0;
}