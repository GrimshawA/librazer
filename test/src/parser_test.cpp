#include "parser_test.h"
#include "gtest/gtest.h"

#include <RazerParser/Parser/TokenParser.h>
#include <RazerParser/Parser/RzParser.h>

const char* fragmentTest01 = "import std;";

class ParserTest : public ::testing::Test {
protected:

};

TEST_F(ParserTest, TestTokenParser) {
    RzTokenParser tk;
    tk.tokenize(fragmentTest01);

    EXPECT_EQ(tk.getTokens().size(), 4);

    tk.clear();

    EXPECT_EQ(tk.getTokens().empty(), true);
}

