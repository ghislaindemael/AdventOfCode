#include <algorithm>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <boost/functional/hash.hpp>

struct Stone {
    int64_t number = -1;
    int64_t leftChild = -1;
    int64_t rightChild = -1;
    int64_t descendants = 0;

    Stone() = default;

    explicit Stone(const int64_t num) : number(num) {
    }

    bool noChildren() const {
        return leftChild + rightChild == -2;
    }

    int64_t getNumOfChildren() {
        if (noChildren()) {
            return 1;
        }
        return descendants;
    }
};

void printStones(const std::vector<Stone>& stones) {
    for (const auto& stone: stones) {
        std::cout << stone.number << " ";
    }
    std::cout << std::endl;
}

bool stoneNumberExists(const std::vector<Stone>& stones, const int64_t number) {
    bool exists = std::ranges::any_of(stones, [number](const Stone& stone) {
        return stone.number == number;
    });

    if (exists) {
        return true;
    }
    //std::cout << number << " not in stones" << std::endl;
    return false;
}

void addStoneIfNotPresent(std::vector<Stone>& stones, const int64_t number) {
    if (number < 0 || stoneNumberExists(stones, number)) {
        return;
    }
    stones.emplace_back(number);
}

void blink(std::vector<Stone>& stones) {
    int stonesLength = stones.size();

    for (int i = 0; i < stonesLength; i++) {
        Stone& stone = stones[i];
        //std::cout << "Blinking stone " << stone.number << std::endl;
        if (stone.noChildren()) {
            if (stone.number == 0) {
                stone.leftChild = 1;
            } else if (int digitCount = static_cast<int>(std::to_string(stone.number).length()); digitCount % 2 == 0) {
                const int halfDigits = digitCount / 2;
                std::string numberStr = std::to_string(stone.number);
                std::string leftPart = numberStr.substr(0, halfDigits);
                std::string rightPart = numberStr.substr(halfDigits);

                try {
                    stone.leftChild = std::stoll(leftPart);
                    stone.rightChild = std::stoll(rightPart);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid number format during child conversion for stone " << stone.number <<
                            std::endl;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range error during child conversion for stone " << stone.number << std::endl;
                }
            } else {
                stone.leftChild = stone.number * 2024;
            }

            addStoneIfNotPresent(stones, stones[i].leftChild);
            addStoneIfNotPresent(stones, stones[i].rightChild);
        }
    }
}

int64_t exploreNode(std::vector<Stone>& stones, int64_t number, int level, int maxLevel) {
    if (number == -1) {
        return 0;
    }

    if (level == maxLevel) {
        return 1;
    }

    //std::cout << "Level " << level  << ", exploring " << number << std::endl;
    int64_t leftChild = -1;
    int64_t rightChild = -1;
    for (Stone& stone: stones) {
        if (stone.number == number) {
            leftChild = stone.leftChild;
            rightChild = stone.rightChild;
            break;
        }
    }
    //std::cout << "Children: " << leftChild << " " << rightChild << std::endl;

    int64_t count = 0;
    count += exploreNode(stones, leftChild, level + 1, maxLevel);
    count += exploreNode(stones, rightChild, level + 1, maxLevel);
    return count;
}

void migrateStonesToMap(const std::vector<Stone>& stones, std::unordered_map<int64_t, Stone>& stoneMap) {
    for (const auto& stone: stones) {
        stoneMap[stone.number] = stone;
    }
}

int64_t exploreNode(std::unordered_map<int64_t, Stone>& stoneMap, int64_t number, int level, int maxLevel,
                    std::unordered_map<std::pair<int64_t, int>, int64_t, boost::hash<std::pair<int64_t, int> > >&
                    memo) {
    if (number == -1) {
        return 0;
    }

    if (level == maxLevel) {
        return 1;
    }

    const auto memoKey = std::make_pair(number, level);
    if (memo.contains(memoKey)) {
        return memo[memoKey];
    }

    const auto it = stoneMap.find(number);
    if (it == stoneMap.end()) {
        return 0;
    }

    const Stone& stone = it->second;
    int64_t count = 0;
    count += exploreNode(stoneMap, stone.leftChild, level + 1, maxLevel, memo);
    count += exploreNode(stoneMap, stone.rightChild, level + 1, maxLevel, memo);

    memo[memoKey] = count;
    return count;
}


int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file";
        return 1;
    }

    std::string line;
    std::getline(inputFile, line);
    std::vector<Stone> stones = std::vector<Stone>();

    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        try {
            int number = std::stoll(token);
            stones.push_back(Stone(number));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number format: " << token << std::endl;
        }
    }

    printStones(stones);

    int initialStoneCount = stones.size();
    int numberOfBlinks = 75;

    for (int i = 0; i < numberOfBlinks; i++) {
        blink(stones);
    }

    /*
    std::cout << "Reducing stones" << std::endl;
    int64_t stoneCount = 0;
    for(int j = 0; j < initialStoneCount; j++) {
        //stoneCount += exploreNode(stones, stones[j].number, 0, numberOfBlinks);
    }
    std::cout << numberOfBlinks << " blinks: " << stoneCount << " stones on last row." << std::endl;
    */

    std::unordered_map<int64_t, Stone> stoneMap;
    std::cout << "Migrating stones to unordered_map" << std::endl;
    migrateStonesToMap(stones, stoneMap);

    std::unordered_map<std::pair<int64_t, int>, int64_t, boost::hash<std::pair<int64_t, int> > > memo;

    int64_t stoneCount = 0;
    for (int j = 0; j < initialStoneCount; j++) {
        stoneCount += exploreNode(stoneMap, stones[j].number, 0, numberOfBlinks, memo);
    }
    std::cout << numberOfBlinks << " blinks: " << stoneCount << " stones on last row." << std::endl;


    return 0;
}
