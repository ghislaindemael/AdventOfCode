#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>

constexpr int dRow[] = {-1, 0, 1, 0};
constexpr int dCol[] = {0, 1, 0, -1};
const char *directionNames[] = {"U", "R", "D", "L"};

class Cell {
    int row;
    int col;
    bool isWall = false;
    bool isProtected = false;
    bool onBestPath = false;
    std::vector<int> directionCosts;

public:
    Cell(const int r, const int c, const bool w) : row(r), col(c), isWall(w),
                                                   directionCosts(2, std::numeric_limits<int>::max()) {
    }

    [[nodiscard]] bool isWallCell() const {
        return isWall;
    }

    void setWall() {
        isWall = true;
    }

    [[nodiscard]] bool getProtected() const {
        return isProtected;
    }

    void setProtected() {
        isProtected = true;
    }

    void setDirectionCost(int dir, int newCost) {
        directionCosts[dir] = newCost;
    }

    [[nodiscard]] int getDirectionCost(int dir) const {
        return directionCosts[dir];
    }

    [[nodiscard]] bool getOnBestPath() const { return onBestPath; }

    void setOnBestPath(bool value) { onBestPath = value; }

    [[nodiscard]] int countWallsAround(const std::vector<std::vector<Cell>>& maze) const {
        int count = 0;
        for (int dir = 0; dir < 4; dir++) {
            int row = this->row + dRow[dir], col = this->col + dCol[dir];
            if (row >= 0 && row < maze.size() && col >= 0 && col < maze.size() && maze[row][col].isWallCell()) {
                ++count;
            }
        }
        return count;
    }
};

struct State {
    int row, col, direction, cost;

    bool operator>(const State& other) const { return cost > other.cost; }
};

std::vector<std::vector<Cell>> extractMazeFromFile(const std::string& inputFilename) {
    std::ifstream inputFile(inputFilename);
    std::vector<std::vector<Cell>> maze;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::vector<Cell> row;
        int rowNum = static_cast<int>(maze.size());
        char cell;
        while (iss >> cell) {
            row.emplace_back(rowNum, row.size(), cell == '#');
        }
        maze.push_back(row);
    }
    maze[maze.size() - 2][1].setProtected();
    maze[1][maze.size() - 2].setProtected();
    return maze;
}

