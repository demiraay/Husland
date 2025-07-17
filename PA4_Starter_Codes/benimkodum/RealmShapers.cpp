#include "RealmShapers.h"
#include <cmath>
#include <algorithm>
#include <queue>

ShaperTree::ShaperTree()
{
}

ShaperTree::~ShaperTree()
{
    // TODO: Free any dynamically allocated memory if necessary
    for (RealmShaper *shaper : realmShapers)
    {
        delete shaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers)
{
    // TODO: Insert initial shapers to the tree

    for (RealmShaper *shaper : shapers)
    {
        insert(shaper);
    }


}

int ShaperTree::getSize()
{
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree()
{
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index)
{
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if(index >= 0 && index < realmShapers.size()) {
        isValid = true;
    }

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper)
{
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper)
{
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    // else

    int index = findIndex(shaper);
    if (index == -1)
    {
        std::cerr << "Shaper not found in the tree!" << std::endl;
        return -1;
    }

    delete realmShapers[index];

    // Diziyi bir sola kaydır
    for (int i = index; i < realmShapers.size() - 1; i++)
    {
        realmShapers[i] = realmShapers[i + 1];
    }

    // Son elemanı kaldır
    realmShapers.pop_back();

    return index;




}

int ShaperTree::findIndex(RealmShaper *shaper)
{
    // return index in the tree if found
    // else

    for (int index = 0; index < realmShapers.size(); index++)
    {
        if (*realmShapers[index] == *shaper)
            return index;
    }
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper)
{
    // return depth of the node in the tree if found
    // else
    int index = findIndex(shaper);
    if (index == -1)
    {
        std::cerr << "Shaper not found in the tree!" << std::endl;
        return -1;
    }


    int depth = 0;
    while (index > 0)
    {
        index = (index - 1) / 2; // Parent düğümün indeksine git
        depth++;
    }

    return depth;
}

RealmShaper* ShaperTree::findPlayerWithIndex(int index)
{
    return realmShapers[index];
}


int ShaperTree::getDepth()
{
    // return total|max depth|height of the tree
    if (realmShapers.empty())
        return 0;


    int size = realmShapers.size();
    int depth = 0;


    while (size > 0)
    {
        size /= 2;
        depth++;

    }

    return depth - 1; // Derinlik 0'dan başladığı için son adımı çıkar.
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result)
{
    // TODO: Implement duel logic, return the victor
    // Use   std::cout << "[Duel] " << victorName << " won the duel" << std::endl;
    // Use   std::cout << "[Honour] " << "New honour points: ";
    // Use   std::cout << challengerName << "-" << challengerHonour << " ";
    // Use   std::cout << opponentName << "-" << opponentHonour << std::endl;
    // Use   std::cout << "[Duel] " << loserName << " lost all honour, delete" << std::endl;


    int index = findIndex(challenger);
    int parentIndex = (index - 1) / 2;
    RealmShaper *opponent = realmShapers[parentIndex];
    if (index == -1 )
    {

        return *challenger;
    }
    if(index == 0) {
        //root
        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;


        challenger->gainHonour();
        opponent->loseHonour();

        std::cout << "[Honour] New honour points: "
                  << challenger->getName() << "-" << challenger->getHonour() << " "
                  << opponent->getName() << "-" << opponent->getHonour() << std::endl;


        if (opponent->getHonour() <= 0)
        {
            std::cout << "[Duel] " << opponent->getName() << " lost all honour, delete" << std::endl;

            remove(opponent);
        }
        else
        {

            replace(opponent, challenger);
        }

        return *challenger;


    }




    if (result)
    {

        std::cout << "[Duel] " << challenger->getName() << " won the duel" << std::endl;


        challenger->gainHonour();
        opponent->loseHonour();

        std::cout << "[Honour] New honour points: "
                  << challenger->getName() << "-" << challenger->getHonour() << " "
                  << opponent->getName() << "-" << opponent->getHonour() << std::endl;


        if (opponent->getHonour() <= 0)
        {
            std::cout << "[Duel] " << opponent->getName() << " lost all honour, delete" << std::endl;

            remove(opponent);
        }
        else
        {

            replace(opponent, challenger);
        }

         return *challenger;
    }
    else
    {

        std::cout << "[Duel] " << challenger->getName() << " lost the duel" << std::endl;


        challenger->loseHonour();
        opponent->gainHonour();

        std::cout << "[Honour] New honour points: "
                  << challenger->getName() << "-" << challenger->getHonour() << " "
                  << opponent->getName() << "-" << opponent->getHonour() << std::endl;


        if (challenger->getHonour() <= 0)
        {
            std::cout << "[Duel] " << challenger->getName() << " lost all honour, delete" << std::endl;
            remove(challenger);

        }

        return *opponent;
    }


}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper)
{
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    int index = findIndex(shaper);
    int parentIndex = (index - 1) / 2;

    parent = realmShapers[parentIndex];

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high)
{
    // TODO: Change player_low and player_high's positions on the tree

    int low_index = findIndex(player_low);
    int high_index = findIndex(player_high);
    std::swap(realmShapers[low_index], realmShapers[high_index]);
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (RealmShaper *player : realmShapers)
    {
        if (*player == shaper) //
        {
            foundShaper = player;
            return player; // Bulunan nesneyi döndür
        }
    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name)
{
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found

    for (RealmShaper *player : realmShapers)
    {
        if (player->getName() == name)
        {
            foundShaper = player;
            break;
        }
    }

    return foundShaper;
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since ShaperTree is not a binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well


    inOrderTraversalHelper(index, result);
    return result;
}



void ShaperTree::inOrderTraversalHelper(int index, std::vector<std::string> &result)
{
    if (!isValidIndex(index))
        return;


    inOrderTraversalHelper(2 * index + 1, result);


    result.push_back(realmShapers[index]->getName());


    inOrderTraversalHelper(2 * index + 2, result);
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    preOrderTraversalHelper(index, result);
    return result;
}

void ShaperTree::preOrderTraversalHelper(int index, std::vector<std::string> &result)
{
    if (!isValidIndex(index))
        return;


    result.push_back(realmShapers[index]->getName());


    preOrderTraversalHelper(2 * index + 1, result);


    preOrderTraversalHelper(2 * index + 2, result);
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index)
{
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    postOrderTraversalHelper(index, result);
    return result;
}

void ShaperTree::postOrderTraversalHelper(int index, std::vector<std::string> &result)
{
    if (!isValidIndex(index))
        return;


    postOrderTraversalHelper(2 * index + 1, result);


    postOrderTraversalHelper(2 * index + 2, result);


    result.push_back(realmShapers[index]->getName());
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile)
{
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file

    // Define and implement as many helper functions as necessary for recursive implementation
    preOrderTraversalHelper(index, outFile);
}
void ShaperTree::preOrderTraversalHelper(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index))
        return;

    outFile << realmShapers[index]->getName() << std::endl;


    preOrderTraversalHelper(2 * index + 1, outFile);


    preOrderTraversalHelper(2 * index + 2, outFile);
}


