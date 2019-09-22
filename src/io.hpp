#pragma once

#include <string>
#include <vector>

namespace io {
    /**
     * Have user choose from a list of options
     * 
     * @return Index of chosen option in list
     */
    int choose(const std::vector<std::string>& options);

    /** Finalize curses */
    void finCurses();

    /** Initialize curses */
    void initCurses();

    /**
     * Ask user for input
     * 
     * @param[in] output This will be written to the left of input field
     * @param[out] input User input
     */
    void prompt(const std::string& output, std::string& input);

    /** Print text to console and wait for any key press */
    void tell(const std::string& text);
}
