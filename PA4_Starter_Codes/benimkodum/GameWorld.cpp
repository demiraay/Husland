#include "GameWorld.h"
#include "Map.h"
GameWorld::GameWorld() : mapTree(), shaperTree() {}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players)
{
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
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


    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess
    // Use // std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;



    bool hasAccess = false;

    if (!shaperTree.findPlayer(realmShaper->getName())) {
        std::cout << "[Access Control] RealmShaper not found!" << std::endl;
        return hasAccess;
    }

    int playerDepth = shaperTree.getDepth(realmShaper);
    int totalShaperTreeHeight = shaperTree.getDepth();
    int isleDepth = mapTree.getIsleDepth(isle);
    int totalMapDepth = mapTree.getDepth();
    int minAccessDepth = mapTree.calculateMinMapDepthAccess(playerDepth, totalShaperTreeHeight, totalMapDepth);


    hasAccess = (isleDepth >= minAccessDepth);


    return hasAccess;



}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle)
{
    // TODO:
    // Check if realmShaper has access
    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary

    // Use // std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    // Use // std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;
    // Use // std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" << std::endl;

    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation

    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle << std::endl;
        return;
    }


    isle = mapTree.findIsle(*isle);

    if (isle == nullptr) {
        return;
    }

    isle->increaseShaperCount();
    realmShaper->collectItem(isle->getItem());

    std::cout << "[Explore Area] " << realmShaper->getName()<< " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() << std::endl;


     if(realmShaper->currentIsle != "")
    {
        Isle* PastIsle = mapTree.findIsle(realmShaper->currentIsle);
         if (PastIsle) {
             PastIsle->decreaseShaperCount();
         }
    }


    if (isle->getShaperCount() <= 10) {

        realmShaper->currentIsle = isle->getName();
    }
    else {
        std::cout << "[Owercrowding] " << isle->getName() << "self-destructed, it will be removed from the map" << std::endl;
        for (int i = 0; i < shaperTree.getSize(); ++i)
        {
            if(shaperTree.findPlayerWithIndex(i)->currentIsle == isle->getName()) {
                shaperTree.findPlayerWithIndex(i)->currentIsle = "";
            }
        }
        MapNode* Node = mapTree.findNode(*isle);
        mapTree.remove(isle);
        delete Node;




    }


}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName){
    // TODO: Check energy and craft new isle if possible
    // Use std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    // Use std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;
    // Use std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;

    const std::string shaperName = shaper->getName();
    int shaperEnergyLevel = shaper->getEnergyLevel();
    if (!shaper->hasEnoughEnergy()) {

        std::cout << "[Energy] " << shaperName << " does not have enough energy points: " << shaperEnergyLevel << std::endl;
        return;
    }
    std::cout << "[Energy] " << shaperName << " has enough energy points: " << shaperEnergyLevel << std::endl;
    Isle *newIsle = new Isle(isleName);
    mapTree.insert(newIsle);
    shaper->loseEnergy();


    std::cout << "[Craft] " << shaperName << " crafted new Isle " << isleName << std::endl;



}

void GameWorld::displayGameState()
{


    // mapTree.displayMap();
    // shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs) {
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other

    // This function should call exploreArea and craft functions

    // Use displayGameState();

    std::ifstream accessFile(accessLogs);
    std::ifstream duelFile(duelLogs);

    // İlk satırı oku ve atla (access dosyası)
    {
        std::string dummyLine;
        std::getline(accessFile, dummyLine);
    }

    // İlk satırı oku ve atla (duel dosyası)
    {
        std::string dummyLine;
        std::getline(duelFile, dummyLine);
    }

    std::string accessLine, duelLine;
    int accessCount = 0;


    // Access kayıtlarını oku ve işle
    while (std::getline(accessFile, accessLine)) {
        std::istringstream accessStream(accessLine);
        std::string playerName, placeName;

        accessStream >> playerName >> placeName;
        RealmShaper *player = shaperTree.findPlayer(playerName);


        if (!player) {

            continue;
        }

        Isle *isle = mapTree.findIsle(placeName);

        if (isle) {

            exploreArea(player, isle);
        } else {

            craft(player, placeName);
        }


        accessCount++;




        if (accessCount % 5 == 0) {
            if (std::getline(duelFile, duelLine)) {
                std::istringstream duelStream(duelLine);
                std::string challengerName;
                bool result;
                duelStream >> challengerName >> result;

                RealmShaper *challenger = shaperTree.findPlayer(challengerName);
                if (challenger) {
                    shaperTree.duel(challenger, result);

                }

                displayGameState();

            }
        }
    }

    // Kalan duelloları işleme
    while (std::getline(duelFile, duelLine)) {
        std::istringstream duelStream(duelLine);
        std::string challengerName;
        bool result;

        duelStream >> challengerName >> result;

        RealmShaper *challenger = shaperTree.findPlayer(challengerName);
        if (challenger) {
            shaperTree.duel(challenger, result);
            displayGameState();

        }



    }

    accessFile.close();
    duelFile.close();

}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld, const std::string &currentShapers, const std::string &currentPlayerTree)
{
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}

