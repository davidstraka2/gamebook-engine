#pragma once

#include "Inventory.hpp"

#include <string>

class Entity {
    public:
        /** Attribute type */
        typedef int Attr;

        Entity();
        virtual ~Entity() = default;

        /** Attack target */
        virtual void attack(Entity& target);
        /** Defend from and attack of certain damage dealt by attacker */
        virtual void defend(Attr damage, Entity& attacker);
        /** Receive item */
        virtual void receive(const Item& item);

        /** Get dexterity */
        Attr dexterity() const;
        /** Set dexterity */
        void dexterity(Attr value);

        /** Get health */
        Attr health() const;
        /** Set health */
        void health(Attr value);

        /** Get strength */
        Attr strength() const;
        /** Set strength */
        void strength(Attr value);

        /** Get vitality */
        Attr vitality() const;
        /** Set vitality */
        void vitality(Attr value);

        /** Get const access to inventory */
        const Inventory& inventory() const;
        /** Get non-const access to inventory */
        Inventory& inventory();

        /** Get name */
        const std::string& name() const;
        /** Set name */
        void name(const std::string& value);

        /** Get basic damage (without any multiplier) */
        Attr basicDamage() const;
        /** Get basic defense (without any multiplier) */
        Attr basicDefense() const;
        /** Die due to attacker */
        void die(Entity& attacker);
        /** Is entity alive? */
        bool lives() const;
        /** Restore entity to full health */
        void restoreHealth();

    protected:
        Attr m_dexterity;
        Attr m_health;
        Attr m_strength;
        Attr m_vitality;
        Inventory m_inventory;
        bool m_isAlive;
        std::string m_name;

    public:
        class VirtualMethodUseErr {
            public:
                VirtualMethodUseErr(const std::string& msg);

                /** Get message */
                const std::string& msg() const;

            private:
                std::string m_msg;
        };
};
