#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef enum {
    tokEof,
    tokId,
    tokIf,
    tokElse,
    tokLiteral,
    tokNumber,
    tokType,
    tokEq,
    tokSC,
    tokOpenBrace,
    tokCloseBrace,
    tokOpenPar,
    tokClosePar,
    tokReturn,
} TokenType;

typedef struct {
    TokenType type;
    string content;
    int line;
    int col;
} Token;

void lexerInit(string str);
void lexerLex();
vector<Token> lexerGetTokens();
string lexerTok2Str(TokenType type);