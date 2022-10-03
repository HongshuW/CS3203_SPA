//
// Created by Tan Wei En on 3/10/22.
//

#include "ASTValidator.h"
#include "SPExceptions.h"
#include "queue"
#include "AST/utils/ASTUtils.h"

using namespace SourceParser;

ASTValidator::ASTValidator(shared_ptr<ProgramNode> ast): ast(ast) {}

bool ASTValidator::validateAST() {
    updateProcedureNames();
    validateCalls();
    cout << "procedures: " << endl;
    for (const auto& elem: procedureNames) {
        /* ... process elem ... */
        cout << elem << endl;
        auto iterator = procedureCalls.find(elem);
        cout << "calls:" << endl;
        auto calledProcedures = iterator->second;
        for (const auto& secondElem: calledProcedures) {
            cout << secondElem << endl;
        }
        cout << "end" << endl;
    }

    validateLoops();
    return false;
}

void ASTValidator::updateProcedureNames() {
    for (auto procedure : ast->procedureList) {
        string procedureName = procedure->procedureName;

        auto iterator = procedureNames.find(procedureName);

        if (iterator != procedureNames.end()) {
            throw new SPASTException("duplicate procedure");
        }

        procedureNames.insert(procedureName);
        procedureCalls.insert({procedureName, unordered_set<string>()});
    }
}

bool ASTValidator::validateCalls() {
    for (auto procedure: ast->procedureList) {
        string procedureName = procedure->procedureName;

        queue<shared_ptr<StmtNode>> queue;
        for (auto statement: procedure->stmtList) {
            queue.push(statement);
        }

        while (!queue.empty()) {
            auto stmtNode = queue.front();
            queue.pop();

            NodeType nodeType = ASTUtils::getNodeType(stmtNode);
            switch (nodeType) {
                case AST::IF_NODE: {
                    shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                    for (auto childStmtNode: ifNode->ifStmtList) {
                        queue.push(childStmtNode);
                    }
                    for (auto childStmtNode: ifNode->elseStmtList) {
                        queue.push(childStmtNode);
                    }
                    break;
                }
                case AST::WHILE_NODE: {
                    shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                    for (auto childStmtNode: whileNode->stmtList) {
                        queue.push(childStmtNode);
                    }
                    break;
                }
                case AST::CALL_NODE: {
                    cout << "call node!" << endl;
                    shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                    string calledProcedure = callNode->procedureName;

                    // check if procedure calls itself
                    if (calledProcedure == procedureName) {
//                        throw new SPASTException("procedure calls itself");
                    }

                    // check if procedure called exists
                    auto iterator = procedureNames.find(procedureName);
                    if (iterator == procedureNames.end()) {
//                        throw new SPASTException("called procedure does not exist");
                    }

                    // update unordered_set
                    auto callsIterator = procedureCalls.find(procedureName);
                    if (callsIterator == procedureCalls.end()) {
//                        throw new SPASTException("procedure does not exist");
                    }
                    unordered_set<string> calledProcedures = callsIterator->second;
                    calledProcedures.insert(calledProcedure);
                    callsIterator->second = calledProcedures;

                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
}

// check for cyclical dependencies
bool ASTValidator::validateLoops() {
    for (auto &unorderedMapIterator: procedureCalls) {
        string calledProcedure = unorderedMapIterator.first;
        unordered_set<string> calledProcedures = unorderedMapIterator.second;
        for (auto &unorderedSetIterator : calledProcedures) {
            string procedure = unorderedSetIterator;

            if (calls(procedure, calledProcedure)) {
                throw new SPASTException("cycling dependency found");
            }
        }
    }
    return true;
}

bool ASTValidator::calls(string procedure, string calledProcedure) {
    if (procedure == calledProcedure) {
        return true;
    }

    auto callsIterator = procedureCalls.find(procedure);
    if (callsIterator == procedureCalls.end()) {
//        throw new SPASTException("procedure does not exist");
    }
    unordered_set<string> calledProcedures = callsIterator->second;
    for (auto &unorderedSetIterator : calledProcedures) {
        return calls(unorderedSetIterator, calledProcedure);
    }

    return false;
}
