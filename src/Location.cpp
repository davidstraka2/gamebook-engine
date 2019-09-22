#include "Location.hpp"

using std::string;

Location::Location(const string& name):
    m_name(name) {}

const Location::CreatureContainer& Location::creatures() const {
    return m_creatures;
}

Location::CreatureContainer& Location::creatures() {
    return m_creatures;
}

const Location::OptionContainer& Location::options() const {
    return m_options;
}

Location::OptionContainer& Location::options() {
    return m_options;
}

const string& Location::name() const {
    return m_name;
}

void Location::name(const string& value) {
    m_name = value;
}

const string& Location::text() const {
    return m_text;
}

void Location::text(const string& value) {
    m_text = value;
}

Option& Location::addOption() {
    m_options.emplace_back();
    return m_options.back();
}

void Location::appendText(const string& text) {
    if (m_text.length() == 0) {
        m_text = text;
    // Add space before appended text if it's not the first text
    } else {
        m_text += " " + text;
    }
}

void Location::listOptions(std::vector<string>& list) const {
    for (const auto& option: m_options)
        list.emplace_back(option.text());
    list.emplace_back("View inventory");
    list.emplace_back("Save and exit to menu");
}
