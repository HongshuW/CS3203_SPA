//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"
#include <queue>

using namespace AST;
using namespace DE;

unordered_map<int, vector<int>> parentRelations;

DesignExtractor::DesignExtractor(shared_ptr<ASTNode> programNode)
        : programNode(programNode){
    programNode = programNode;
}

std::list<string> DesignExtractor::extractVariables() {
//    std::vector<std::shared_ptr<ProcedureNode>> procedureList = programNode.getProcedureList();
//    ProcedureNode procedureNode = *procedureList.front();
//    AssignNode assignNode = procedureNode.getAssignNode();
//    VariableNode variableNode = assignNode.getVariableNode();
//    list<string> variableList;
//    variableList.push_back(variableNode.getVariable());
//    return variableList;
}

void DesignExtractor::saveVariableToPKB() {
    DataModifier dataMod = DataModifier();
    list<string> varList = extractVariables();
    dataMod.saveVariables(varList);
}

unordered_map<int, vector<int>> DesignExtractor::extractParentRelations() {
    unordered_map<int, vector<int>> parentRelations;

    // extract Parent and Parent* relations
    ASTNode root = *programNode;
//    shared_ptr<unordered_map<shared_ptr<ASTNode>, int>> stmtNumbers = ASTUtils::getNodePtrToLineNumMap(programNode);
    queue<shared_ptr<ASTNode>> queue;
    queue.push(make_shared<ASTNode>(root));
    while (!queue.empty()) {
        shared_ptr<ASTNode> current = queue.front();
        queue.pop();

        NodeType nodeType = ASTUtils::getNodeTypeByName(current);
        if (nodeType == NodeType::WHILE_NODE) {
            shared_ptr<WhileNode> ptr = dynamic_pointer_cast<WhileNode>(current);
            vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
            for (shared_ptr<StmtNode> n: stmtList) {
                queue.push(n);
                // TODO: Find out statement numbers -> row{(*ptr).stmt_no, (*n).stmt_no}
                pair<int, int> row{};
                parentRelations.insert(row);
            }
        } else if (nodeType == NodeType::IF_NODE) {
            shared_ptr<IfNode> ptr = dynamic_pointer_cast<IfNode>(current);
            vector<shared_ptr<StmtNode>> ifStmtList = ptr->ifStmtList;
            vector<shared_ptr<StmtNode>> elseStmtList = ptr->elseStmtList;
            for (shared_ptr<StmtNode> n: ifStmtList) {
                queue.push(n);
                // TODO: Find out statement numbers -> row{(*ptr).stmt_no, (*n).stmt_no}
                pair<int, int> row{};
                parentRelations.insert(row);
            }
            for (shared_ptr<StmtNode> n: elseStmtList) {
                queue.push(n);
                // TODO: Find out statement numbers -> row{(*ptr).stmt_no, (*n).stmt_no}
                pair<int, int> row{};
                parentRelations.insert(row);
            }
        } else if (nodeType == NodeType::PROGRAM_NODE) {
            // encounter a program node, check its procedures
            shared_ptr<ProgramNode> ptr = dynamic_pointer_cast<ProgramNode>(current);
            vector<shared_ptr<ProcedureNode>> procedureList = ptr->procedureList;
            for (shared_ptr<ProcedureNode> n: procedureList) {
                queue.push(n);
            }
        } else if (nodeType == NodeType::PROCEDURE_NODE) {
            // encounter a procedure node, check its statements
            shared_ptr<ProcedureNode> ptr = dynamic_pointer_cast<ProcedureNode>(current);
            vector<shared_ptr<StmtNode>> stmtList = ptr->stmtList;
            for (shared_ptr<StmtNode> n: stmtList) {
                queue.push(n);
            }
        }
    }
}

void DesignExtractor::saveParentToPKB() {
    if (parentRelations.empty()) {
        parentRelations = extractParentRelations();
    }
}
