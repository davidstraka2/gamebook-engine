#include "Creature.hpp"

#include "util.hpp"

using util::randomInt;

// Attack and defense multipliers
const Entity::Attr attackMultiplierMin = 5;
const Entity::Attr attackMultiplierMax = 10;
const Entity::Attr defenseMultiplierMin = 5;
const Entity::Attr defenseMultiplierMax = 10;

void Creature::attack(Entity& target) {
    // Get creature's basic damage
    auto damage = basicDamage();
    // Multiply damage by random multiplier
    damage *= randomInt(attackMultiplierMin, attackMultiplierMax);
    // Have target defend against damage
    target.defend(damage, *this);
}

void Creature::defend(Attr damage, Entity& attacker) {
    // Get creature's basic defense
    auto defense = basicDefense();
    // Multiply defense by random multiplier
    defense *= randomInt(defenseMultiplierMin, defenseMultiplierMax);
    // Subtract defense from damage
    damage -= defense;

    // If there is still damage to be dealt, subtract it from health
    if (damage > 0) {
        m_health -= damage;
        // If health is now <= 0, creature is dead (give loot to attacker)
        if (m_health <= 0)
            die(attacker);
    }
}

void Creature::receive(const Item& item) {
    // Creature's don't care about inventory space
    m_inventory.forceAdd(item);
}
