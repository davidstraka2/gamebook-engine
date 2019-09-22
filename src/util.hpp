#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace util {
    /**
     * Check, whether two chains of strings match (use empty string where the
     * other chain doesn't have to match)
     */
    bool chainsMatch(
        const std::vector<std::string>& chainA,
        const std::vector<std::string>& chainB
    );

    /** Get indentation of given level. Return reference to indentation. */
    std::string& indent(size_t level, std::string& indentation);

    /** Pop everything up to given level from chain and push item */
    void manageChain(
        std::vector<std::string>& chain,
        size_t level,
        const std::string& item
    );

    /** Generate random integer from min to max (both inclusive) */
    int randomInt(int min, int max);

    /** Convert string to integer */
    int stringToInt(const std::string& value);

    /** Trim indentation from the beginning of text, return indentation level */
    size_t trimIndentation(std::string& text);

    /** Left indent line up to given level, end it with lf and write to file */
    void writeLine(std::ofstream& file, size_t level, const std::string& text);
}
