//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"

using namespace AST;
using namespace DE;


std::list<string> DesignExtractor::extractVariables() {
    for (auto p: this->programNode->procedureList) {
        this->extractVariablesFromProcedure(p);
    }

}

void DesignExtractor::saveVariableToPKB() {
    list<string> varList = extractVariables();
    this->dataModifier.saveVariables(varList);
}

DesignExtractor::DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode) : dataModifier(this->dataModifier), programNode(this->programNode) {

}

unordered_set DesignExtractor::extractVariablesFromProcedure(shared_ptr<ProcedureNode> procedureNode) {

}


