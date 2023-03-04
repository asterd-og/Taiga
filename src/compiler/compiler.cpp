#include "compiler.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <string>

using namespace llvm;

LLVMContext context;
Module module("program", context);
IRBuilder<> builder(context);
Type* currentType;

map<string, Type*> str2type = {
    {"i8", builder.getInt8Ty()},
    {"i16", builder.getInt16Ty()},
    {"i32", builder.getInt32Ty()},
    {"i64", builder.getInt64Ty()},
    {"v0", builder.getVoidTy()},
};

map<string, AllocaInst*> globalVars;
map<string, map<string, AllocaInst*>> localVars;

string currentFunc;

void compilerInit() {
}

void compilerHandleFuncDef(vector<Node> ast, int p) {
    FunctionType* funcType = FunctionType::get(
        str2type[ast[p].leftTok.content],
        true);
    
    Function* func = Function::Create(funcType, 
        Function::ExternalLinkage, ast[p].midTok.content.c_str(),
        &module);
    
    BasicBlock* block = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(block);

    currentType = str2type[ast[p].leftTok.content];
    currentFunc = ast[p].midTok.content;

    compilerCompile(ast[p].innerNodes);
    
}

void compilerHandleVarDef(vector<Node> ast, int p) {
    Type* type = str2type[ast[p].leftTok.content];

    AllocaInst* var = builder.CreateAlloca(
        type, nullptr, ast[p].midTok.content.c_str()
    );

    builder.CreateStore(ConstantInt::get(type,
            atoi(ast[p].rightTok.content.c_str())),
        var);
    
    localVars[currentFunc][ast[p].midTok.content] = var;
    
    //printf("%s %s = %s\n", ast[p].leftTok.content.c_str(), ast[p].midTok.content.c_str(), ast[p].rightTok.content.c_str());
}

void compilerHandleReturn(vector<Node> ast, int p) {
    switch (ast[p].leftTok.type) {
        case TokenType::tokNumber:
            builder.CreateRet(ConstantInt::get(currentType, 
                    atoi(ast[p].leftTok.content.c_str())));
            return;
        case TokenType::tokId:
            AllocaInst* var = localVars[currentFunc][ast[p].leftTok.content.c_str()];
            if (var->getAllocatedType() != currentType) {
                // Handle this situation
                printf("Return type doesn't matches %d:%d!\n",
                    ast[p].line,
                    ast[p].col);
                exit(1);
            }
            builder.CreateRet(builder.CreateLoad(
                    var->getAllocatedType(),
                    var,
                    ast[p].leftTok.content.c_str()
                )
            );
            return;
    }
}

void compilerCompile(vector<Node> ast) {
    int p = 0;
    for (Node node : ast) {
        switch (node.type) {
            case NodeType::nodeFuncDef:
                compilerHandleFuncDef(ast, p);
                break;
            case NodeType::nodeVarDef:
                compilerHandleVarDef(ast, p);
                break;
            case NodeType::nodeReturn:
                compilerHandleReturn(ast, p);
                break;
        }
        p++;
    }
}

void compilerSave(string path) {
    string buffer;
    raw_string_ostream stream(buffer);
    module.print(stream, nullptr);
    std::ofstream out("out.ll");
    out << stream.str() << endl;

    printf("\n%s\n", stream.str().c_str());

    ostringstream objStream;
    objStream << path << ".o";
    string obj = objStream.str();

    ostringstream llcStream;
    llcStream << "llc -filetype=obj out.ll -o " << obj;
    string llc = llcStream.str();

    ostringstream clangStream;
    clangStream << "clang " << obj << " -o " << path;
    string clang = clangStream.str();

    system(llc.c_str());
    system(clang.c_str());

    remove("out.ll");
    remove(obj.c_str());
}