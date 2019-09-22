#include "Player.hpp"

#include "io.hpp"
#include "util.hpp"

using io::choose;
using io::prompt;
using io::tell;
using std::string;
using std::to_string;
using util::randomInt;
using util::stringToInt;

const Entity::Attr attackMultiplierMin = 5;
const Entity::Attr attackMultiplierMax = 10;
const Entity::Attr defenseMultiplierMin = 5;
const Entity::Attr defenseMultiplierMax = 10;

void Player::attack(Entity& target) {
    // Have player guess a number
    string guessString;
    prompt(
        "You are about to attack. Guess a number between " +
        to_string(attackMultiplierMin) + " and " +
        to_string(attackMultiplierMax) + ". If you guess right, your attack"
        " damage will be doubled!\n\nGuess: ",
        guessString
    );
    auto guess = stringToInt(guessString);

    // Generate multiplier and check guess
    auto multiplier = randomInt(attackMultiplierMin, attackMultiplierMax);
    if (guess == multiplier) {
        multiplier *= 2;
        tell("You guessed right, congratulations!");
    } else {
        tell("You guessed wrong. Better luck next time.");
    }

    // Calculate and deal damage
    auto damage = basicDamage();
    damage *= multiplier;
    auto targetHealthBefore = target.health();
    target.defend(damage, *this);
    auto targetHealthAfter = target.health();
    auto damageDealt = targetHealthBefore - targetHealthAfter;

    // Inform player of outcome
    string msg = "You dealt " + to_string(damageDealt) + " damage, target is ";
    if (targetHealthAfter > 0) {
        msg += "at " + to_string(targetHealthAfter) + " health.";
    } else {
        msg += "dead.";
    }
    tell(msg);
}

void Player::defend(Attr damage, Entity& attacker) {
    // Have player guess a number
    string guessString;
    prompt(
        "You are about to defend against " + to_string(damage) + " damage."
        " Guess a number between " + to_string(defenseMultiplierMin) + " and " +
        to_string(defenseMultiplierMax) + ". If you guess right, your defense"
        " will be doubled!\n\nGuess: ",
        guessString
    );
    auto guess = stringToInt(guessString);

    // Generate multiplier and check guess
    auto multiplier = randomInt(defenseMultiplierMin, defenseMultiplierMax);
    if (guess == multiplier) {
        multiplier *= 2;
        tell("You guessed right, congratulations!");
    } else {
        tell("You guessed wrong. Better luck next time.");
    }

    // Calculate and deal damage
    auto defense = basicDefense();
    defense *= multiplier;
    damage -= defense;
    if (damage < 0)
        damage = 0;
    m_health -= damage;

    // Inform player of outcome
    string msg;
    msg = "You've been dealt " + to_string(damage) + " damage. ";
    if (m_health > 0) {
        msg += "You're currently sitting at " + to_string(m_health) + " health.";
    } else {
        msg += "You died.";
    }
    tell(msg);

    if (m_health <= 0)
        die(attacker);
}

void Player::receive(const Item& item) {
    // Ask whether player wants to keep item
    tell(
        "You have received an item called " + item.name() + ". Its stats are:"
        " dexterity " + to_string(item.dexterity()) + ", strength " +
        to_string(item.strength()) + ", vitality " + to_string(item.vitality()) +
        ". Do you wish to keep it?"
    );

    /*
     * Have player make the choice. Then keep asking while they wish to keep the
     * item, but their inventory is full
     */
    bool loop = true;
    while (loop) {
        // Get player answer
        int answer = choose({"Keep", "Throw away"});

        // Keep item
        if (answer == 0) {
            bool added = m_inventory.add(item);
            // Item added to inventory
            if (added) {
                tell("Item put into bag.");
                loop = false;
            // No inventory space
            } else {
                tell("No inventory space!");
                // Allow player to make space
                if (m_inventory.freeSpace()) {
                    m_inventory.add(item);
                    loop = false;
                }
            }
        } else {
            loop = false;
        }
    }
}

const std::string& Player::location() const {
    return m_location;
}

void Player::location(const std::string& value) {
    m_location = value;
}
