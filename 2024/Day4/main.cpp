#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#define SIZE 140

std::vector<std::vector<char> > extractGrid(const std::string& inputFilename) {
    std::ifstream inputFile(inputFilename);
    std::vector<std::vector<char> > grid;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::vector<char> row;
        char num;
        while (iss >> num) {
            row.push_back(num);
        }
        grid.push_back(row);
    }
    return grid;
}

bool checkForXMASLetter(auto& grid, int row, int col, int dir, char letter) {
    // Must break out if letter is invalid
    if(grid[row][col] != letter) {
        return false;
    }
    char nextLetter = 'M';
    switch (letter) {
        case 'X':
            nextLetter = 'M';
            break;
        case 'M':
            nextLetter = 'A';
            break;
        case 'A':
            nextLetter = 'S';
            break;
        case 'S':
            return grid[row][col] == 'S';
        default: ;
            return false;
    }

    switch (dir) {
        case 0:
            return checkForXMASLetter(grid, row, col + 1, dir, nextLetter);
        case 1:
            return checkForXMASLetter(grid, row + 1, col + 1, dir, nextLetter);
        case 2:
            return checkForXMASLetter(grid, row + 1, col, dir, nextLetter);
        case 3:
            return checkForXMASLetter(grid, row + 1, col - 1, dir, nextLetter);
        case 4:
            return checkForXMASLetter(grid, row , col - 1, dir, nextLetter);
        case 5:
            return checkForXMASLetter(grid, row - 1, col - 1, dir, nextLetter);
        case 6:
            return checkForXMASLetter(grid, row - 1, col, dir, nextLetter);
        case 7:
            return checkForXMASLetter(grid, row - 1, col + 1, dir, nextLetter);
        default:
            return false;
    }
}

int countStraightXMAS(auto& grid, int row, int col) {
    if (grid[row][col] != 'X') {
        return 0;
    }

    int validDirs = 0xFF;

    if (col >= SIZE - 3) {
        validDirs &= 0b01111100;
    }
    if (row >= SIZE - 3) {
        validDirs &= 0b11110001;
    }
    if (col < 3) {
        validDirs &= 0b11000111;
    }
    if (row < 3) {
        validDirs &= 0b00011111;
    }

    int count = 0;
    for (int k = 0; k < 8; k++) {
        if (validDirs & 1 << k) {
            count += checkForXMASLetter(grid, row, col, k, 'X');
        }
    }

    return count;
}


bool isCellCenterOfXMAS(auto& grid, int row, int col) {
    if(grid[row][col] != 'A' || row == 0 || col == 0 || row == SIZE - 1 || col == SIZE - 1) {
        return false;
    }
    int sum = 0;
    sum += grid[row - 1][col - 1];
    sum += grid[row - 1][col + 1];
    sum += grid[row + 1][col - 1];
    sum += grid[row + 1][col + 1];

    /*
     Given M = 77 & S = 83, 2M + 2S = 320 and they must not be on each diagonal
     */
    return sum == 320 && grid[row-1][col - 1] != grid[row + 1][col + 1];
}

int main() {

    const auto grid = extractGrid("input.txt");

    int validStraightXMASs = 0;
    int validCrossXMASs = 0;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            validStraightXMASs += countStraightXMAS(grid, row, col);
            validCrossXMASs += isCellCenterOfXMAS(grid, row, col);
        }
    }

    std::cout << "Valid straight XMASs found: " << validStraightXMASs << std::endl;
    std::cout << "Valid cross XMASs found: " << validCrossXMASs << std::endl;
    return 0;
}
