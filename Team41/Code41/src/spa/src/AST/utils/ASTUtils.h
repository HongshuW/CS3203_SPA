//
// Created by Nafour on 9/9/22.
//

#ifndef SPA_ASTUTILS_H
#define SPA_ASTUTILS_H

#include "AST/ASTNode.h"
#include "AST/AssignNode.h"
#include "AST/CallNode.h"
#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "AST/ProgramNode.h"
#include "AST/ProcedureNode.h"

#include <unordered_map>
#include <unordered_set>
#include <map>
#include "query_builder/commons/DesignEntity.h"

using namespace QB;
namespace AST {
    //
// Created by Nafour on 9/9/22.
//

    enum NodeType {ASSIGN_NODE, CALL_NODE, IF_NODE, PRINT_NODE, READ_NODE, WHILE_NODE, CONDEXPR_NODE, EXPR_NODE, PROCEDURE_NODE, PROGRAM_NODE, VARIABLE_NODE};

    class ASTUtils {
        static int getNodeLineMappingHelper(shared_ptr<ASTNode> node, shared_ptr<unordered_map < shared_ptr<ASTNode>, int>> map, int curr_line_no, shared_ptr<unordered_map < int, shared_ptr<ASTNode>>> lineToNode);
        static int getNodePtrToLineNoMapHelper(shared_ptr<ASTNode> node, shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> map, int curr_line_no);
    public:
        static NodeType getNodeType(shared_ptr<ASTNode> node);
        static void getNodeLineMapping(shared_ptr<ProgramNode> root, shared_ptr<unordered_map<shared_ptr < ASTNode>, int>> nodeToLine, shared_ptr<unordered_map<int, shared_ptr < ASTNode>>> lineToNode);
        static shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> getNodePtrToLineNumMap(shared_ptr<ProgramNode> root);
        static DesignEntity getStmtNodeDesignEntity(shared_ptr<StmtNode> node);
        static shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> getLineNumToProcMap(shared_ptr<ProgramNode> root);
        static shared_ptr<map<string, shared_ptr<ProcedureNode>>> getProcNameToProcNodeMap(shared_ptr<ProgramNode> root);
    };

} // AST

#endif //SPA_ASTUTILS_H
