#include "World.hpp"

#include "io.hpp"
#include "util.hpp"

#include <fstream>
#include <set>

using io::tell;
using std::string;
using std::to_string;
using std::vector;
using util::chainsMatch;
using util::stringToInt;
using util::writeLine;

const char labelEnd = ':';

const string l_bookName = "Name";
const string l_skillPts = "Skill points";
const string l_player = "Player";
const string l_playerLocation = "Location";
const string l_inventorySize = "Inventory size";
const string l_itemEquip = "Item equipped";
const string l_health = "Health";
const string l_dex = "Dexterity";
const string l_str = "Strength";
const string l_vit = "Vitality";
const string l_inventory = "Inventory";
const string l_locations = "Locations";
const string l_txt = "Text";
const string l_creatures = "Creatures";
const string l_loot = "Loot";
const string l_options = "Options";
const string l_option = "-";
const string l_action = "Action";
const string l_target = "Target";
const string l_winAction = "Win action";
const string l_winTarget = "Win target";

/** Get label and value from line */
void getLabelAndValue(const string& line, string& label, string& value) {
    size_t i;
    // Get label
    for (i = 0; i < line.length(); i++) {
        auto ch = line[i];
        if (ch == labelEnd)
            break;
    }
    label = line.substr(0, i);
    i++;
    // Skip any leading spaces
    for (; i < line.length(); i++) {
        auto ch = line[i];
        if (ch != ' ')
            break;
    }
    // Get value
    if (i < line.length())
        value = line.substr(i);
}

/**
 * Have player assign some of skill points to a skill. Return -1 when input is
 * invalid. Return 0 when no input given.
 */
int promptSkill(const string& skillName, int skillPts) {
    // Get player input
    string input;
    io::prompt("[Skill points remaining: " + to_string(skillPts) + "]\n\n" +
         skillName + ": ", input);
    // Get and validate number from input
    int number = stringToInt(input);
    if (number > skillPts)
        number = -1;
    return number;
}

string lastCreatureName,
    lastLocationName;
int itemEquipped = -1;

World::World(const string& filename):
    // Set default values
    m_skillPoints(0) {
    // Open file, throw error on fail
    std::ifstream file(filename);
    if (!file)
        throw FileErr();

    // Parse book line by line
    string line;
    vector<string> chain;
    while (getline(file, line))
        readLine(line, chain);

    // Close file
    file.close();

    // Locations can't be empty
    if (m_locations.size() == 0)
        throw NoLocationsErr();

    // Player must have a valid location assigned
    if (
        m_player.location().length() == 0 ||
        m_locations.count(m_player.location()) == 0
    ) {
        throw PlayerLocationErr();
    }

    // Locations must have unique text
    for (const auto& location: m_locations) {
        std::set<string> options;
        for (const auto& option: location.second.options()) {
            if (options.count(option.text()) != 0)
                throw OptionTextErr();
            options.emplace(option.text());
        }
    }

    // Set health of all creatures where health has no been set yet
    for (auto& location: m_locations) {
        for (auto& creature: location.second.creatures()) {
            Creature& c = creature.second;
            if (c.health() < 0)
                c.restoreHealth();
        }
    }

    // Set player's equipped item
    m_player.inventory().equipped(itemEquipped);
}