void ShaperTree::breadthFirstTraversal(std::ofstream &outFile)
{
    // TODO: Implement level-order traversal
    // write nodes to output file

    // Define and implement as many helper functions as necessary
    if (realmShapers.empty())
        return;


    std::queue<int> nodeQueue;
    nodeQueue.push(0);

    while (!nodeQueue.empty())
    {

        int currentIndex = nodeQueue.front();
        nodeQueue.pop();


        outFile << realmShapers[currentIndex]->getName() << std::endl;


        int leftChildIndex = 2 * currentIndex + 1;
        if (isValidIndex(leftChildIndex))
            nodeQueue.push(leftChildIndex);


        int rightChildIndex = 2 * currentIndex + 2;
        if (isValidIndex(rightChildIndex))
            nodeQueue.push(rightChildIndex);
    }
}

void ShaperTree::displayTree()
{
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix)
{
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = 2*index +1 ;  // TODO: Calculate left index
    int right = 2*index +2 ; // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right))
    {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename)
{
    // TODO: Write the shapers to filename output level by level
    // Use std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;

    std::ofstream outFile(filename);

    if (realmShapers.empty()) {
        outFile.close();
        return;
    }

    std::queue<int> nodeQueue;
    nodeQueue.push(0);
    while (!nodeQueue.empty())
    {
        int currentIndex = nodeQueue.front();
        nodeQueue.pop();


        outFile << realmShapers[currentIndex]->getName() << std::endl;


        int leftChildIndex = 2 * currentIndex + 1;
        if (isValidIndex(leftChildIndex))
            nodeQueue.push(leftChildIndex);


        int rightChildIndex = 2 * currentIndex + 2;
        if (isValidIndex(rightChildIndex))
            nodeQueue.push(rightChildIndex);
    }


    outFile.close();



}

void ShaperTree::writeToFile(const std::string &filename)
{
    // TODO: Write the tree to filename output pre-order
    // Use std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

    std::ofstream outFile(filename);

    if (realmShapers.empty()) {
        outFile.close();
        return;
    }
    writeToFileHelper(0, outFile);


    outFile.close();



}
void ShaperTree::writeToFileHelper(int index, std::ofstream &outFile)
{
    if (!isValidIndex(index))
        return;


    outFile << realmShapers[index]->getName() << std::endl;


    writeToFileHelper(2 * index + 1, outFile);


    writeToFileHelper(2 * index + 2, outFile);
}