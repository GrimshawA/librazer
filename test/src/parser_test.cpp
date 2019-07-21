#include "parser_test.h"
#include "gtest/gtest.h"

#include <razer/frontend/Parser/TokenParser.h>
#include <razer/frontend/Parser/RzParser.h>

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

