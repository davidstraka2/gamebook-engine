#include "Inventory.hpp"

#include "io.hpp"

using io::choose;
using io::tell;
using std::string;
using std::to_string;
using std::vector;

Inventory::Inventory():
    // Set default values
    m_capacity(0),
    m_equipped(-1) {}

const Inventory::ItemContainer& Inventory::items() const {
    return m_items;
}

Inventory::ItemContainer& Inventory::items() {
    return m_items;
}

int Inventory::capacity() const {
    return m_capacity;
}

void Inventory::capacity(int value) {
    // Capacity can't be negative
    if (value < 0)
        value = 0;
    // Set value
    m_capacity = value;
}

int Inventory::equipped() const {
    return m_equipped;
}

void Inventory::equipped(int value) {
    // Check index range
    if (value >= (int) m_items.size())
        throw InvalidRangeErr();
    // Lowest possible value is -1
    if (value < -1)
        value = -1;
    // Set value
    m_equipped = value;
}

bool Inventory::add(const Item& item) {
    if (hasSpace()) {
        m_items.emplace_back(item);
        return true;
    }
    return false;
}

void Inventory::equip() {
    // List all items in options, add "None" option
    vector<string> options;
    listItems(options);
    options.emplace_back("None");
    // Have player choose which item to equip, if any
    tell("Choose item to equip");
    auto choice = choose(options);
    // If player didn't pick "None", equip chosen item
    if (choice < (int) m_items.size()) {
        m_equipped = choice;
    } else {
        m_equipped = -1;
    }
}

void Inventory::forceAdd(const Item& item) {
    // Increase capacity if needed
    if (!hasSpace())
        m_capacity++;
    // Add item
    m_items.emplace_back(item);
}

bool Inventory::freeSpace() {
    // List all items in options, add "None" option
    vector<string> options;
    listItems(options);
    options.emplace_back("None");
    // Have player choose which item to throw away, if any
    tell("Choose item to throw away to free up inventory space");
    auto choice = choose(options);
    // If player didn't pick "None", throw away chosen item
    if (choice < (int) m_items.size()) {
        throwAway(choice);
        return true;
    }
    return false;
}

const Item& Inventory::getEquippedItem() const {
    // Return first item if no item equipped
    if (m_equipped < 0)
        return m_items[0];
    // Return equipped item
    return m_items[m_equipped];
}

const Item& Inventory::getItem(int index) const {
    // Check index range
    if (index >= (int) m_items.size())
        throw InvalidRangeErr();
    return m_items[index];
}

bool Inventory::hasSpace() const {
    if ((int) m_items.size() < m_capacity)
        return true;
    return false;
}

void Inventory::listItems(vector<string>& list) const {
    for (size_t i = 0; i < m_items.size(); i++) {
        const auto& item = m_items[i];
        // Item index and name
        list.emplace_back(to_string(i) + ") " + item.name());
        // Attributes
        list.back() += " [strength: " + to_string(item.strength()) + ", ";
        list.back() += "dexterity: " + to_string(item.dexterity()) + ", ";
        list.back() += "vitality: " + to_string(item.vitality()) + "]";
        // Is equipped?
        if ((int) i == m_equipped)
            list.back() += " (equipped)";
    }
}

void Inventory::throwAway(int index) {
    // Check index range
    if (index >= (int) m_items.size())
        throw InvalidRangeErr();
    // Erase item from m_items
    m_items.erase(m_items.begin() + index);
    // Fix equipped item index
    if (m_equipped > index) {
        m_equipped--;
    } else if (m_equipped == index) {
        m_equipped = -1;
    }
}
