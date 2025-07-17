#include "Map.h"

Map::Map()
{
    this->root = nullptr;
}
Map::~Map()
{
    // TODO: Free any dynamically allocated memory if necessary
    delete root;
    root = nullptr;



}



void Map::initializeMap(std::vector<Isle *> isles)
{
    // TODO: Insert initial isles to the tree
    // Then populate with Goldium and Einstainium items

    for (auto *isle : isles)
    {
        insert(isle);
    }





    populateWithItems();
    inserted = true;
    rebalanced = false;
    countRebalances = 0;



}

MapNode *Map::rotateRight(MapNode *current)
{
    // TODO: Perform right rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
    if (current == nullptr || current->left == nullptr)
    {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }


    MapNode *newRoot = current->left;
    current->left = newRoot->right;
    newRoot->right = current;


    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;


}

MapNode *Map::rotateLeft(MapNode *current)
{
    // TODO: Perform left rotation according to AVL
    // return necessary new root
    // Use std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;


    if (current == nullptr || current->right == nullptr)
    {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return current;
    }


    MapNode *newRoot = current->right;
    current->right = newRoot->left;
    newRoot->left = current;


    current->height = std::max(height(current->left), height(current->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;

    return newRoot;
}



int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth)
{
    return (int)totalMapDepth * ((double)playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node)
{
    // TODO: Return height of the node
    if (node == nullptr)
        return 0;


    int hl;
    int hr;

    hl = (node && node->left) ? node->left->height : 0;
    hr = (node && node->right) ? node->right->height : 0;

    return hl > hr ? hl + 1 : hr + 1;
}
MapNode *Map::insert(MapNode *node, Isle *isle)
{
    MapNode *newNode = nullptr;

    // TODO: Recursively insert isle to the tree
    // returns inserted node

    if (node == nullptr)
        return new MapNode(isle);

    if (*isle < *(node->isle))
        node->left = insert(node->left, isle);
    else if (*isle > *(node->isle))
        node->right = insert(node->right, isle);
    else
        return node;

    // Yükseklik güncellemesi
    node->height = std::max(height(node->left), height(node->right)) + 1;

    // Denge faktörü hesaplama
    node = balance(node,isle);

    return node;
}

void Map::insert(Isle *isle)
{
    root = insert(root, isle);
    if (rebalanced && ++countRebalances % 3 == 0 && inserted == true)
    {
        populateWithItems();
        dropItemBFS();
        rebalanced = false;
        countRebalances = 0 ;
    }

    // you might need to insert some checks / functions here depending on your implementation
}

MapNode *Map::remove(MapNode *node, Isle *isle)
{
    if (node == nullptr)
    {
        std::cerr << "[Remove] Tree is Empty or Isle not found: " << isle->getName() << std::endl;
        return nullptr;
    }

    if (*isle < *(node->isle))
    {
        node->left = remove(node->left, isle);


    }
    else if (*isle > *(node->isle))
    {
        node->right = remove(node->right, isle);
       
    }
    else
    {
        // Silinecek node bulundu
        if (node->left == nullptr && node->right == nullptr)
        {
            // Yaprak düğüm

            return nullptr;
        }
        else if (node->left == nullptr)
        {
            // Tek çocuk (sağ çocuk var)
            MapNode *temp = node->right;
            node->right = nullptr;

            return temp;
        }
        else if (node->right == nullptr)
        {
            // Tek çocuk (sol çocuk var)
            MapNode *temp = node->left;
            node->left = nullptr;

            return temp;
        }
        else
        {
            // İki çocuklu düğüm
            MapNode *inPre = InPre(node->left);
            // inPre, node->left alt ağacının en sağ düğümü

            // Isle pointerlarını swap yapıyoruz
            Isle *tempIsle = node->isle;
            node->isle = inPre->isle;
            inPre->isle = tempIsle;

            // Şimdi inPre->isle aslında tempIsle (eski node->isle)
            // Bu isle değeri üzerinden inPre düğümünü siliyoruz
            node->left = remove(node->left, tempIsle);
        }
    }

    if (node == nullptr) return node;

    // Yükseklik güncelle
    node->height = std::max(height(node->left), height(node->right)) + 1;

    node = balance(node,isle);
    return node;
}

void Map::remove(Isle *isle)
{
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
    if (rebalanced && ++countRebalances % 3 == 0 && inserted == true)
    {
        populateWithItems();
        dropItemBFS();
        rebalanced = false;
        countRebalances = 0 ;
    }
}

void Map::preOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop EINSTEINIUM according to rules
    // Use std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;


    if (current == nullptr)
        return;


    count++;
    if (count % 5 == 0)
    {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }


    preOrderItemDrop(current->left, count);


    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count)
{
    // TODO: Drop GOLDIUM according to rules
    // Use  std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;

    if (current == nullptr)
        return;


    postOrderItemDrop(current->left, count);


    postOrderItemDrop(current->right, count);


    count++;
    if (count % 3 == 0)
    {
        current->isle->setItem(GOLDIUM);

        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
}

MapNode *Map::findFirstEmptyIsle(MapNode *node)
{
    // TODO: Find first Isle with no item

    if (node == nullptr)
        return nullptr;


    std::queue<MapNode *> queue;
    queue.push(node);

    while (!queue.empty())
    {

        MapNode *current = queue.front();
        queue.pop();


        if (current->isle->getItem() == EMPTY)
            return current;


        if (current->left)
            queue.push(current->left);


        if (current->right)
            queue.push(current->right);
    }


    return nullptr;
}

void Map::dropItemBFS()
{
    // TODO: Drop AMAZONITE according to rules
    // Use std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << targetNode->isle->getName() << std::endl;
    // Use std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl;

    if (root == nullptr) {
        std::cout << "[BFS Drop] No eligible Isle found for AMAZONITE drop." << std::endl;
        return;
    }
    std::queue<MapNode *> queue;
    queue.push(root);

    while (!queue.empty())
    {

        MapNode *current = queue.front();
        queue.pop();


        if (current->isle->getItem() == EMPTY)
        {
            current->isle->setItem(AMAZONITE);
            std::cout << "[BFS Drop] AMAZONITE dropped on Isle: " << current->isle->getName() << std::endl;
            return;
        }


        if (current->left)
            queue.push(current->left);


        if (current->right)
            queue.push(current->right);
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
    // TODO: Return node depth if found, else

    if (root == nullptr || node == nullptr)
        return -1;

    int depth = 0;
    MapNode *current = root;


    while (current != nullptr)
    {
        if (node->isle == current->isle)
            return depth;

        if (*node->isle < *(current->isle))
            current = current->left;
        else
            current = current->right;

        depth++;
    }

    return -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle)
{
    // TODO: Return node depth by isle if found, else
    if (root == nullptr || isle == nullptr)
        return -1;

    int depth = 0;
    MapNode *current = root;


    while (current != nullptr)
    {
        if (*(current->isle) == *isle)
            return depth;

        if (*isle < *(current->isle))
            current = current->left;
        else
            current = current->right;

        depth++;
    }

    return -1;
}
int Map::countNodes(MapNode *node)
{
    if (node == nullptr)
        return 0;

    return 1 + countNodes(node->left) + countNodes(node->right);
}

int Map::getDepth()
{
    // TODO: Return max|total depth of tree
    if (root == nullptr)
        return 0; // Eğer ağaç boşsa, derinlik 0.

    // Toplam düğüm sayısını hesaplayın
    int totalNodes = countNodes(root);

    // Derinliği logaritmik formülle hesapla
    return static_cast<int>(std::floor(std::log2(totalNodes)));
}


void Map::populateWithItems()
{
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM

    int goldiumCounter = 0;
    postOrderItemDrop(root, goldiumCounter);


    int einsteiniumCounter = 0;
    preOrderItemDrop(root, einsteiniumCounter);
}

Isle *Map::findIsle(Isle isle)
{
    // TODO: Find isle by value
    MapNode *current = root;


    while (current != nullptr)
    {
        if (*(current->isle) == isle)
            return current->isle;

        if (isle < *(current->isle))
            current = current->left;
        else
            current = current->right;
    }

    return nullptr;
}

Isle *Map::findIsle(std::string name)
{
    // TODO: Find isle by name
    MapNode *current = root;


    while (current != nullptr)
    {
        if (current->isle->getName() == name)
            return current->isle;

        if (name < current->isle->getName())
            current = current->left;
        else
            current = current->right;
    }

    return nullptr;
}

MapNode *Map::findNode(Isle isle)
{
    // TODO: Find node by value
    MapNode *current = root;


    while (current != nullptr)
    {
        if (*(current->isle) == isle)
            return current;

        if (isle < *(current->isle))
            current = current->left;
        else
            current = current->right;
    }

    return nullptr;
}

MapNode *Map::findNode(std::string name)
{
    // TODO: Find node by name
    MapNode *current = root;


    while (current != nullptr)
    {
        if (current->isle->getName() == name)
            return current;

        if (name < current->isle->getName())
            current = current->left;
        else
            current = current->right;
    }

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
    if (root == nullptr)
    {
        return;
    }

    std::ofstream outFile(filename);
    std::queue<MapNode *> queue;
    queue.push(root);

    bool hasNonNullInLastLevel = true; // Ağaçta yazılacak başka bir seviye kalıp kalmadığını kontrol için

    while (!queue.empty() && hasNonNullInLastLevel)
    {
        int levelSize = queue.size();
        hasNonNullInLastLevel = false; // Yeni seviyeyi kontrol etmeden önce sıfırla

        for (int i = 0; i < levelSize; ++i)
        {
            MapNode *current = queue.front();
            queue.pop();

            if (current == nullptr)
            {
                outFile << "NULL ";
                // NULL bile olsa, alt dallar için yer ayırarak tam ikili ağaç yapısını koru
                queue.push(nullptr);
                queue.push(nullptr);
            }
            else
            {
                outFile << current->isle->getName() << " ";
                hasNonNullInLastLevel = true; // NULL olmayan düğüm var
                queue.push(current->left);
                queue.push(current->right);
            }
        }

        outFile << std::endl; // Seviyeyi tamamladıktan sonra satır atla
    }

    outFile.close();
}

void Map::writeIslesToFile(const std::string &filename)
{
    // TODO: Write Isles to output file in alphabetical order
    // Use std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
    if (root == nullptr)
    {
        return;
    }

    std::vector<std::string> isleNames;
    collectIsleNamesInOrder(root, isleNames);


    std::ofstream outFile(filename);
    for (const std::string &name : isleNames)
    {
        outFile << name << std::endl;
    }

    outFile.close();




}

void Map::collectIsleNamesInOrder(MapNode *node, std::vector<std::string> &isleNames)
{
    if (node == nullptr)
        return;


    collectIsleNamesInOrder(node->left, isleNames);


    isleNames.push_back(node->isle->getName());


    collectIsleNamesInOrder(node->right, isleNames);
}
MapNode* Map::InPre(MapNode *node) {
    while (node->right != nullptr) {
        node = node->right;
    }
    return node;
}


int Map::BalanceFactor(MapNode *node) {
    if (node == nullptr) return 0;

    int hl = (node->left != nullptr) ? node->left->height : 0;
    int hr = (node->right != nullptr) ? node->right->height : 0;

    return hl - hr;
}
MapNode *Map::balance(MapNode *node, Isle *isle) {
    if (node == nullptr) return nullptr;

    // Denge faktörü hesaplama
    int balance = height(node->left) - height(node->right);

    if(countRebalances % 3 == 0 && countRebalances > 0 && inserted == true ) {
        populateWithItems();
        dropItemBFS();
        countRebalances = 0;

    }
    // Dört olası AVL dengeleme durumu:

    // 1. Left-Left (LL)
    if (balance > 1 && *isle < *(node->left->isle)) {
        countRebalances++ ;
        return rotateRight(node);
    }

    // 2. Right-Right (RR)
    if (balance < -1 && *isle > *(node->right->isle)) {
        countRebalances++ ;
        return rotateLeft(node);
    }

    // 3. Left-Right (LR)
    if (balance > 1 && *isle > *(node->left->isle)) {
        countRebalances++ ;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // 4. Right-Left (RL)
    if (balance < -1 && *isle < *(node->right->isle)) {
        countRebalances++ ;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;

}