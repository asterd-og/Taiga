#include <stdio.h>
#include "lexer/lexer.hpp"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "parser/parser.hpp"
#include "compiler/compiler.hpp"

using namespace std;

string openFile(string fileName) {
    ifstream t(fileName);
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    string data = openFile(argv[1]);
    if (data[0] == -1) {
        printf("Unable to open file: %d.\n", argv[1]);
        return 1;
    }
    printf("%s\n", data.c_str());
    lexerInit(data);
    lexerLex();
    vector<Token> toks = lexerGetTokens();
    for (Token tok : toks) {
        printf("%s - %s\n", lexerTok2Str(tok.type).c_str(), tok.content.c_str());
    }
    parserInit(toks);
    parserParse();
    vector<Node> nodes = parserGetAst();
    compilerCompile(nodes);
    compilerSave(argv[2]);
}