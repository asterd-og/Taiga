#include "lexer.hpp"
#include <stdio.h>
#include <map>

using namespace std;
string code;
int pos = 0;
vector<Token> toks;
int l = 1;
int c = 1;
bool f = false;

map<TokenType, string> tok2str = {
    {TokenType::tokEof, "EOF"},
    {TokenType::tokId, "Id"},
    {TokenType::tokIf, "If"},
    {TokenType::tokElse, "Else"},
    {TokenType::tokLiteral, "Literal"},
    {TokenType::tokNumber, "Number"},
    {TokenType::tokType, "Type"},
    {TokenType::tokEq, "Eq"},
    {TokenType::tokSC, "Semicolon"},
    {TokenType::tokOpenBrace, "OpenBrace"},
    {TokenType::tokCloseBrace, "CloseBrace"},
    {TokenType::tokOpenPar, "OpenPar"},
    {TokenType::tokClosePar, "ClosePar"},
    {TokenType::tokReturn, "Return"},
};

map<string, TokenType> keywords = {
    {"if", TokenType::tokIf},
    {"else", TokenType::tokIf},
    {"u8", TokenType::tokType},
    {"u16", TokenType::tokType},
    {"u32", TokenType::tokType},
    {"u64", TokenType::tokType},
    {"i8", TokenType::tokType},
    {"i16", TokenType::tokType},
    {"i32", TokenType::tokType},
    {"i64", TokenType::tokType},
    {"v0", TokenType::tokType},
    {"str", TokenType::tokType},
    {"return", TokenType::tokReturn},
};

void lexerInit(string _code) {
    code = _code;
}

void addToken(TokenType type, string content) {
    toks.push_back({
        .type = type,
        .content = content,
        .line = l,
        .col = c
    });
}

bool isSkippable(char c) {
    return ((code[pos] == ' '  ||
            code[pos] == '\t') ? true : false);
}

bool isAlpha(char c) {
    return ((code[pos] >= 'a' && code[pos] <= 'z' ||
                code[pos] >= 'A' && code[pos] <= 'Z') ? true : false);
}

bool isNum(char c) {
    return ((code[pos] >= '0' && code[pos] <= '9') ? true : false);
}

bool isAlNum(char c) {
    return (isNum(c) || isAlpha(c));
}

void advanceChar() {
    if (code[pos] == '\0') {
        addToken(TokenType::tokEof, "eof");
        f = true;
    } else if (isSkippable(code[pos])) {
        c++;
    } else if (code[pos] == '\n') {
        c = 1;
        l++;
    } else if (code[pos] == '\r') {
        c = 1;
    } else if (isAlpha(code[pos])) {
        string content;
        while (isAlNum(code[pos])) {
            content += code[pos];
            pos++;
            c++;
        }
        TokenType type = TokenType::tokId;
        if (keywords.find(content) != keywords.end()) {
            type = keywords[content];
        }
        addToken(type, content);
        pos--;
    } else if (isNum(code[pos])) {
        string content;
        while (isNum(code[pos])) {
            content += code[pos];
            pos++;
            c++;
        }
        pos--;
        addToken(TokenType::tokNumber, content);
    } else if (code[pos] == '"') {
        string content;
        pos++;
        while (code[pos] != '"') {
            if (code[pos] == '\n' || code[pos] == '\r') {
                printf("Not terminated string at %d:%d.\n", l, c);
                return;
            }
            content += code[pos];
            c++;
            pos++;
        }
        pos--;
        addToken(TokenType::tokLiteral, content);
    } else {
        switch (code[pos]) {
            case '=':
                addToken(TokenType::tokEq, "=");
                break;
            case ';':
                addToken(TokenType::tokSC, ";");
                break;
            case '{':
                addToken(TokenType::tokOpenBrace, "{");
                break;
            case '}':
                addToken(TokenType::tokCloseBrace, "}");
                break;
            case '(':
                addToken(TokenType::tokOpenPar, "(");
                break;
            case ')':
                addToken(TokenType::tokClosePar, ")");
                break;
            default:
                printf("Invalid character at %d:%d.\n", l, c);
                return;
                break;
        }
        c++;
    }
}

void lexerLex() {
    while (!f) {
        advanceChar();
        pos++;
    }
}

vector<Token> lexerGetTokens() {
    return toks;
}

string lexerTok2Str(TokenType type) {
    return tok2str[type];
}