#include "Entity.hpp"

using std::string;

const Entity::Attr vitalityHealthMultiplier = 50;

Entity::VirtualMethodUseErr::VirtualMethodUseErr(const string& msg):
    m_msg(msg) {}

Entity::Entity():
    // Set default values
    m_dexterity(1),
    m_health(-1),
    m_strength(1),
    m_vitality(1),
    m_isAlive(true) {}

void Entity::attack(Entity& target) {
    throw VirtualMethodUseErr("attack: " + target.name());
}

void Entity::defend(Attr damage, Entity& attacker) {
    throw VirtualMethodUseErr("defend: " + attacker.name() + ", " +
        std::to_string(damage));
}

void Entity::receive(const Item& item) {
    throw VirtualMethodUseErr("receive: " + item.name());
}

Entity::Attr Entity::dexterity() const {
    return m_dexterity;
}

void Entity::dexterity(Attr value) {
    m_dexterity = value;
}

Entity::Attr Entity::health() const {
    return m_health;
}

void Entity::health(Attr value) {
    m_health = value;
}

Entity::Attr Entity::strength() const {
    return m_strength;
}

void Entity::strength(Attr value) {
    m_strength = value;
}

Entity::Attr Entity::vitality() const {
    return m_vitality;
}

void Entity::vitality(Attr value) {
    m_vitality = value;
}

const Inventory& Entity::inventory() const {
    return m_inventory;
}

Inventory& Entity::inventory() {
    return m_inventory;
}

const string& Entity::name() const {
    return m_name;
}

void Entity::name(const string& value) {
    m_name = value;
}

Entity::Attr Entity::basicDamage() const {
    auto damage = m_strength;
    // If an item is equipped, add its strength to damage
    if (m_inventory.equipped() >= 0)
        damage += m_inventory.getEquippedItem().strength();
    // Damage can't be negative
    if (damage < 0)
        damage = 0;
    return damage;
}

Entity::Attr Entity::basicDefense() const {
    auto defense = m_dexterity;
    // If an item is equipped, add its dexterity to defense
    if (m_inventory.equipped() >= 0)
        defense += m_inventory.getEquippedItem().dexterity();
    // Defense can't be negative
    if (defense < 0)
        defense = 0;
    return defense;
}

void Entity::die(Entity& attacker) {
    m_isAlive = false;

    // Attacker receives loot
    for (const auto& item: m_inventory.items())
        attacker.receive(item);
}

bool Entity::lives() const {
    if (m_isAlive)
        return true;
    return false;
}

void Entity::restoreHealth() {
    m_health = m_vitality * vitalityHealthMultiplier;
}
