#include <parser_test/parser_test.h>

#include <RazerParser/Parser/TokenParser.h>
#include <RazerParser/Parser/RzParser.h>

const char* fragmentTest01 = "import std;";

void RzTestParser::execute() {

    {
        RzTokenParser tk;
        tk.tokenize(fragmentTest01);

        RZTASSERT(tk.getTokens().size() == 4);

        tk.clear();

        RZTASSERT(tk.getTokens().empty());
    }

}
