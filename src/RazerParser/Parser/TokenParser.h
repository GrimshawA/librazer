#ifndef RZTOKENS_H__
#define RZTOKENS_H__

#include <string>
#include <vector>

#include <RazerParser/Parser/Token.h>

/**
    \class RzTokenParser
    \brief Generates a stream of token from a source program so it can be parsed meaningfully
*/
class RzTokenParser
{
public:
    RzTokenParser();

    /// Get the list of collected tokens
    std::vector<RzToken>& getTokens();

    /// Clear the cached tokens
    void clear();

    void print();

    void tokenize(const std::string& src);

private:
    RzToken getToken();

    struct SourceCursor
    {
        int line;
        int col;
    };

public:
    std::string             program_source;
    int                     i;
    std::vector<RzToken>    tokens;

private:
    SourceCursor            m_cursor;
};

#endif // RZTOKENS_H__
