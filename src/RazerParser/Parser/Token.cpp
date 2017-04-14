#include <RazerParser/Parser/Token.h>

RzToken::RzToken()
    : m_pos(-1)
    , type(RZTK_UNKNOWN) {

}

RzToken::RzToken(TokenType type, const std::string& text, int pos)
    : m_pos(pos)
    , type(type)
    , text(text) {

}

int RzToken::pos() const {
    return m_pos;
}

const char* RzToken::str() {
    return text.c_str();
}

std::string RzToken::extract_stringliteral() {
    std::string s = text;
    if (type == RZTK_STRINGLITERAL) {
        s.erase(s.begin());
        s.erase(s.begin() + s.size() - 1);
    }
    return s;
}
