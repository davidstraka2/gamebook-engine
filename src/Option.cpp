#include "Option.hpp"

using std::string;

const Option::PropContainer& Option::props() const {
    return m_props;
}

Option::PropContainer& Option::props() {
    return m_props;
}

const string& Option::action() const {
    return m_action;
}

void Option::action(const string& value) {
    m_action = value;
}

const string& Option::text() const {
    return m_text;
}

void Option::text(const string& value) {
    m_text = value;
}