void World::readLine(string& line, vector<string>& chain) {
    // Trim indentation from line and get indentation level
    auto level = util::trimIndentation(line);
    // Skip empty (except for indentation) lines
    if (line == "")
        return;
    // Get label and value
    string label, value;
    getLabelAndValue(line, label, value);
    // Add label to chain
    util::manageChain(chain, level, label);

    // Get data

    // Book name
    if (chainsMatch(chain, {l_bookName})) {
        m_name = value;

    // Skill points
    } else if (chainsMatch(chain, {l_skillPts})) {
        m_skillPoints = stringToInt(value);

    // Player
    } else if (chainsMatch(chain, {l_player})) {
        // Do nothing

    // Player: Location
    } else if (chainsMatch(chain, {l_player, l_playerLocation})) {
        m_player.location(value);

    // Player: Inventory size
    } else if (chainsMatch(chain, {l_player, l_inventorySize})) {
        m_player.inventory().capacity(stringToInt(value));

    // Player: Item equipped
    } else if (chainsMatch(chain, {l_player, l_itemEquip})) {
        itemEquipped = stringToInt(value);

    // Player: Health
    } else if (chainsMatch(chain, {l_player, l_health})) {
        m_player.health(stringToInt(value));

    // Player: Dexterity
    } else if (chainsMatch(chain, {l_player, l_dex})) {
        m_player.dexterity(stringToInt(value));

    // Player: Strength
    } else if (chainsMatch(chain, {l_player, l_str})) {
        m_player.strength(stringToInt(value));

    // Player: Vitality
    } else if (chainsMatch(chain, {l_player, l_vit})) {
        m_player.vitality(stringToInt(value));

    // Player: Inventory
    } else if (chainsMatch(chain, {l_player, l_inventory})) {
        // Do nothing

    // Player: Inventory: Item name
    } else if (chainsMatch(chain, {l_player, l_inventory, ""})) {
        Item item;
        item.name(label);
        m_player.inventory().forceAdd(item);

    // Player: Inventory: Item: Dexterity
    } else if (chainsMatch(chain, {l_player, l_inventory, "", l_dex})) {
        m_player.inventory().items().back().dexterity(stringToInt(value));

    // Player: Inventory: Item: Strength
    } else if (chainsMatch(chain, {l_player, l_inventory, "", l_str})) {
        m_player.inventory().items().back().strength(stringToInt(value));

    // Player: Inventory: Item: Vitality
    } else if (chainsMatch(chain, {l_player, l_inventory, "", l_vit})) {
        m_player.inventory().items().back().vitality(stringToInt(value));

    // Locations
    } else if (chainsMatch(chain, {l_locations})) {
        // Do nothing

    // Locations: Location name
    } else if (chainsMatch(chain, {l_locations, ""})) {
        m_locations.emplace(label, label);
        lastLocationName = label;

    // Locations: Location: Text
    } else if (chainsMatch(chain, {l_locations, "", l_txt})) {
        // Do nothing

    // Locations: Location: Text: ...
    } else if (chainsMatch(chain, {l_locations, "", l_txt, ""})) {
        m_locations[lastLocationName].appendText(line);

    // Locations: Location: Creatures
    } else if (chainsMatch(chain, {l_locations, "", l_creatures})) {
        // Do nothing

    // Locations: Location: Creatures: Creature name
    } else if (chainsMatch(chain, {l_locations, "", l_creatures, ""})) {
        Location& l = m_locations[lastLocationName];
        l.creatures().emplace(label, Creature());
        l.creatures()[label].name(label);
        lastCreatureName = label;

    // Locations: Location: Creatures: Creature: Health
    } else if (
        chainsMatch(chain, {l_locations, "", l_creatures, "", l_health})
    ) {
        m_locations[lastLocationName].creatures()[lastCreatureName]
            .health(stringToInt(value));

    // Locations: Location: Creatures: Creature: Dexterity
    } else if (chainsMatch(chain, {l_locations, "", l_creatures, "", l_dex})) {
        m_locations[lastLocationName].creatures()[lastCreatureName]
            .dexterity(stringToInt(value));

    // Locations: Location: Creatures: Creature: Strength
    } else if (chainsMatch(chain, {l_locations, "", l_creatures, "", l_str})) {
        m_locations[lastLocationName].creatures()[lastCreatureName]
            .strength(stringToInt(value));

    // Locations: Location: Creatures: Creature: Vitality
    } else if (chainsMatch(chain, {l_locations, "", l_creatures, "", l_vit})) {
        m_locations[lastLocationName].creatures()[lastCreatureName]
            .vitality(stringToInt(value));

    // Locations: Location: Creatures: Creature: Loot
    } else if (chainsMatch(chain, {l_locations, "", l_creatures, "", l_loot})) {
        // Do nothing

    // Locations: Location: Creatures: Creature: Loot: Item name
    } else if (
        chainsMatch(chain, {l_locations, "", l_creatures, "", l_loot, ""})
    ) {
        Item item;
        item.name(label);
        m_locations[lastLocationName].creatures()[lastCreatureName].inventory()
            .forceAdd(item);

    // Locations: Location: Creatures: Creature: Loot: Item: Dexterity
    } else if (
        chainsMatch(chain, {l_locations, "", l_creatures, "", l_loot, "",
            l_dex})
    ) {
        m_locations[lastLocationName].creatures()[lastCreatureName].inventory()
            .items().back().dexterity(stringToInt(value));

    // Locations: Location: Creatures: Creature: Loot: Item: Strength
    } else if (
        chainsMatch(chain, {l_locations, "", l_creatures, "", l_loot, "",
            l_str})
    ) {
        m_locations[lastLocationName].creatures()[lastCreatureName].inventory()
            .items().back().strength(stringToInt(value));

    // Locations: Location: Creatures: Creature: Loot: Item: Vitality
    } else if (
        chainsMatch(chain, {l_locations, "", l_creatures, "", l_loot, "",
            l_vit})
    ) {
        m_locations[lastLocationName].creatures()[lastCreatureName].inventory()
            .items().back().vitality(stringToInt(value));

    // Locations: Location: Options
    } else if (chainsMatch(chain, {l_locations, "", l_options})) {
        // Do nothing

    // Locations: Location: Options: Option
    } else if (chainsMatch(chain, {l_locations, "", l_options, l_option})) {
        m_locations[lastLocationName].addOption();

    // Locations: Location: Options: Option: Text
    } else if (
        chainsMatch(chain, {l_locations, "", l_options, l_option, l_txt})
    ) {
        m_locations[lastLocationName].options().back().text(value);

    // Locations: Location: Options: Option: Action
    } else if (
        chainsMatch(chain, {l_locations, "", l_options, l_option, l_action})
    ) {
        m_locations[lastLocationName].options().back().action(value);

    // Locations: Location: Options: Option: Target
    } else if (
        chainsMatch(chain, {l_locations, "", l_options, l_option, l_target})
    ) {
        m_locations[lastLocationName].options().back().props()
            .emplace("Target", value);

    // Locations: Location: Options: Option: Win action
    } else if (
        chainsMatch(chain, {l_locations, "", l_options, l_option, l_winAction})
    ) {
        m_locations[lastLocationName].options().back().props()
            .emplace("Win action", value);

    // Locations: Location: Options: Option: Win target
    } else if (
        chainsMatch(chain, {l_locations, "", l_options, l_option, l_winTarget})
    ) {
        m_locations[lastLocationName].options().back().props()
            .emplace("Win target", value);
    }
}

