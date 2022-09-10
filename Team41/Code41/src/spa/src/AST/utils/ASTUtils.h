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

namespace AST {
    //
// Created by Nafour on 9/9/22.
//

    enum NodeType {ASSIGN_NODE, CALL_NODE, IF_NODE, PRINT_NODE, READ_NODE, WHILE_NODE, CONDEXPR_NODE, EXPR_NODE, PROCEDURE_NODE, PROGRAM_NODE, VARIABLE_NODE};

    class ASTUtils {
        static int getNodePtrToLineNoMapHelper(shared_ptr<ASTNode> node, shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> map, int curr_line_no);
    public:
        static NodeType getNodeTypeByName(shared_ptr<ASTNode> node);
        static shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> getNodePtrToLineNumMap(shared_ptr<ProgramNode> root);
    };

} // AST

#endif //SPA_ASTUTILS_H
