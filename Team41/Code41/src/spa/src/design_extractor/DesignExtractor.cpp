//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"

using namespace AST;
using namespace DE;
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


