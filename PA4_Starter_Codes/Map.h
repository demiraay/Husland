#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <queue>
#include <cmath>
#include "Isle.h"

struct MapNode
{
    Isle *isle;
    MapNode *left, *right;
    int height = 1;

    MapNode(Isle *isle) : isle(isle), left(nullptr), right(nullptr), height(1) {}
    ~MapNode()
    {
        // In MapNode destructor, we only delete the Isle
        // The Map class will handle deleting child nodes
        delete isle;
        // Don't delete left and right here to avoid cascading deletes
        // The Map class's destructor will handle the tree structure
    }
};

class Map
{
private:
    MapNode *root; // Root node of the tree

    // Height of a node
    int height(MapNode *node);

    // AVL Rotations
    MapNode *rotateRight(MapNode *current);
    MapNode *rotateLeft(MapNode *current);

    // Helper functions for recursive AVL insertion and deletion
    // USE THEM
    MapNode *insert(MapNode *node, Isle *isle);
    MapNode *remove(MapNode *node, Isle *isle);

    // Item drop helper functions
    void preOrderItemDrop(MapNode *current, int &count);
    void postOrderItemDrop(MapNode *current, int &count);

public:
    // Constructor declaration
    Map();
    // Destructor
    ~Map();

    // Calculation of minumum depth a player can access at the map
    int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth);

    // Tree operations
    void insert(Isle *isle);
    void remove(Isle *isle);
    /// Search
    Isle *findIsle(Isle isle);
    Isle *findIsle(std::string name);
    MapNode *findNode(Isle isle);
    MapNode *findNode(std::string name);

    // Initilize tree from a vector
    void initializeMap(std::vector<Isle *> isles);

    MapNode *findFirstEmptyIsle(MapNode *node);
    

    int getDepth(MapNode *node); // Determines the depth of a node within the tree.
    int getIsleDepth(Isle *isle); // Determines the depth of a isle within the tree.
    int getDepth(); // Total level of the tree

    // Display tree in terminal
    void display(MapNode *current, int depth, int state);
    void displayMap();

    // Item drop api
    void populateWithItems();
    void dropItemBFS();


    // Write the AVL tree to a file with levels
    void writeToFile(const std::string &filename);
    // Write current Isles to file in alphabetical order
    void writeIslesToFile(const std::string &filename);
};

#endif