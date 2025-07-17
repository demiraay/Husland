// ShaperTree.cpp
#include "RealmShapers.h"
#include <cmath>
#include <queue>
#include <fstream>
#include <algorithm>
#include <functional>

ShaperTree::ShaperTree() {}

ShaperTree::~ShaperTree() {
    // Delete all RealmShaper objects
    for (RealmShaper* shaper : realmShapers) {
        delete shaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers) {
    // Clear existing shapers without deleting them
    realmShapers.clear();

    // Simply store the pointers
    realmShapers = shapers;
}

int ShaperTree::getSize() {
    return realmShapers.size();
}


RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result) {
    if (!challenger) {
        return RealmShaper("", 0);
    }

    // Find opponent (parent)
    RealmShaper* opponent = getParent(challenger);
    if (!opponent) {
        return *challenger;  // Challenger is root, no opponent
    }

    RealmShaper* victor = result ? challenger : opponent;
    RealmShaper* loser = result ? opponent : challenger;

    // Update honour points
    victor->gainHonour();
    loser->loseHonour();

    std::cout << "[Duel] " << victor->getName() << " won the duel" << std::endl;
    std::cout << "[Honour] " << "New honour points: ";
    std::cout << challenger->getName() << "-" << challenger->getHonour() << " ";
    std::cout << opponent->getName() << "-" << opponent->getHonour() << std::endl;

    // Handle loser with 0 honour
    if (loser->getHonour() <= 0) {
        std::cout << "[Duel] " << loser->getName() << " lost all honour, delete" << std::endl;
        remove(loser);
    } else if (result) {
        // Swap positions if challenger won
        replace(challenger, opponent);
    }

    return *victor;
}

// Helper function for traversals
void addToVector(std::vector<std::string>& result, RealmShaper* shaper) {
    if (shaper) result.push_back(shaper->getName());
}


void ShaperTree::preOrderTraversal(int index, std::ofstream& outFile) {
    if (!isValidIndex(index)) return;

    if (realmShapers[index]) {
        outFile << realmShapers[index]->getName() << std::endl;
    }

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    preOrderTraversal(left, outFile);
    preOrderTraversal(right, outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream& outFile) {
    for (size_t i = 0; i < realmShapers.size(); i++) {
        if (realmShapers[i]) {
            outFile << realmShapers[i]->getName() << std::endl;
        }
    }
}

void ShaperTree::printTree(int index, int level, const std::string& prefix) {
    if (!isValidIndex(index)) return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;

    // Calculate left and right child indices
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (isValidIndex(left) || isValidIndex(right)) {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : ""));
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}


std::vector<RealmShaper *> ShaperTree::getTree() {
    return realmShapers;
}

// In RealmShapers.cpp, implement displayTree
void ShaperTree::displayTree() {
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}



void ShaperTree::insert(RealmShaper *shaper) {
    realmShapers.push_back(shaper);
}

bool ShaperTree::isValidIndex(int index) {
    return index >= 0 && index < static_cast<int>(realmShapers.size());
}

int ShaperTree::findIndex(RealmShaper *shaper) {
    for (size_t i = 0; i < realmShapers.size(); i++) {
        if (realmShapers[i] == shaper) {
            return i;
        }
    }
    return -1;
}

RealmShaper* ShaperTree::findPlayer(std::string name) {
    for (RealmShaper* shaper : realmShapers) {
        if (shaper->getName() == name) {
            return shaper;
        }
    }
    return nullptr;
}

RealmShaper* ShaperTree::findPlayer(RealmShaper shaper) {
    for (RealmShaper* s : realmShapers) {
        if (*s == shaper) {
            return s;
        }
    }
    return nullptr;
}

// Traversal functions
std::vector<std::string> ShaperTree::inOrderTraversal(int index) {
    std::vector<std::string> result;
    if (!isValidIndex(index)) return result;

    // Left subtree
    int leftChildIndex = 2 * index + 1;
    if (isValidIndex(leftChildIndex)) {
        auto leftResult = inOrderTraversal(leftChildIndex);
        result.insert(result.end(), leftResult.begin(), leftResult.end());
    }

    // Current node
    result.push_back(realmShapers[index]->getName());

    // Right subtree
    int rightChildIndex = 2 * index + 2;
    if (isValidIndex(rightChildIndex)) {
        auto rightResult = inOrderTraversal(rightChildIndex);
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }

    return result;
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index) {
    std::vector<std::string> result;
    if (!isValidIndex(index)) return result;

    // Current node
    result.push_back(realmShapers[index]->getName());

    // Left subtree
    int leftChildIndex = 2 * index + 1;
    if (isValidIndex(leftChildIndex)) {
        auto leftResult = preOrderTraversal(leftChildIndex);
        result.insert(result.end(), leftResult.begin(), leftResult.end());
    }

    // Right subtree
    int rightChildIndex = 2 * index + 2;
    if (isValidIndex(rightChildIndex)) {
        auto rightResult = preOrderTraversal(rightChildIndex);
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }

    return result;
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index) {
    std::vector<std::string> result;
    if (!isValidIndex(index)) return result;

    // Left subtree
    int leftChildIndex = 2 * index + 1;
    if (isValidIndex(leftChildIndex)) {
        auto leftResult = postOrderTraversal(leftChildIndex);
        result.insert(result.end(), leftResult.begin(), leftResult.end());
    }

    // Right subtree
    int rightChildIndex = 2 * index + 2;
    if (isValidIndex(rightChildIndex)) {
        auto rightResult = postOrderTraversal(rightChildIndex);
        result.insert(result.end(), rightResult.begin(), rightResult.end());
    }

    // Current node
    result.push_back(realmShapers[index]->getName());

    return result;
}

int ShaperTree::remove(RealmShaper *shaper) {
    int index = findIndex(shaper);
    if (index == -1) return -1;

    // Move last element to this position
    if (index != static_cast<int>(realmShapers.size() - 1)) {
        realmShapers[index] = realmShapers.back();
    }
    realmShapers.pop_back();

    // Don't delete shaper - caller owns it
    return index;
}

int ShaperTree::getDepth(RealmShaper *shaper) {
    int index = findIndex(shaper);
    if (index == -1) return -1;
    return static_cast<int>(std::floor(std::log2(index + 1)));
}

int ShaperTree::getDepth() {
    if (realmShapers.empty()) return -1;
    return static_cast<int>(std::floor(std::log2(realmShapers.size())));
}

RealmShaper* ShaperTree::getParent(RealmShaper *shaper) {
    int index = findIndex(shaper);
    if (index <= 0) return nullptr;
    return realmShapers[(index - 1) / 2];
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high) {
    int lowIndex = findIndex(player_low);
    int highIndex = findIndex(player_high);
    if (lowIndex != -1 && highIndex != -1) {
        std::swap(realmShapers[lowIndex], realmShapers[highIndex]);
    }
}

void ShaperTree::writeToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) return;

    // Pre-order traversal of the complete binary tree
    std::function<void(int)> preOrder = [&](int index) {
        if (!isValidIndex(index)) return;

        // Write current node
        outFile << realmShapers[index]->getName() << std::endl;

        // Traverse left and right children
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (isValidIndex(leftChild)) {
            preOrder(leftChild);
        }
        if (isValidIndex(rightChild)) {
            preOrder(rightChild);
        }
    };

    preOrder(0);  // Start from root
    outFile.close();
}

void ShaperTree::writeShapersToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) return;

    // Write each shaper's name on a new line according to current rankings
    for (RealmShaper* shaper : realmShapers) {
        outFile << shaper->getName() << std::endl;
    }

    outFile.close();
}