#pragma once

#include "Location.hpp"
#include "Player.hpp"

#include <map>
#include <string>
#include <vector>

class World {
    public:
        /** Location container type */
        typedef std::map<std::string, Location> LocationContainer;

        /** Read world from file */
        World(const std::string& filename);

        /** Have player create their hero */
        void createHero();
        /** Play world */
        void play();
        /** Save world to file */
        void saveWorld(const std::string filename) const;
        /**
         * Play current location. Return false if player wants to save and exit
         * or their hero died; true otherwise.
         */
        bool showLocation(Location& location);

    private:
        Player m_player;
        int m_skillPoints;
        std::string m_name;
        LocationContainer m_locations;

        /** Parse line from book file */
        void readLine(std::string& line, std::vector<std::string>& chain);

    public:
        class FileErr {};
        class NoLocationsErr {};
        class OptionTextErr {};
        class PlayerLocationErr {};
};
