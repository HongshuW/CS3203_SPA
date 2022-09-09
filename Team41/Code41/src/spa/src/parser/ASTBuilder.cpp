//
// Created by Xingchen Lin on 8/9/22.
//

#include "ASTBuilder.h"
#include "AST/ProgramNode.h"
#include "parser/Parser.h"

using namespace SourceParser;

shared_ptr<ASTNode> ASTBuilder::buildAST(std::string filename) {
    std::string str;
    std::ifstream MyReadFile(filename);
    std::string input;
    while (getline (MyReadFile, str)) {
        // Output the text from the file
        input.append(str);
    }
    MyReadFile.close();
    Tokenizer tokenizer = Tokenizer(input);
    vector<string> tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> rootPtr = parser.parse();
    return rootPtr;
}
