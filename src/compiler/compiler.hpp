#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../parser/parser.hpp"
#include "../lexer/lexer.hpp"
#include <string>
#include <vector>
#include <map>

using namespace std;

void compilerInit();
void compilerCompile(vector<Node> ast);
void compilerSave(string path);