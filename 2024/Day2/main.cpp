#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

void printVector(const std::vector<int>& v) {
    for(int i : v){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

bool isValidReport(const std::vector<int> & vector) {

    std::vector<int> copy(vector);
    std::sort(copy.begin(), copy.end());
    bool sortedPos = vector == copy;
    std::reverse(copy.begin(), copy.end());
    bool sortedNeg = vector == copy;

    if(!(sortedPos || sortedNeg)){
        return false;
    }

    for(int i = 0; i < vector.size() - 1; i++) {
        const int diff = abs(vector[i] - vector[i + 1]);
        if(diff < 1 || diff > 3) {
            return false;
        }
    }
    return true;
}

bool isSemiValidReport(const std::vector<int> & vector) {
    for(int i = 0; i < vector.size(); i++) {
        std::vector<int> copy(vector);
        copy.erase(copy.begin() + i);
        if(isValidReport(copy)) {
            return true;
        }
    }
    return false;
}

int main() {

    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file";
        return 1;
    }

    std::string line;

    int validReportCount { 0 };

    while (std::getline(inputFile, line)) {
        std::vector<int> digits;
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            try {
                int number = std::stoi(token);
                digits.push_back(number);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid number format: " << token << std::endl;
            }
        }

        if(isValidReport(digits) || isSemiValidReport(digits)) {
            validReportCount++;
        }

    }

    std::cout << "Valid reports: " << validReportCount << std::endl;

    inputFile.close();
}
