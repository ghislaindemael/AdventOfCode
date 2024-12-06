#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

void initializeOrderGrid(bool(* arrs)[100], const std::string& inputFilename) {
    std::ifstream inputFile(inputFilename);
    std::string line;
    while (std::getline(inputFile, line) && !line.empty()) {
        std::istringstream iss(line);
        int num1 = stoi(line.substr(0, 2));
        int num2 = stoi(line.substr(3, 2));
        arrs[num1][num2] = true;
    }
}

void initializeUpdateVectors(auto& updates, const std::string& inputFilename) {
    std::ifstream inputFile(inputFilename);
    std::string line;
    while (std::getline(inputFile, line) && !line.empty()) { /* Pass */ }
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string token;
        std::vector<int> row;
        while (std::getline(iss, token, ',')) {
            row.push_back(std::stoi(token));
        }
        updates.push_back(row);
    }

}

bool isOrderedRow(const bool(* grid)[100], const std::vector<int>& row) {
    for(int i = 0; i < row.size() - 1; i++) {
        if(!grid[row[i]][row[i + 1]]) {
            return false;
        }
    }
    return true;
}

void orderRow(const bool(* grid)[100], std::vector<int>& row) {
    while(!isOrderedRow(grid, row)) {
        for(int i = 0; i < row.size() - 1; i++) {
            if(!grid[row[i]][row[i + 1]]) {
                int temp = row[i];
                row[i] = row[i + 1];
                row[i + 1] = temp;
            }
        }
    }
}

int main() {

    bool grid[100][100] {false};
    initializeOrderGrid(grid, "input.txt");

    std::vector<std::vector<int>> updates;
    initializeUpdateVectors(updates, "input.txt");

    int validUpdatesCenterCount { 0 };
    int invalidUpdatesCenterCount { 0 };

    for(auto& row : updates) {
        if(isOrderedRow(grid, row)){
            validUpdatesCenterCount += row[ceil(row.size() / 2)];
        } else {
            orderRow(grid, row);
            invalidUpdatesCenterCount += row[ceil(row.size() / 2)];
        }
    }

    std::cout << "Sum of valid updates centers : " << validUpdatesCenterCount << std::endl;
    std::cout << "Sum of invalid updates centers : " << invalidUpdatesCenterCount << std::endl;

    return 0;
}
