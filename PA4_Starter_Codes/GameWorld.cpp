#include "GameWorld.h"

#include <utility>

GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(std::move(players));
    mapTree.initializeMap(std::move(places));
}

Map& GameWorld::getMapTree()
{
    return mapTree;
}
ShaperTree& GameWorld::getShaperTree()
{
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle)
{
    bool hasAccess = false;

    // Find depth values
    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalShaperTreeHeight = shaperTree.getDepth();
    int totalMapDepth = mapTree.getDepth();

    if (playerDepth == -1)
    {
        // RealmShaper not found in the ShaperTree
        std::cout << "[Access Control] RealmShaper not found!" << std::endl;
        return false;
    }

    // Calculate the minimum accessible depth for the player
    int minAccessDepth = static_cast<int>(totalMapDepth * (static_cast<double>(playerDepth) / totalShaperTreeHeight));

    // Get the depth of the Isle in the map
    int isleDepth = mapTree.getIsleDepth(isle);

    if (isleDepth != -1 && isleDepth <= minAccessDepth)
    {
        hasAccess = true;
    }

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    if (!hasAccess(realmShaper, isle))
    {
        // Player does not have access to the Isle
        std::cout << "[Explore Area] " << realmShaper->getName()
                  << " does not have access to explore area "
                  << *isle << std::endl;
        return;
    }

    // Visit the Isle and interact with it
    std::cout << "[Explore Area] " << realmShaper->getName()
              << " visited " << isle->getName() << std::endl;

    // Collect item from the Isle
    realmShaper->collectItem(isle->getItem());


    // Display updated energy level
    std::cout << "[Energy] " << realmShaper->getName()
              << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;


    // Check for overcrowding
    if (isle->increaseShaperCount())
    {
        // Isle is overcrowded and self-destructs
        std::cout << "[Overcrowding] " << isle->getName()
                  << " self-destructed, it will be removed from the map"
                  << std::endl;

        mapTree.remove(isle); // Remove the Isle from the map
        // delete isle;          // Free Isle memory
    }
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName)
{
    // Check if the RealmShaper has enough energy to craft an Isle
    if (shaper->hasEnoughEnergy())
    {
        std::cout << "[Energy] " << shaper->getName()
                  << " has enough energy points: " << shaper->getEnergyLevel() << std::endl;

        // Deduct energy points from the shaper
        shaper->loseEnergy();

        // Create a new Isle and insert it into the map
        Isle *newIsle = new Isle(isleName);
        mapTree.insert(newIsle);

        std::cout << "[Craft] " << shaper->getName()
                  << " crafted new Isle " << isleName << std::endl;
    }
    else
    {
        std::cout << "[Energy] " << shaper->getName()
                  << " does not have enough energy points: "
                  << shaper->getEnergyLevel() << std::endl;
    }
}


void GameWorld::displayGameState()
{
    // mapTree.displayMap();
    // shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs
// Functions to read and parse Access and Duel logs


void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs)
{
    std::ifstream accessFile(accessLogs), duelFile(duelLogs);
    if (!accessFile.is_open() || !duelFile.is_open())
        return;

    std::queue<std::pair<std::string, std::string>> accessQueue;
    std::queue<std::tuple<std::string, std::string, bool>> duelQueue;

    std::string line;

    // Parse access logs
    while (std::getline(accessFile, line))
    {
        std::istringstream iss(line);
        std::string shaperName, isleName;
        if (iss >> shaperName >> isleName)
        {
            accessQueue.push(std::pair<std::string, std::string>(shaperName, isleName));
        }
    }

    // Parse duel logs
    while (std::getline(duelFile, line))
    {
        std::istringstream iss(line);
        std::string challengerName, opponentName, resultStr;
        if (iss >> challengerName >> opponentName >> resultStr)
        {
            bool result = (resultStr == "1");
            duelQueue.push(std::tuple<std::string, std::string, bool>(challengerName, opponentName, result));
        }
    }

    accessFile.close();
    duelFile.close();

    int accessCount = 0;

    // Process logs
    while (!accessQueue.empty())
    {
        std::pair<std::string, std::string> access = accessQueue.front();
        accessQueue.pop();

        std::string shaperName = access.first;
        std::string isleName = access.second;

        RealmShaper *shaper = shaperTree.findPlayer(shaperName);
        Isle *isle = mapTree.findIsle(isleName);

        if (shaper && isle)
        {
            exploreArea(shaper, isle);
        }

        accessCount++;

        // Trigger a duel every 5 accesses
        if (accessCount % 5 == 0 && !duelQueue.empty())
        {
            std::tuple<std::string, std::string, bool> duel = duelQueue.front();
            duelQueue.pop();

            std::string challengerName = std::get<0>(duel);
            std::string opponentName = std::get<1>(duel);
            bool result = std::get<2>(duel);

            RealmShaper *challenger = shaperTree.findPlayer(challengerName);
            RealmShaper *opponent = shaperTree.findPlayer(opponentName);

            if (challenger && opponent)
            {
                shaperTree.duel(challenger, result);
            }
        }
    }

    // Process remaining duels
    while (!duelQueue.empty())
    {
        std::tuple<std::string, std::string, bool> duel = duelQueue.front();
        duelQueue.pop();

        std::string challengerName = std::get<0>(duel);
        std::string opponentName = std::get<1>(duel);
        bool result = std::get<2>(duel);

        RealmShaper *challenger = shaperTree.findPlayer(challengerName);
        RealmShaper *opponent = shaperTree.findPlayer(opponentName);

        if (challenger && opponent)
        {
            shaperTree.duel(challenger, result);
        }
    }

    // Display game state after processing events
    displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}
