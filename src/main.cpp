#include "Creature.hpp"
#include "Location.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "io.hpp"

#include <string>

using io::tell;
using std::string;

class InvalidBookName {};

void loadGame() {
    try {
        World world(".save");
        tell("Game successfully loaded.");
        world.play();
    } catch (const World::FileErr& e) {
        tell("Error while loading game: Book file couldn't be opened.");
    } catch (...) {
        tell("An error occured.");
    }
}

void newGame() {;
    // Have player choose which book to open
    string bookName;
    io::prompt("Book name: ", bookName);
    // Try to open book and play
    try {
        // Book name can't be empty
        if (bookName == "")
            throw InvalidBookName();
        // Open and parse book
        tell("Opening " + bookName);
        World world("examples/" + bookName);
        // Have player create their hero
        tell("Book successfully opened. Let's go create your hero!");
        world.createHero();
        // Play
        world.play();
    // Catch errors
    } catch (const World::FileErr& e) {
        tell("Error while creating game: Book file couldn't be opened.");
    } catch (const World::NoLocationsErr& e) {
        tell("Error while creating game: No locations found in book.");
    } catch (const World::OptionTextErr& e) {
        tell("Error while creating game: Every option within a location must"
            " have unique text.");
    } catch (const World::PlayerLocationErr& e) {
        tell("Error while creating game: Invalid player location.");
    } catch (const InvalidBookName& e) {
        tell("Error while creating game: Invalid book name provided (\"" +
            bookName + "\")");
    } catch (...) {
        tell("An error occured.");
    }
}

/**
 * Have player choose from main menu and load/create game (if desired). Return
 * false if player wishes to exit, true otherwise.
 */
bool menu() {
    // Have player choose what to do
    int choice = io::choose({"New game", "Load game", "Exit"});
    switch (choice) {
        // New game
        case 0:
            newGame();
            break;
        // Load game
        case 1:
            loadGame();
            break;
        // Exit
        case 2:
            return false;
    }
    return true;
}

int main() {
    // Initialize curses
    io::initCurses();
    // Welcome
    tell("Welcome to my simple text RPG engine.");
    // Display menu, play
    while (menu());
    // Goodbye
    tell("See you next time!");
    // Finalize curses
    io::finCurses();
    return 0;
}
