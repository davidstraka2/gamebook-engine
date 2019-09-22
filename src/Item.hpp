#pragma once

#include <string>

class Item {
    public:
        /** Attribute tyoe */
        typedef int Attr;

        Item();

        /** Get dexterity */
        Attr dexterity() const;
        /** Set dexterity */
        void dexterity(Attr value);

        /** Get strength */
        Attr strength() const;
        /** Set strength */
        void strength(Attr value);

        /** Get vitality */
        Attr vitality() const;
        /** Set vitality */
        void vitality(Attr value);

        /** Get name */
        const std::string& name() const;
        /** Set name */
        void name(const std::string& value);

    private:
        Attr m_dexterity;
        Attr m_strength;
        Attr m_vitality;
        std::string m_name;
};
