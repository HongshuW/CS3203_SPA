//
// Created by Tan Wei En on 2/9/22.
//
#ifndef SPA_PARSER_H
#define SPA_PARSER_H

#include <vector>
#include "AST/ProcedureNode.h"
#include "AST/AssignNode.h"
#include "AST/VariableNode.h"
#include "AST/ASTNode.h"
#include "AST/ProgramNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/CallNode.h"
#include "AST/WhileNode.h"
#include "AST/IfNode.h"
#include "utils/Utils.h"
#include "utils/ExprNodeParser.h"
#include "AST/RelExprNode.h"
#include <unordered_set>
#include "constants/ParserConstants.h"
#include "utils/ErrorMessageFormatter.h"
using namespace std;
using namespace AST;

namespace SourceParser {
    class Parser {
    private:
        int currIdx;
        vector<string> tokens;
        string peek();
        string pop();
        bool match(const string& s);
        bool expect(const string& s);

        // procedure
        shared_ptr<ProcedureNode> parseProcedureNode();

        // statements
        shared_ptr<AssignNode> parseAssignNode();
        shared_ptr<PrintNode> parsePrintNode();
        shared_ptr<ReadNode> parseReadNode();
        shared_ptr<CallNode> parseCallNode();
        shared_ptr<IfNode> parseIfNode();
        shared_ptr<WhileNode> parseWhileNode();

        //! if, read, assign, print, while, call
        shared_ptr<StmtNode> parseStatementNode();
        vector<shared_ptr<StmtNode>> parseStatementList();
        shared_ptr<VariableNode> parseVariableNode();

        shared_ptr<ExprNode> parseExprNode();
        shared_ptr<CondExprNode> parseCondExprNode(int startIdx);
        shared_ptr<RelExprNode> parseRelExprNode();
        shared_ptr<ExprNode> parseRelFactor(int priority);
        shared_ptr<ExprNode> parseToken(const string& curr);
        shared_ptr<ExprNode> makeExprNode(const string& curr, const shared_ptr<ExprNode>& lhs);
        int parseSymbol(const string& curr);

    public:
        explicit Parser(vector<string> tokens);
        shared_ptr<ProgramNode> parse();
    };
}

#endif //SPA_PARSER_H