//
// Created by Tan Wei En on 2/9/22.
//

#include <vector>
#include "AST/ProcedureNode.h"
#include "AST/AssignNode.h"
#include "AST/VariableNode.h"
#include "AST/ASTNode.h"
#include "AST/ProgramNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/CallNode.h"
#include "AST/IfNode.h"


#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#endif //SPA_PARSER_H

using namespace std;
using namespace AST;

namespace SourceParser {
    class Parser {
    private:
        vector<string> tokens;
        unsigned int currIdx;
        string peek();
        string previous();
        string pop();
        bool match(string s);
        bool expect(string s);

        shared_ptr<ProcedureNode> parseProcedure();

        // statements
        shared_ptr<AssignNode> parseAssign();
        shared_ptr<PrintNode> parsePrint();
        shared_ptr<ReadNode> parseRead();
        shared_ptr<CallNode> parseCall();
        shared_ptr<IfNode> parseIf();

        // helpers

        shared_ptr<VariableNode> parseVariable();

    public:
        explicit Parser(std::vector<std::string> tokens);
        shared_ptr<ProgramNode> parseProgram();
    };
}
