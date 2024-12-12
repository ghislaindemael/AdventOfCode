#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Block {
    bool empty;
    int id;

    Block(const bool empty, const int id) : empty(empty), id(id) {}
};

struct File {
    int id;
    int size;

    File(const int id, const int size) : id(id), size(size) {}

    bool isEmpty() {
        return id == -1;
    }
};

void printDiskBlocks(const std::vector<Block>& blocks) {
    for (Block block: blocks) {
        if (block.empty) {
            std::cout << '.';
        } else {
            std::cout << block.id;
        }
    }
    std::cout << std::endl;
}

void printDiskFiles(const std::vector<File>& files) {
    for (File file: files) {
        for (int i = 0; i < file.size; i++) {
            if (file.isEmpty()) {
                std::cout << '.';
            } else {
                std::cout << file.id;
            }
        }
    }
    std::cout << std::endl;
}

void initializeDiskBlocks(std::vector<Block>& cells, const std::string& inputFilename) {
    std::ifstream inputFile(inputFilename);
    std::string line;
    std::getline(inputFile, line);
    for (int i = 0; i < line.length(); i++) {
        if (i % 2 == 0) {
            int count = line[i] - 48;
            for (int j = 0; j < count; j++) {
                cells.emplace_back(false, i / 2);
            }
        } else {
            int count = line[i] - 48;
            for (int j = 0; j < count; j++) {
                cells.emplace_back(true, -1);
            }
        }
    }
}

void initializeDiskFiles(std::vector<File>& files, const std::string& inputFilename) {
    std::ifstream inputFile(inputFilename);
    std::string line;
    std::getline(inputFile, line);
    for (int i = 0; i < line.length(); i++) {
        int count = line[i] - 48;
        files.emplace_back(i % 2 == 0 ? i / 2 : -1, count);
    }
}

void reduceDiskByBlocks(std::vector<Block>& blocks) {
    for (int i = 0; i < blocks.size(); i++) {
        if (blocks[i].empty) {
            while (blocks.back().empty) {
                blocks.pop_back();
            }
            blocks[i].empty = false;
            blocks[i].id = blocks.back().id;
            blocks.pop_back();
        }
    }
}

void reduceCellsByFile(std::vector<File>& files) {
    std::vector<File> fixedDiskEnd = std::vector<File>();
    std::vector<File> fixedDiskFront = std::vector<File>();

    while (!files.empty()) {
        if (!files.front().isEmpty()) {
            //std::cout << "Moving front cell to fixed front" << std::endl;
            fixedDiskFront.emplace_back(files.front());
            files.erase(files.begin());
        }

        if (files.empty()) {
            break;
        }

        bool immutableLastCell = true;
        for (int i = 0; i < files.size() - 1; i++) {
            if (files[i].isEmpty() && files[i].size >= files.back().size) {
                //std::cout << "Last file has a space to be inserted." << std::endl;
                immutableLastCell = false;
                break;
            }
        }

        if (immutableLastCell) {
            //std::cout << "Immutable last cell, moving it to fixed end" << std::endl;
            fixedDiskEnd.emplace(fixedDiskEnd.begin(), files.back());
            files.pop_back();
            if (files.empty()) {
                break;
            }
            if (files.back().isEmpty()) {
                fixedDiskEnd.emplace(fixedDiskEnd.begin(), files.back());
                files.pop_back();
            }
            continue;
        }

        for (int i = 0; i < files.size(); i++) {
            if (files[i].isEmpty() && files[i].size >= files.back().size) {
                //std::cout << "Moving back file to front" << std::endl;
                files.insert(files.begin() + i, files.back());
                files.back().id = -1;
                files[i + 1].size -= files[i].size;
                if (files[i + 1].size == 0) {
                    files.erase(files.begin() + i + 1);
                }
                while (files.back().isEmpty()) {
                    fixedDiskEnd.emplace(fixedDiskEnd.begin(), files.back());
                    files.pop_back();
                }
                break;
            }
        }

        if (!files.front().isEmpty()) {
            //std::cout << "Front cell is not empty, moving it" << std::endl;
            fixedDiskFront.emplace_back(files.front());
            files.erase(files.begin());
        }
    }

    for (File file: fixedDiskFront) {
        files.emplace_back(file);
    }
    for (File file: fixedDiskEnd) {
        files.emplace_back(file);
    }
}

int64_t getBlocksCheckSum(std::vector<Block>& blocks) {
    int64_t checksum = 0;
    for (int i = 0; i < blocks.size(); i++) {
        if (!blocks[i].empty) {
            checksum += blocks[i].id * i;
        }
    }
    return checksum;
}

int64_t getFileCheckSum(const std::vector<File>& files) {
    auto blocks = std::vector<Block>();
    for (File file: files) {
        for (int i = 0; i < file.size; i++) {
            blocks.emplace_back(file.isEmpty(), file.id);
        }
    }
    return getBlocksCheckSum(blocks);
}


int main() {

    std::vector<Block> blocks = std::vector<Block>();
    initializeDiskBlocks(blocks, "input.txt");
    reduceDiskByBlocks(blocks);
    std::cout << "Block checksum: " << getBlocksCheckSum(blocks) << std::endl;

    std::vector<File> diskFiles = std::vector<File>();
    initializeDiskFiles(diskFiles, "input.txt");
    reduceCellsByFile(diskFiles);
    std::cout << "File checksum: " << getFileCheckSum(diskFiles) << std::endl;
}
