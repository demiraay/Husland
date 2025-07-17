#include "Isle.h"
#include <fstream>
#include <sstream>

Isle::Isle(std::string name) : name(name)
{
}

const std::string& Isle::getName() const
{
    return this->name;
}

Item Isle::getItem()
{
    return item;
}

void Isle::setItem(Item newItem)
{
    this->item = newItem;
}

int Isle::getShaperCount()
{
    return shaperCount;
}

bool Isle::increaseShaperCount()
{
    // Increase the number of shapers and check if capacity is exceeded
    shaperCount++;
    return shaperCount > capacity; // Returns true if Isle is overcrowded
}

bool Isle::decreaseShaperCount()
{
    if (shaperCount > 0)
    {
        shaperCount--;
    }
    return shaperCount <= 0; // Returns true if the Isle becomes empty
}

bool Isle::operator==(const Isle &other) const
{
    // Compare by name
    return this->name == other.name;
}

bool Isle::operator<(const Isle &other) const
{
    // Compare alphabetically
    return this->name < other.name;
}

bool Isle::operator>(const Isle &other) const
{
    // Compare alphabetically
    return this->name > other.name;
}


std::vector<Isle*> Isle::readFromFile(const std::string& filename) {
    std::vector<Isle*> isles;
    std::ifstream file(filename);

    // Check if file opened successfully
    if (!file.is_open()) {
        return isles;  // Return empty vector if file couldn't be opened
    }

    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (!line.empty() && line[0] != '#') {  // Skip empty lines and comments
            Isle* newIsle = new Isle(line);
            isles.push_back(newIsle);
        }
    }

    file.close();
    return isles;
}

std::ostream &operator<<(std::ostream &os, const Isle &p)
{
    // Existing color implementation from original file
    std::string einsteiniumColor = "\033[38;2;6;134;151m";
    std::string goldiumColor = "\033[38;2;255;198;5m";
    std::string amazoniteColor = "\033[38;2;169;254;255m";
    std::string resetColorTag = "\033[0m";

    if (p.item == EINSTEINIUM)
        return (os << einsteiniumColor << p.name << resetColorTag);
    if (p.item == GOLDIUM)
        return (os << goldiumColor << p.name << resetColorTag);
    if (p.item == AMAZONITE)
        return (os << amazoniteColor << p.name << resetColorTag);
    return (os << p.name);
}