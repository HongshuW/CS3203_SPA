//
// Created by Tan Wei En on 2/9/22.
//

#include <vector>
#include "AST/ProcedureNode.h"
#include "AST/AssignNode.h"
#include "AST/VariableNode.h"
#include "AST/ASTNode.h"
#include "AST/ProgramNode.h"

#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#endif //SPA_PARSER_H

using namespace std;

namespace SourceParser {
    class Parser {
    private:
        vector<std::string> tokens;
        unsigned int currIdx;
        string peek();
        string previous();
        string pop();
        bool match(string s);
        bool expect(string s);

        shared_ptr<ProcedureNode> parseProcedure();
        shared_ptr<VariableNode> parseVariable();
        shared_ptr<NumberNode> parseNumber();
        shared_ptr<AssignNode> parseAssign();

    public:
        explicit Parser(std::vector<std::string> tokens);
        shared_ptr<ProgramNode> parse();
    };
}
