#pragma once

#include "Creature.hpp"
#include "Option.hpp"

#include <map>
#include <string>
#include <vector>

class Location {
    public:
        /** Creature container type */
        typedef std::map<std::string, Creature> CreatureContainer;
        /** Option container type */
        typedef std::vector<Option> OptionContainer;

        Location() {}
        Location(const std::string& name);

        /** Get const access to creatures */
        const CreatureContainer& creatures() const;
        /** Get non-const access to creatures */
        CreatureContainer& creatures();

        /** Get const access to options */
        const OptionContainer& options() const;
        /** Get non-const access to options */
        OptionContainer& options();

        /** Get name */
        const std::string& name() const;
        /** Set name */
        void name(const std::string& value);

        /** Get text */
        const std::string& text() const;
        /** Set text */
        void text(const std::string& value);

        /** Add empty option and get non-const access to it */
        Option& addOption();
        /** Append text to location text (space is auto added before text) */
        void appendText(const std::string& text);
        /** List all options */
        void listOptions(std::vector<std::string>& list) const;

    private:
        CreatureContainer m_creatures;
        OptionContainer m_options;
        std::string m_name;
        std::string m_text;
};
