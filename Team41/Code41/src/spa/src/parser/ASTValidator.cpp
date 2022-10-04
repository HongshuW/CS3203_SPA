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
    return validateProcedureNames() && validateNodes() && validateCyclicDependencies();
}

bool ASTValidator::validateProcedureNames() {
    for (auto procedure : ast->procedureList) {
        string procedureName = procedure->procedureName;
        auto iterator = procedureNames.find(procedureName);
        if (iterator != procedureNames.end()) {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                    ParserConstants::SP_AST_EXCEPTION_DUPLICATE_PROCEDURE, procedureName);
            throw SPASTException(errorMessage);
        }

        procedureNames.insert(procedureName);
        procedureCalls.insert({procedureName, unordered_set<string>()});
    }
    return true;
}

bool ASTValidator::validateNodes() {
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

            if (nodeType == AST::IF_NODE) {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                for (auto childStmtNode: ifNode->ifStmtList) {
                    queue.push(childStmtNode);
                }
                for (auto childStmtNode: ifNode->elseStmtList) {
                    queue.push(childStmtNode);
                }
            }

            if (nodeType == AST::WHILE_NODE) {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                for (auto childStmtNode: whileNode->stmtList) {
                    queue.push(childStmtNode);
                }
            }

            if (nodeType == AST::CALL_NODE) {
                shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                validateCallNode(callNode, procedureName);
            }
        }
    }
    return true;
}

void ASTValidator::validateCallNode(shared_ptr<CallNode> callNode, string procedureName) {
    string calledProcedure = callNode->procedureName;

    if (calledProcedure == procedureName) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_AST_EXCEPTION_PROCEDURE_CALLS_ITSELF, procedureName);
        throw SPASTException(errorMessage);
    }

    auto iterator = procedureNames.find(procedureName);
    if (iterator == procedureNames.end()) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND, procedureName);
        throw SPASTException(errorMessage);
    }

    auto callsIterator = procedureCalls.find(procedureName);
    if (callsIterator == procedureCalls.end()) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND, procedureName);
        throw SPASTException(errorMessage);
    }

    unordered_set<string> calledProcedures = callsIterator->second;
    calledProcedures.insert(calledProcedure);
    callsIterator->second = calledProcedures;
}


bool ASTValidator::validateCyclicDependencies() {
    for (auto &unorderedMapIterator: procedureCalls) {
        string mainProcedure = unorderedMapIterator.first;
        unordered_set<string> proceduresCalled = unorderedMapIterator.second;
        for (auto &unorderedSetIterator : proceduresCalled) {
            string procedure = unorderedSetIterator;

            if (calls(procedure, mainProcedure)) {
                throw SPASTException(ParserConstants::SP_AST_EXCEPTION_CYCLIC_DEPENDENCY);
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
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND, procedure);
        throw SPASTException(errorMessage);
    }
    unordered_set<string> calledProcedures = callsIterator->second;
    for (auto &unorderedSetIterator : calledProcedures) {
        return calls(unorderedSetIterator, calledProcedure);
    }

    return false;
}