void World::createHero() {
    // Tell player info
    tell(
        "You get " + to_string(m_skillPoints) + " skill points total to give to"
        " your dexterity, strength, and vitality skills. Plus you get 1 point"
        " to each skill on top of that."
    );
    // Have player assign skill points. Do so again while they choose "Redo".
    int create = 1,
        dex, str, vit;
    while (create == 1) {
        // Set default values
        dex = -1;
        str = -1;
        vit = -1;
        int skillPts = m_skillPoints;
        // Prompt for dexterity. Do so again while value is invalid.
        while (dex < 0)
            dex = promptSkill("Dexterity", skillPts);
        skillPts -= dex;
        // Prompt for strength. Do so again while value is invalid.
        while (str < 0)
            str = promptSkill("Strength", skillPts);
        skillPts -= str;
        // Prompt for vitality. Do so again while value is invalid.
        while (vit < 0)
            vit = promptSkill("Vitality", skillPts);
        skillPts -= vit;
        // Add bonus point to each skill
        dex += 1;
        str += 1;
        vit += 1;
        // Tell player their chosen skills and ask if they want to play or redo
        tell(
            "Your hero's stats are:\nDexterity: " + to_string(dex) +
            "\nStrength: " + to_string(str) + "\nVitality: " + to_string(vit) +
            "\n\nYou have " + to_string(skillPts) + " unspent skill points."
        );
        create = io::choose({"Play", "Redo"});
    }
    // Set chosen skills to player's hero
    m_player.dexterity(dex);
    m_player.strength(str);
    m_player.vitality(vit);
    m_player.restoreHealth();
}

void World::play() {
    // Move through locations while hero lives and player wants to continue
    bool continuePlaying = true;
    while (continuePlaying && m_player.lives()) {
        string locationName = m_player.location();
        continuePlaying = showLocation(m_locations[locationName]);
    }
    // Loop ended -> either player wants to save and exit, or their hero died
    if (!m_player.lives()) {
        tell("You have lost. Game over");
    } else {
        tell("Game saved.");
    }
}

