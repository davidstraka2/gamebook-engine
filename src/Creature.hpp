#pragma once

#include "Entity.hpp"

class Creature: public Entity {
    public:
        /** Attack target */
        void attack(Entity& target) override;
        /** Defend from and attack of certain damage dealt by attacker */
        void defend(Attr damage, Entity& attacker) override;
        /** Receive item */
        void receive(const Item& item) override;
};
