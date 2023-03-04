#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include "../lexer/lexer.hpp"

using namespace std;

typedef enum {
    nodeFuncDef,
    nodeVarDef,
    nodeReturn,
} NodeType;

typedef struct Node {
    NodeType type;
    Token leftTok;
    Token midTok;
    Token rightTok;
    int line;
    int col;
    vector<Node> innerNodes;
} Node;

void parserInit(vector<Token> tokens);
void parserParse();
vector<Node> parserGetAst();