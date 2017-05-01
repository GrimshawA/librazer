#include <RazerParser/Parser/TokenParser.h>
#include <Base/StringUtils.h>
#include <Logger.h>

RzTokenParser::RzTokenParser()
    : i(-1) {

}

std::vector<RzToken>& RzTokenParser::getTokens() {
    return tokens;
}

void RzTokenParser::clear() {
    tokens.clear();
}

void RzTokenParser::tokenize(const std::string& src) {
    if (src.empty())
        return;

    program_source = src;
    i = -1;
    m_cursor.line = 0;
    m_cursor.col = 0;

    RzToken token = getToken();
    while (token.type != RZTK_EOF) {
        tokens.push_back(token);
        token = getToken();
    }
    tokens.push_back(token); // push the eof
}

RzToken RzTokenParser::getToken() {
    // We are at program_source[i], let's get the next token

    RzToken token;
    char LastChar = ' ';

    // If i is already at the end, we give back the end of file token, which finishes the stream
    int sourceEnd = program_source.size() - 1;
    if (i >= sourceEnd) {
        token.type = RZTK_EOF;
        token.text = "EOF";
        return token;
    }

    LastChar = program_source[++i];

    if (LastChar == EOF) {
        token.type = RZTK_EOF;
        token.text = "EOF";
        return token;
    }

    while (LastChar == '\r') {
        // skip \r entirely
        LastChar = program_source[++i];
    }

    // we have room to work, skip whitespace
    while (LastChar != '\n' && isspace(LastChar))
        LastChar = program_source[++i];

    if (LastChar == '\n') {
        // tokenize every line for optional ;

        m_cursor.line += 1;
        token.text = "\n";
        token.type = RZTK_NEWLINE;
        return token;
    }

    // Check for // comments
    if (LastChar == '/') {
        if (program_source[i + 1] == '/') {
            token.text = "/";

            // we have a comment
            while ((LastChar = program_source[++i]) != '\n') {
                token.text += LastChar;
            }

            token.type = RZTK_LINECOMMENT;
            return token;
        }
        else if (program_source[i + 1] == '*') {
            i++;
            LastChar = program_source[++i];

            //Log("First char %c", LastChar);

            bool foundPair = false;
            while (!foundPair) {
                if (LastChar == '*' && program_source[i + 1] == '/') {
                    ++i;
                    token.type = RZTK_MULTICOMMENT;
                    return token;
                }

                LastChar = program_source[++i];
            }
        }
    }

    if (LastChar == '(') {
        token.text = "(";
        token.type = RZTK_OPENPAREN;
        return token;
    }

    if (LastChar == ')') {
        token.text = ")";
        token.type = RZTK_CLOSEPAREN;
        return token;
    }

    if (LastChar == ':') {
        token.text = ":";
        token.type = RZTK_COLON;
        return token;
    }

    if (LastChar == '.') {
        token.text = ".";
        token.type = RZTK_DOT;
        return token;
    }

    if (LastChar == '[') {
        token.text = "[";
        token.type = RZTK_OPENSQBRACKET;
        return token;
    }

    if (LastChar == ']') {
        token.text = "]";
        token.type = RZTK_CLOSESQBRACKET;
        return token;
    }

    if (LastChar == ',') {
        token.text = ",";
        token.type = RZTK_COMMA;
        return token;
    }

    if (LastChar == '~') {
        token.text = "~";
        token.type = RZTK_TILDE;
        return token;
    }

    if (LastChar == '{') {
        token.text = "{";
        token.type = RZTK_OPENBRACKET;
        return token;
    }

    if (LastChar == '}') {
        token.text = "}";
        token.type = RZTK_CLOSEBRACKET;
        return token;
    }

    if (LastChar == ';') {
        token.type = RZTK_SEMICOLON;
        token.text = ";";
        return token;
    }

    if (LastChar == '@') {
        token.type = RZTK_HANDLE;
        token.text = "@";
        return token;
    }

    if (LastChar == '#') {
        token.type = RZTK_DIRECTIVE;
        token.text = "#";
        return token;
    }

    if (LastChar == '-') {
        if (program_source[i + 1] == '>') {
            i++;
            token.type = RZTK_ARROW;
            token.text = "->";
            return token;
        }
    }

    if (LastChar == '+') {
        if (program_source[i + 1] == '+') {
            i++;
            token.type = RZTK_INCREMENT;
            token.text = "++";
            return token;
        }
    }

    if (LastChar == '-') {
        if (program_source[i + 1] == '-') {
            i++;
            token.type = RZTK_DECREMENT;
            token.text = "--";
            return token;
        }
    }

    if (LastChar == '=' && program_source[i + 1] == '>') {
        token.type = RZTK_LAMBDA;
        token.text = "=>";
        return token;
    }

    if (LastChar == '|' && program_source[i + 1] == '|') {
        token.type = RZTK_BINOP;
        token.text = "||";
        ++i;
        return token;
    }

    if (LastChar == '&' && program_source[i + 1] == '&') {
        token.type = RZTK_BINOP;
        token.text = "&&";
        ++i;
        return token;
    }

    if (LastChar == '>' && program_source[i + 1] == '=') {
        token.type = RZTK_BINOP;
        token.text = ">=";
        ++i;
        return token;
    }

    if (LastChar == '<' && program_source[i + 1] == '=') {
        token.type = RZTK_BINOP;
        token.text = "<=";
        ++i;
        return token;
    }

    // operators
    if (LastChar == '>' || LastChar == '<' || LastChar == '+' || LastChar == '-' || LastChar == '*' || LastChar == '/' || LastChar == '^' ||
            LastChar == '=' || LastChar == '%') {
        token.type = RZTK_BINOP;
        token.text += LastChar;
        return token;
    }

    // We're facing a string literal
    if (LastChar == '"') {
        token.type = RZTK_STRINGLITERAL;
        token.text += LastChar;
        while ((LastChar = program_source[++i]) != '"') {
            token.text += LastChar;
        }
        token.text += LastChar;
        EscapeString(token.text);
        return token;
    }

    // We're looking at a number literal
    if (isdigit(LastChar)) {
        token.text += LastChar;
        while (isdigit((LastChar = program_source[++i]))) {
            token.text += LastChar;
        }
        // digits are over, it can still be a floating point literal
        if (LastChar == '.') {
            token.text += ".";

            // read the remaining digits for the floating point number
            while (isdigit((LastChar = program_source[++i]))) {
                token.text += LastChar;
            }
            if (LastChar == 'f') {
                token.type = RZTK_FLOATLITERAL;
            }
            else {
                token.type = RZTK_FLOATLITERAL;
            }
        }
        else {
            token.type = RZTK_INTLITERAL;
        }

        --i;

        return token;
    }

    // let's find identifier
    if (isalpha(LastChar)) {
        token.text += LastChar;
        //token.pos = i;

        while (isalpha((LastChar = program_source[++i])) || isdigit(LastChar) || LastChar == '_') {
            token.text += LastChar;
        }

        // backtrack i by 1, so the next character read is the relevant item
        i--;
        if (token.text == "class")
            token.type = RZTK_CLASS;
        else if (token.text == "struct")
            token.type = RZTK_STRUCT;
        else if (token.text == "null" || token.text == "nullptr")
            token.type = RZTK_NULL;
        else if (token.text == "fn")
            token.type = RZTK_FUNCTION;
        else if (token.text == "function")
            token.type = RZTK_FUNCTION;
        else if (token.text == "enum")
            token.type = RZTK_ENUM;
        else if (token.text == "union")
            token.type = RZTK_UNION;
        else if (token.text == "cast")
            token.type = RZTK_CAST;
        else if (token.text == "if")
            token.type = RZTK_IF;
        else if (token.text == "else")
            token.type = RZTK_ELSE;
        else if (token.text == "while")
            token.type = RZTK_WHILE;
        else if (token.text == "do")
            token.type = RZTK_DO;
        else if (token.text == "const")
            token.type = RZTK_CONST;
        else if (token.text == "new")
            token.type = RZTK_NEW;
        else if (token.text == "delete")
            token.type = RZTK_DELETE;
        else if (token.text == "sizeof")
            token.type = RZTK_SIZEOF;
        else if (token.text == "typeof")
            token.type = RZTK_TYPEOF;
        else if (token.text == "operator")
            token.type = RZTK_OPERATOR;
        else if (token.text == "for")
            token.type = RZTK_FOR;
        else if (token.text == "foreach")
            token.type = RZTK_FOREACH;
        else if (token.text == "asm")
            token.type = RZTK_ASM;
        else if (token.text == "auto")
            token.type = RZTK_AUTO;
        else if (token.text == "true")
            token.type = RZTK_TRUE;
        else if (token.text == "false")
            token.type = RZTK_FALSE;
        else if (token.text == "return")
            token.type = RZTK_RETURN;
        else if (token.text == "public")
            token.type = RZTK_PUBLIC;
        else if (token.text == "protected")
            token.type = RZTK_PROTECTED;
        else if (token.text == "private")
            token.type = RZTK_PRIVATE;
        else if (token.text == "namespace")
            token.type = RZTK_NAMESPACE;
        else if (token.text == "using")
            token.type = RZTK_USING;
        else if (token.text == "break")
            token.type = RZTK_BREAK;
        else if (token.text == "goto")
            token.type = RZTK_GOTO;
        else if (token.text == "continue")
            token.type = RZTK_CONTINUE;
        else if (token.text == "as")
            token.type = RZTK_AS;
        else if (token.text == "in")
            token.type = RZTK_IN;
        else if (token.text == "switch")
            token.type = RZTK_SWITCH;
        else if (token.text == "case")
            token.type = RZTK_CASE;
        else if (token.text == "default")
            token.type = RZTK_DEFAULT;
        else if (token.text == "static")
            token.type = RZTK_STATIC;
        else if (token.text == "typedef")
            token.type = RZTK_TYPEDEF;
        else if (token.text == "and")
            token.type = RZTK_AND;
        else if (token.text == "is")
            token.type = RZTK_IS;
        else if (token.text == "isnt")
            token.type = RZTK_ISNOT;
        else if (token.text == "not")
            token.type = RZTK_NOT;
        else if (token.text == "import")
            token.type = RZTK_IMPORT;
        else
            token.type = RZTK_IDENTIFIER;
        return token;
    }
    return token;
}

void RzTokenParser::print() {
    for (auto& t : tokens) {
        RZLOG("Token: %s\n", t.text.c_str());
    }
}
