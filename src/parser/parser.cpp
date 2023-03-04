#include "parser.hpp"
#include <stdio.h>

vector<Node> ast;
vector<Token> tokens;
int idx = 0;
int line = 0;
int col = 0;
bool finished = false;

Node parseExpression();

void parserInit(vector<Token> toks) {
    tokens = toks;
}

void advance() {
    idx++;
}

void expect(TokenType type) {
    advance();
    if (tokens[idx].type != type) {
        printf("Expected token %s but found %s.\n",
               lexerTok2Str(type).c_str(),
               lexerTok2Str(tokens[idx].type).c_str());
    }
}

void eat(TokenType type) {
    if (tokens[idx].type != type) {
        printf("Expected token %s but found %s.\n",
               lexerTok2Str(type).c_str(),
               lexerTok2Str(tokens[idx].type).c_str());
    }
    advance();
}

vector<Node> parseBlock() {
    vector<Node> ret;
    int open = 1;
    eat(TokenType::tokOpenBrace);
    while (open >= 1) {
        switch (tokens[idx].type) {
            case TokenType::tokEof:
                printf("Invalid EOF at %d:%d.\n",
                        tokens[idx].line, tokens[idx].col);
                return {};
                break;
            case TokenType::tokOpenBrace:
                open++;
                break;
            case TokenType::tokCloseBrace:
                open--;
                break;
            default:
                ret.push_back(parseExpression());
                break;
        }
        advance();
    }
    return ret;
}

Node parseFuncDef(Token type, Token name) {
    eat(TokenType::tokClosePar);
    // For now we skip ), we won't
    // Be parsing arguments FOR NOW.
    return {
        .type = NodeType::nodeFuncDef,
        .leftTok = type,
        .midTok = name,
        .innerNodes = parseBlock(),
        .line = line,
        .col = col
    };
}

Node parseVarDef(Token type, Token name) {
    Token value = tokens[idx];
    expect(TokenType::tokSC);
    return {
        .type = NodeType::nodeVarDef,
        .leftTok = type,
        .midTok = name,
        .rightTok = value,
        .line = line,
        .col = col
    };
}

Node parseReturn(Token value) {
    expect(TokenType::tokSC);
    return {
        .type = NodeType::nodeReturn,
        .leftTok = value,
        .line = line,
        .col = col
    };
}

Node parseDecl(Token type, Token name) {
    switch (tokens[idx].type) {
        case TokenType::tokOpenPar:
            advance();
            return parseFuncDef(type, name);
            break;
        case TokenType::tokEq:
            advance();
            return parseVarDef(type, name);
            break;
        default:
            printf("Invalid token at %d:%d.\n", line, col);
            return {};
            break;
    }
}

Node parseStatement(Token left) {
    switch (tokens[idx].type) {
        case TokenType::tokId:
            advance();
            return parseDecl(left, tokens[idx-1]);
            break;
        default:
            printf("Invalid token at %d:%d.\n", line, col);
            return {};
            break;
    }
}

Node parseExpression() {
    line = tokens[idx].line;
    col = tokens[idx].col;
    switch (tokens[idx].type) {
        case TokenType::tokType:
            advance();
            return parseStatement(tokens[idx-1]);
            break;
        case TokenType::tokReturn:
            advance();
            return parseReturn(tokens[idx]);
            break;
        case TokenType::tokEof:
            break;
        default:
            printf("Invalid token at %d:%d.\n", line, col);
            return {};
            break;
    }
}

void parserParse() {
    ast.push_back(parseExpression());
}

vector<Node> parserGetAst() { return ast; }