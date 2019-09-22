#include "util.hpp"

#include <random>
#include <sstream>

using std::string;
using std::vector;

const char indentationChar = '\t';

std::random_device seed;
std::mt19937 rng(seed());

bool util::chainsMatch(
    const vector<string>& chainA,
    const vector<string>& chainB
) {
    if (chainA.size() != chainB.size())
        return false;
    for (size_t i = 0; i < chainA.size(); i++) {
        if (
            chainA[i] != chainB[i] &&
            chainA[i].length() != 0 &&
            chainB[i].length() != 0
        ) {
            return false;
        }
    }
    return true;
}

/** Get indentation of given level. Return reference to indentation. */
string& util::indent(size_t level, string& indentation) {
    for (size_t i = 0; i < level; i++)
        indentation += indentationChar;
    return indentation;
}

void util::manageChain(
    vector<string>& chain,
    size_t level,
    const string& item
) {
    while (chain.size() > level)
        chain.pop_back();
    chain.emplace_back(item);
}

int util::randomInt(int min, int max) {
    // https://stackoverflow.com/a/13445752
    std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
    return dist(rng);
}

int util::stringToInt(const string& value) {
    std::stringstream conversionStream(value);
    int number = 0;
    conversionStream >> number;
    return number;
}

/** Trim indentation from the beginning of text, return indentation level */
size_t util::trimIndentation(string& text) {
    // Find where indentation ends
    size_t indentationCharCount = 0;
    for (auto ch: text) {
        if (ch != indentationChar)
            break;
        indentationCharCount++;
    }
    // Trim indentation and return level
    text = text.substr(indentationCharCount);
    return indentationCharCount;
}

/** Left indent line up to given level, end it with newline and write to file */
void util::writeLine(std::ofstream& file, size_t level, const string& text) {
    string indentation;
    file << indent(level, indentation) << text + "\n";
}
