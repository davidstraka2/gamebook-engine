#pragma once

#include <map>
#include <string>

class Option {
    public:
        /** Property container type */
        typedef std::map<std::string, std::string> PropContainer;

        /** Get const access to props */
        const PropContainer& props() const;
        /** Get non-const access to props */
        PropContainer& props();

        /** Get action */
        const std::string& action() const;
        /** Set action */
        void action(const std::string& value);

        /** Get text */
        const std::string& text() const;
        /** Set text */
        void text(const std::string& value);

    private:
        PropContainer m_props;
        std::string m_action;
        std::string m_text;
};
