#pragma once

#include "Item.hpp"

#include <string>
#include <vector>

class Inventory {
    public:
        /** Item container type */
        typedef std::vector<Item> ItemContainer;

        Inventory();

        /** Get const access to all items */
        const ItemContainer& items() const;
        /** Get non-const access to all items */
        ItemContainer& items();

        /** Get capacity */
        int capacity() const;
        /** Set capacity */
        void capacity(int value);

        /** Get equipped item index */
        int equipped() const;
        /** Set equipped item index. Throw InvalidRangeErr if value too big. */
        void equipped(int value);

        /**
         * Add item to inventory, if there is space. Return true on success,
         * false otherwise.
         */
        bool add(const Item& item);
        /** Have player choose which item to equip, if any */
        void equip();
        /** Add item to inventory and increase capacity if needed */
        void forceAdd(const Item& item);
        /**
         * Allow player to throw away some item to make inventory space.
         * Return true if player did make space, false otherwise.
         */
        bool freeSpace();
        /**
         * Get const access to equipped item. Undefined behavior if no item
         * equipped, so check first.
         */
        const Item& getEquippedItem() const;
        /**
         * Get const access to item at given index. Throw InvalidRangeErr if
         * index is too big.
         */
        const Item& getItem(int index) const;
        /** Is there space in inventory */
        bool hasSpace() const;
        /** List all items, mark equipped item */
        void listItems(std::vector<std::string>& list) const;
        /**
         * Throw away item at given index. Throw InvalidRangeErr if index is
         * too big.
         */
        void throwAway(int index);

    private:
        ItemContainer m_items;
        int m_capacity;
        int m_equipped;

    public:
        class InvalidRangeErr {};
};
