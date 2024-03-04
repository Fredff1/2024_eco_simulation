#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>
#include <string>
using namespace std;

int stringToInteger(const string& str){
    istringstream iss(str);
    int result;
    iss>>result;
    if(iss.fail()){
        throw domain_error("failed to convert!");
    }
    char ch;
    iss>>ch;
    if(!iss.fail()){
        throw domain_error("illegal input after numbers");
    }
    return result;
}

void writeFile(const std::string& filename) {
    std::ofstream outFile(filename);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    outFile << "Hello, this is a test file.\n";
    outFile << "Here is some more text.\n";

    outFile.close();
}

void readFile(const std::string& filename) {
    std::ifstream inFile(filename);

    if (!inFile.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::cout << line << std::endl;
    }

    inFile.close();
}


int main(){
    std::string validStr = "123";
    try {
        int result = stringToInteger(validStr);
        std::cout << "Valid input (" << validStr << ") converted to integer: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 测试包含额外字符的输入
    std::string invalidStr = "123abc";
    try {
        int result = stringToInteger(invalidStr);
        std::cout << "Invalid input (" << invalidStr << ") converted to integer: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // 测试完全无效的输入
    std::string nonNumericStr = "abc";
    try {
        int result = stringToInteger(nonNumericStr);
        std::cout << "Non-numeric input (" << nonNumericStr << ") converted to integer: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    writeFile("1.txt");
    readFile("1.txt");

    return 0;
}