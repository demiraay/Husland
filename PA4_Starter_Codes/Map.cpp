#include "Map.h"
#include "functional"
#include <algorithm>

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // Free dynamically allocated memory
    std::queue<MapNode*> nodes;
    if (root)
        nodes.push(root);
    while (!nodes.empty())
    {
        MapNode* current = nodes.front();
        nodes.pop();
        if (current->left)
            nodes.push(current->left);
        if (current->right)
            nodes.push(current->right);
        delete current->isle;
    }
}

// Initialize map with Isles and populate items
void Map::initializeMap(std::vector<Isle *> isles)
{
    for (auto isle : isles)
    {
        insert(isle); // Insert each Isle
    }
    populateWithItems(); // Populate with items
}

// Perform right rotation
MapNode *Map::rotateRight(MapNode *current)
{
    if (!current || !current->left)
    {
        // Invalid node for right rotation; return current unchanged
        return current;
    }

    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

// Perform left rotation
MapNode *Map::rotateLeft(MapNode *current)
{
    if (!current || !current->right)
    {
        // Invalid node for left rotation; return current unchanged
        return current;
    }

    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;

    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}

int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // Return height of the node
    if (node)
    {
        return node->height;
    }
    // Final return statement preserved
    return 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;
    // Recursively insert isle into the tree
    if (!node)
    {
        newNode = new MapNode(isle);
        return newNode; // New node is directly returned when created
    }

    if (*isle < *node->isle)
    {
        node->left = insert(node->left, isle);
    }
    else if (*isle > *node->isle)
    {
        node->right = insert(node->right, isle);
    }
    else
    {
        return node; // Duplicate Isles are not allowed
    }

    // Update the height of the current node
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // Balance the node
    int balance = height(node->left) - height(node->right);

    // Left heavy
    if (balance > 1 && *isle < *node->left->isle)
        return rotateRight(node);

    // Right heavy
    if (balance < -1 && *isle > *node->right->isle)
        return rotateLeft(node);

    // Left-Right case
    if (balance > 1 && *isle > *node->left->isle)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left case
    if (balance < -1 && *isle < *node->right->isle)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    // If no rotations are performed, assign updated node to newNode
    newNode = node;
    // Return the newly assigned newNode
    return newNode;
}


void Map::insert(Isle *isle)
{
    root = insert(root, isle);

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    if (!node)
    {
        // Base case: Tree is empty
        return nullptr;
    }

    if (*isle < *node->isle)
    {
        // Recurse on the left subtree
        node->left = remove(node->left, isle);
    }
    else if (*isle > *node->isle)
    {
        // Recurse on the right subtree
        node->right = remove(node->right, isle);
    }
    else
    {
        // Node to be deleted found
        if (!node->left || !node->right)
        {
            MapNode *temp = node->left ? node->left : node->right;
            delete node->isle; // Free Isle memory
            return temp;       // Return the non-null child or nullptr
        }
        else
        {
            // Node has two children, find in-order successor
            MapNode *successor = node->right;
            while (successor->left)
                successor = successor->left;

            // Replace current node's Isle with successor's Isle
            node->isle = successor->isle;

            // Remove the successor node
            node->right = remove(node->right, successor->isle);
        }
    }

    // Update the height of the current node
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // Balance the node
    int balance = height(node->left) - height(node->right);

    // Left heavy
    if (balance > 1 && height(node->left->left) >= height(node->left->right))
        return rotateRight(node);

    // Right heavy
    if (balance < -1 && height(node->right->right) >= height(node->right->left))
        return rotateLeft(node);

    // Left-Right case
    if (balance > 1 && height(node->left->right) > height(node->left->left))
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left case
    if (balance < -1 && height(node->right->left) > height(node->right->right))
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Final return statement preserved
    return node;
}

void Map::remove(Isle *isle)
{
    root = remove(root, isle);
}

// Pre-order traversal for item drop
void Map::preOrderItemDrop(MapNode *current, int &count)
{
    if (!current)
        return;

    if (++count % 5 == 0)
    {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }

    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// Post-order traversal for item drop
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    if (!current)
        return;

    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);

    if (++count % 3 == 0)
    {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

// Find first Isle with no item
MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    if (!node)
        return nullptr;

    if (node->isle->getItem() == EMPTY)
        return node;

    MapNode *leftSearch = findFirstEmptyIsle(node->left);
    if (leftSearch)
        return leftSearch;

    return findFirstEmptyIsle(node->right);
}

// BFS item drop for AMAZONITE
void Map::dropItemBFS()
{
    if (!root)
    {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }

    std::queue<MapNode *> q;
    q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        if (current->isle->getItem() == EMPTY)
        {
            current->isle->setItem(AMAZONITE);
            std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return;
        }

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }

    std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
}

void Map::displayMap()
{
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node)
{
    // Return the height of the given node or -1 if not found
    if (!node)
        return -1;

    return std::max(getDepth(node->left), getDepth(node->right)) + 1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    MapNode *current = root;
    int depth = 0;

    while (current)
    {
        if (*isle == *current->isle)
            return depth;

        if (*isle < *current->isle)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
        depth++;
    }

    // Isle not found
    return -1;
}

int Map::getDepth()
{
    // Return max depth of the tree starting from root
    return getDepth(root);
}

void Map::populateWithItems()
{
    int count = 0;

    // First populate GOLDIUM using post-order traversal
    postOrderItemDrop(root, count);

    // Reset count and populate EINSTEINIUM using pre-order traversal
    count = 0;
    preOrderItemDrop(root, count);
}

Isle *Map::findIsle(Isle isle)
{
    MapNode *current = root;

    while (current)
    {
        if (isle == *current->isle)
            return current->isle;

        if (isle < *current->isle)
            current = current->left;
        else
            current = current->right;
    }

    // Isle not found
    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    MapNode *current = root;

    while (current)
    {
        if (name == current->isle->getName())
            return current->isle;

        if (name < current->isle->getName())
            current = current->left;
        else
            current = current->right;
    }

    // Isle not found
    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    MapNode *current = root;

    while (current)
    {
        if (isle == *current->isle)
            return current;

        if (isle < *current->isle)
            current = current->left;
        else
            current = current->right;
    }

    // Node not found
    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    MapNode *current = root;

    while (current)
    {
        if (name == current->isle->getName())
            return current;

        if (name < current->isle->getName())
            current = current->left;
        else
            current = current->right;
    }

    // Node not found
    return nullptr;
}

void Map::display(MapNode *current, int depth, int state)
{
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
              << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;

    std::queue<MapNode *> q;
    if (root)
        q.push(root);

    while (!q.empty())
    {
        MapNode *current = q.front();
        q.pop();

        file << current->isle->getName() << " (Height: " << current->height << ")" << std::endl;

        if (current->left)
            q.push(current->left);
        if (current->right)
            q.push(current->right);
    }

    file.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
        return;

    std::vector<Isle *> isles;
    std::function<void(MapNode *)> inOrderTraversal = [&](MapNode *node) {
        if (!node)
            return;

        inOrderTraversal(node->left);
        isles.push_back(node->isle);
        inOrderTraversal(node->right);
    };

    inOrderTraversal(root);

    // Sort Isles alphabetically
    std::sort(isles.begin(), isles.end(), [](Isle *a, Isle *b) {
        return a->getName() < b->getName();
    });

    for (Isle *isle : isles)
    {
        file << isle->getName() << std::endl;
    }

    std::cout << "[Output] Isles have been written to " << filename << " in alphabetical order." << std::endl;
    file.close();
}