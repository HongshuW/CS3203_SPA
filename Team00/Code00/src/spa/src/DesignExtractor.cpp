//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"

DesignExtractor::DesignExtractor(ProgramNode programNode)
    : programNode(programNode){
    programNode = programNode;
}

std::list<string> DesignExtractor::extractVariables() {
    std::vector<std::shared_ptr<ProcedureNode>> procedureList = programNode.getProcedureList();
    ProcedureNode procedureNode = *procedureList.front();
    AssignNode assignNode = procedureNode.getAssignNode();
    VariableNode variableNode = assignNode.getVariableNode();
    list<string> variableList;
    variableList.push_back(variableNode.getVariable());
    return variableList;
}

void DesignExtractor::saveVariableToPKB() {
    DataModifier dataMod = DataModifier();
    dataMod.saveVariables();
}








