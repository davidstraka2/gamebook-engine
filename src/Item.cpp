#include "Item.hpp"

using std::string;

Item::Item():
    // Set default values
    m_dexterity(0),
    m_strength(0),
    m_vitality(0) {}

Item::Attr Item::dexterity() const {
    return m_dexterity;
}

void Item::dexterity(Attr value) {
    m_dexterity = value;
}

Item::Attr Item::strength() const {
    return m_strength;
}

void Item::strength(Attr value) {
    m_strength = value;
}

Item::Attr Item::vitality() const {
    return m_vitality;
}

void Item::vitality(Attr value) {
    m_vitality = value;
}

const string& Item::name() const {
    return m_name;
}

void Item::name(const string& value) {
    m_name = value;
}