void printMaze(const std::vector<std::vector<Cell>>& maze) {
    for (const auto& row: maze) {
        for (const auto& col: row) {
            std::cout << (col.isWallCell() ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

void printMazeMinCostToReach(const std::vector<std::vector<Cell>>& maze) {
    for (int i = 0; i < maze.size(); ++i) {
        printf("%02d:", i);
        for (int j = 0; j < maze[0].size(); ++j) {
            if (maze[i][j].isWallCell()) {
                std::cout << "#|";
            } else {
                for (int d = 0; d < 2; d++) {
                    int cost = maze[i][j].getDirectionCost(d);
                    if (cost < 100000) {
                        std::cout << cost;
                    } else {
                        std::cout << ".";
                    }
                }
                std::cout << '|';
            }
        }
        std::cout << std::endl;
    }
}

void closeEmptyPaths(std::vector<std::vector<Cell>>& maze, int r, int c) {
    const int size = maze.size();

    auto isValid = [&](int row, int col) {
        return row >= 0 && row < size && col >= 0 && col < size && !maze[row][col].isWallCell() && !maze[row][col].
               getProtected();
    };

    if (!isValid(r, c)) return;

    if (maze[r][c].countWallsAround(maze) >= 3) {
        maze[r][c].setWall();

        for (int dir = 0; dir < 4; dir++) {
            closeEmptyPaths(maze, r + dRow[dir], c + dCol[dir]);
        }
    }
}

void closeIfNotReachable(std::vector<std::vector<Cell>>& maze) {
    const int size = maze.size();
    auto isValid = [&](int row, int col) {
        return row >= 0 && row < size && col >= 0 && col < size && !maze[row][col].isWallCell() && !maze[row][col].
               getProtected();
    };

    for (int r = 0; r < maze.size(); r++) {
        for (int c = 0; c < maze[0].size(); c++) {
            if (!isValid(r, c)) continue;
            Cell& cell = maze[r][c];
            if (cell.getDirectionCost(0) + cell.getDirectionCost(1) == -2) {
                cell.setWall();
            }
        }
    }
}

void simplifyMaze(std::vector<std::vector<Cell>>& maze) {
    int size = maze.size();
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (!maze[r][c].isWallCell() && !maze[r][c].getProtected()) {
                closeEmptyPaths(maze, r, c);
            }
        }
    }
}

// TODO: FIX THE PATHFINDING COSTS
int findLeastExpensivePath(std::vector<std::vector<Cell>>& maze) {
    int size = maze.size();
    auto isValid = [&](int r, int c) {
        return r >= 0 && r < size && c >= 0 && c < size && !maze[r][c].isWallCell();
    };

    std::priority_queue<State, std::vector<State>, std::greater<>> pq;

    pq.push({size - 2, 1, 0, 0});
    pq.push({size - 2, 1, 1, 1000});
    maze[size - 2][1].setDirectionCost(0, 0);
    maze[size - 2][1].setDirectionCost(1, 1000);

    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        if (curr.row == 1 && curr.col == size - 2) {
            return std::min(
                maze[curr.row][curr.col].getDirectionCost(0),
                maze[curr.row][curr.col].getDirectionCost(1)
            );
        }

        for (int dir = 0; dir < 4; ++dir) {
            int newRow = curr.row + dRow[dir];
            int newCol = curr.col + dCol[dir];

            if (isValid(newRow, newCol)) {
                int newDirection = (dir % 2 == 0) ? 1 : 0;
                int turnCost = (curr.direction == newDirection) ? 1 : 1001;
                int newCost = curr.cost + turnCost;

                if (maze[newRow][newCol].getDirectionCost(newDirection) > newCost) {
                    maze[newRow][newCol].setDirectionCost(newDirection, newCost);
                    pq.push({newRow, newCol, newDirection, newCost});
                }
            }
        }
    }

    return -1;
}

// TODO: FIX PATH MARKING
void markBestPaths(std::vector<std::vector<Cell>>& maze, int row, int col) {
    if (row < 0 || row >= maze.size() || col < 0 || col >= maze[0].size() || maze[row][col].isWallCell()) {
        return;
    }

    if (maze[row][col].getOnBestPath()) {
        return;
    }

    bool isOnBestPath = false;

    for (int dir = 0; dir < 4; dir++) {
        int nextRow = row + dRow[dir];
        int nextCol = col + dCol[dir];

        if (nextRow >= 0 && nextRow < maze.size() && nextCol >= 0 && nextCol < maze[0].size() && !maze[nextRow][nextCol]
            .isWallCell()) {
            int currentDirection = (dir % 2 == 0) ? 1 : 0;
            int oppositeDirection = 1 - currentDirection;

            int currentCost = maze[row][col].getDirectionCost(currentDirection);
            int neighborCost = maze[nextRow][nextCol].getDirectionCost(currentDirection);

            int neighborOppositeCost = maze[nextRow][nextCol].getDirectionCost(oppositeDirection);

            if ((neighborCost != -1 && neighborCost < currentCost && currentCost - neighborCost == 1) ||
                (neighborOppositeCost != -1 && neighborOppositeCost < currentCost && currentCost - neighborOppositeCost
                 == 1001)) {
                isOnBestPath = true;
                break;
            }
        }
    }

    if (!isOnBestPath) {
        return;
    }

    maze[row][col].setOnBestPath(true);

    for (int dir = 0; dir < 4; dir++) {
        int nextRow = row + dRow[dir];
        int nextCol = col + dCol[dir];

        if (nextRow >= 0 && nextRow < maze.size() && nextCol >= 0 && nextCol < maze[0].size() && !maze[nextRow][nextCol]
            .isWallCell()) {
            int currentDirection = (dir % 2 == 0) ? 1 : 0;
            int oppositeDirection = 1 - currentDirection;

            int currentCost = maze[row][col].getDirectionCost(currentDirection);
            int neighborCost = maze[nextRow][nextCol].getDirectionCost(currentDirection);

            int neighborOppositeCost = maze[nextRow][nextCol].getDirectionCost(oppositeDirection);

            if ((neighborCost != std::numeric_limits<int>::max() && neighborCost + 1 == currentCost) ||
                (neighborOppositeCost != std::numeric_limits<int>::max() && neighborOppositeCost + 1001 ==
                 currentCost)) {
                markBestPaths(maze, nextRow, nextCol);
            }
        }
    }
}

int countBestPathCells(const std::vector<std::vector<Cell>>& maze) {
    int count = 0;
    for (const auto& row: maze) {
        for (const auto& cell: row) {
            if (cell.getOnBestPath()) {
                count++;
            }
        }
    }
    return count;
}

void saveBestPathToFile(const std::vector<std::vector<Cell>>& maze, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for writing.\n";
        return;
    }

    int bestPathCount = 0;

    for (const auto& row: maze) {
        for (const auto& cell: row) {
            if (cell.isWallCell()) {
                outputFile << "# ";
            } else if (cell.getOnBestPath()) {
                outputFile << "O ";
                bestPathCount++;
            } else {
                outputFile << ". ";
            }
        }
        outputFile << '\n';
    }

    outputFile << "Number of tiles on best path: " << bestPathCount << '\n';

    outputFile.close();
    std::cout << "Grid with best path saved to " << filename << '\n';
}

int main() {

    auto maze = extractMazeFromFile("input.txt");
    simplifyMaze(maze);

    const int size = maze.size();

    if (int minimumCost = findLeastExpensivePath(maze); minimumCost != -1) {
        std::cout << "Least expensive path cost: " << minimumCost << std::endl;
        printMazeMinCostToReach(maze);
        closeIfNotReachable(maze);
        simplifyMaze(maze);

        markBestPaths(maze, 1, maze.size() - 2);

        int count = countBestPathCells(maze);
        std::cout << "Number of tiles on best paths: " << count << '\n';

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (maze[i][j].isWallCell()) {
                    std::cout << "# ";
                } else if (maze[i][j].getOnBestPath()) {
                    std::cout << "O ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << std::endl;
        }

        saveBestPathToFile(maze, "path_output.txt");
    } else {
        std::cout << "No path found." << std::endl;
    }

    return 0;
}
