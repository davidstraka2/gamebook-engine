#include "io.hpp"

/*
 * Curses resources:
 * http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
 * https://www.linuxjournal.com/content/getting-started-ncurses
 * http://www.cs.ukzn.ac.za/~hughm/os/notes/ncurses.html
 */

#include <ncurses.h>
#include <ncurses/menu.h> // NOTE: Replace with "#include <menu.h>" if needed

#include <cstring>

using std::string;
using std::vector;

int io::choose(const vector<string>& options) {
    // Clear console
    clear();

    // Create items
    vector<char*> itemNames;
    vector<ITEM*> items;
    for (const auto& option: options) {
        // Get stable char* from options string
        char* itemName = new char[option.length() + 1];
        strcpy(itemName, option.c_str());
        // Create new item with no description
        ITEM* item = new_item(itemName, NULL);
        items.push_back(item);
        // Store itemName char* to free later
        itemNames.push_back(itemName);
    }
    // Errors without this
    items.push_back((ITEM*) NULL);

    // Create menu of items
    auto menu = new_menu((ITEM**) (items.data()));
    post_menu(menu);

    // Get console row count
    auto rowCount = getmaxy(stdscr);
    // Get current cursor row and column
    int row, column;
    getyx(stdscr, row, column);
    // Print info to console
    mvprintw(rowCount - 1, 0, "(use up and down arrows to switch between"
        " options; press enter to confirm...)\n");
    // Move cursor back
    move(row, column);

    // Show everything to user
    refresh();

    // Let user control the menu and make a choice
    const char* choice;
    int key;
    bool loop = true;
    while (loop) {
        key = getch();
        choice = item_name(current_item(menu));

        switch (key) {
            // Down arrow
            case KEY_DOWN:
                // If already at last item, go to first, otherwise move down
                if (choice == itemNames.back()) {
                    menu_driver(menu, REQ_FIRST_ITEM);
                } else {
                    menu_driver(menu, REQ_DOWN_ITEM);
                }
                break;

            // Up arrow
            case KEY_UP:
                // If already at first item, go to last, otherwise move up
                if (choice == itemNames.front()) {
                    menu_driver(menu, REQ_LAST_ITEM);
                } else {
                    menu_driver(menu, REQ_UP_ITEM);
                }
                break;

            // Enter
            case 10:
                loop = false;
                break;
        }
    }

    // Get option index
    int optionIndex = 0;
    for (size_t i = 0; i < itemNames.size(); i++)
        if (choice == itemNames[i])
            optionIndex = (int) i;

    // Stop showing menu and free resources
    unpost_menu(menu);
    free_menu(menu);
    for (auto& item: items)
        free_item(item);
    for (auto& itemName: itemNames)
        delete[] itemName;

    return optionIndex;
}

void io::finCurses() {
    endwin();
}

void io::initCurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    clear();
}

void io::prompt(const string& output, string& input) {
    // Resources: https://stackoverflow.com/a/28585207
    // Clear console
    clear();
    // Get console row count
    auto rowCount = getmaxy(stdscr);
    // Print output text to console
    printw("%s", output.c_str());
    // Get current cursor row and column
    int row, column;
    getyx(stdscr, row, column);
    // Print info to console
    mvprintw(rowCount - 1, 0, "(press enter to confirm...)\n");
    // Move cursor back
    move(row, column);
    // Refresh console
    refresh();
    // Default console settings
    nocbreak();
    echo();
    // Read input while not enter
    int ch = getch();
    while (ch != 10) {
        input.push_back(ch);
        ch = getch();
    }
    // Restore console settings
    cbreak();
    noecho();
}

void io::tell(const string& text) {
    // Clear console
    clear();
    // Get console row count
    int rowCount = getmaxy(stdscr);
    // Print text to console
    printw("%s\n", text.c_str());
    mvprintw(rowCount - 1, 0, "(press any key to continue...)\n");
    refresh();
    // Wait for any input
    getch();
}
