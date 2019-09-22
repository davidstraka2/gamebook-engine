#pragma once

#include "Entity.hpp"

#include <string>

class Player: public Entity {
    public:
        /** Attack target */
        void attack(Entity& target) override;
        /** Defend from and attack of certain damage dealt by attacker */
        void defend(Attr damage, Entity& attacker) override;
        /** Receive item */
        void receive(const Item& item) override;

        /** Get location */
        const std::string& location() const;
        /** Set location */
        void location(const std::string& value);

    private:
        std::string m_location;
};