void World::saveWorld(const string filename) const {
    // Open file
    std::ofstream file(filename);
    // Convert labelEnd to string
    string separator;
    separator.push_back(labelEnd);

    // Write world to book file
    writeLine(file, 0, "Name" + separator + m_name);
    writeLine(file, 0, "Player" + separator);
    writeLine(file, 1, "Location" + separator + m_player.location());
    writeLine(file, 1, "Inventory size" + separator +
        to_string(m_player.inventory().capacity()));
    writeLine(file, 1, "Item equipped" + separator + to_string(m_player
        .inventory().equipped()));
    writeLine(file, 1, "Health" + separator + to_string(m_player.health()));
    writeLine(file, 1, "Dexterity" + separator + to_string(m_player
        .dexterity()));
    writeLine(file, 1, "Strength" + separator + to_string(m_player.strength()));
    writeLine(file, 1, "Vitality" + separator + to_string(m_player.vitality()));
    writeLine(file, 1, "Inventory" + separator);
    for (const auto& item: m_player.inventory().items()) {
        writeLine(file, 2, item.name() + separator);
        writeLine(file, 3, "Dexterity" + separator + to_string(item
            .dexterity()));
        writeLine(file, 3, "Strength" + separator + to_string(item.strength()));
        writeLine(file, 3, "Vitality" + separator + to_string(item.vitality()));
    }
    writeLine(file, 0, "Locations" + separator);
    for (const auto& locationPtr: m_locations) {
        const auto& location = locationPtr.second;
        writeLine(file, 1, location.name() + separator);
        writeLine(file, 2, "Text" + separator);
        writeLine(file, 3, location.text());
        writeLine(file, 2, "Creatures" + separator);
        for (const auto& creature: location.creatures()) {
            const Creature& c = creature.second;
            string hlt = to_string(c.health());
            string dex = to_string(c.dexterity());
            string str = to_string(c.strength());
            string vit = to_string(c.vitality());
            writeLine(file, 3, c.name() + separator);
            writeLine(file, 4, "Health" + separator + hlt);
            writeLine(file, 4, "Dexterity" + separator + dex);
            writeLine(file, 4, "Strength" + separator + str);
            writeLine(file, 4, "Vitality" + separator + vit);
            writeLine(file, 4, "Loot" + separator);
            for (const auto& item: c.inventory().items()) {
                writeLine(file, 5, item.name() + separator);
                dex = to_string(item.dexterity());
                str = to_string(item.strength());
                vit = to_string(item.vitality());
                writeLine(file, 6, "Dexterity" + separator + dex);
                writeLine(file, 6, "Strength" + separator + str);
                writeLine(file, 6, "Vitality" + separator + vit);
            }
        }
        writeLine(file, 2, "Options" + separator);
        for (const auto& option: location.options()) {
            writeLine(file, 3, "-" + separator);
            writeLine(file, 4, "Text" + separator + option.text());
            writeLine(file, 4, "Action" + separator + option.action());
            for (const auto& prop: option.props())
                writeLine(file, 4, prop.first + separator + prop.second);
        }
    }

    // Close file
    file.close();
}

bool World::showLocation(Location& location) {
    // Get list of options
    vector<string> options;
    location.listOptions(options);
    const int choiceSaveAndExit = (int) options.size() - 1;
    const int choiceEquip = choiceSaveAndExit - 1;
    // Tell player location info
    tell("Location: " + location.name() + "\n\n" + location.text());
    // Have them choose from options. Do so again while they choose equip option
    int choice = choiceEquip;
    while (choice == choiceEquip) {
        choice = io::choose(options);
        // Save and exit
        if (choice == choiceSaveAndExit) {
            saveWorld(".save");
            return false;
        }
        // Equip item from inventory
        if (choice == choiceEquip)
            m_player.inventory().equip();
    }
    // Get chosen option info
    Option& opt = location.options()[choice];
    // If action is goto, set its target as the next location
    if (opt.action() == "goto") {
        m_player.location(opt.props()["Target"]);
    // If action is fight, have player fight its target
    } else if (opt.action() == "fight") {
        // Get target creature
        Creature& target = location.creatures()[opt.props()["Target"]];
        // Have player and creature take turns in attacking; player starts
        int k = 0;
        while (
            m_player.lives() &&
            target.lives()
        ) {
            if (k % 2 == 0) {
                // Player attacks creature
                m_player.attack(target);
            } else {
                // Creature attacks player
                target.attack(m_player);
            }
            k++;
        }
        // If hero died, game over
        if (!m_player.lives())
            return false;
        // Player won the fight
        // If win action is goto, set win target as the next location
        if (opt.props()["Win action"] == "goto") {
            m_player.location(opt.props()["Win target"]);
        // If win action is end, tell player that they've won the game and exit
        } else if (opt.props()["Win action"] == "end") {
            tell("Congratulations, you have won!");
            return false;
        }
    }
    return true;
}
