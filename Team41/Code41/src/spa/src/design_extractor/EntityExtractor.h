//
// Created by Nafour on 10/9/22.
//

#ifndef SPA_ENTITYEXTRACTOR_H
#define SPA_ENTITYEXTRACTOR_H
#include <unordered_set>
#include <string>
#include "AST/ASTNode.h"
#include "query_builder/commons/DesignEntity.h"
#include "AST/utils/ASTUtils.h"

using namespace AST;
using namespace std;
namespace DE {
    class EntityExtractor {
        static unordered_set<string> getVariablesFromExprString(string expr);
        static shared_ptr<unordered_set<string>> extractVariables(shared_ptr<StmtNode> stmtNode);
        static shared_ptr<unordered_set<string>> extractConstants(shared_ptr<StmtNode> stmtNode);
        static unordered_set<string> getConstantsFromExprString(string expr);
        static bool is_number(const std::string& s);
    public:
        static  unordered_set<string> getVariablesFromExprNode(shared_ptr<ExprNode> exprNode);
        static  unordered_set<string> getConstantsFromExprNode(shared_ptr<ExprNode> exprNode);
        static unordered_set<string> extractDesignEntity(shared_ptr<StmtNode> node, QB::DesignEntity designEntityType);
        static unordered_map<string, vector<shared_ptr<CallNode>>> extractCallNodesFromProcedures(shared_ptr<AST::ProgramNode> rootPtr);
    };
} // DE

#endif //SPA_ENTITYEXTRACTOR_H
