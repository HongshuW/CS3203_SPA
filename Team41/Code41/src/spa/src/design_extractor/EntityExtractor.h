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
        static bool is_number(const std::string& s);
    public:
        static shared_ptr<unordered_set<string>> extractVariables(shared_ptr<StmtNode> stmtNode);
        static shared_ptr<unordered_set<string>> extractConstants(shared_ptr<StmtNode> stmtNode);
        static vector<shared_ptr<StmtNode>> extractStmtNodes(shared_ptr<ProgramNode> rootPtr);

        static  unordered_set<string> getVariablesFromExprNode(shared_ptr<ExprNode> exprNode);
        static  unordered_set<string> getConstantsFromExprNode(shared_ptr<ExprNode> exprNode);
        static  unordered_set<string> getVariablesFromCondExprNode(shared_ptr<CondExprNode> condExprNode);
        static  unordered_set<string> getConstantsFromCondExprNode(shared_ptr<CondExprNode> condExprNode);
        static unordered_set<string> extractDesignEntity(shared_ptr<StmtNode> node, QB::DesignEntity designEntityType);
        static unordered_map<string, vector<shared_ptr<CallNode>>> extractCallNodesFromProcedures(shared_ptr<ProgramNode> rootPtr);
        static void extractVariablesFromCallNodesInProceduresToList (shared_ptr<CallNode> callNode,
            unordered_map<string, unordered_set<string>> mappedProceduresToVar,
            unordered_map<string, vector<shared_ptr<CallNode>>> mappedCallNodesToProcedures,
            unordered_set<string>& uniqueVarList);
        static vector<shared_ptr<StmtNode>> extractIfAndWhileNodesFromProcedures(shared_ptr<ProgramNode> rootPtr);
    };
} // DE

#endif //SPA_ENTITYEXTRACTOR_H
