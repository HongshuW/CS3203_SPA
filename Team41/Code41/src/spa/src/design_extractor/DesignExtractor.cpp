//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"
#include <queue>

using namespace AST;
using namespace DE;
using namespace std;

shared_ptr<unordered_set<string>> DesignExtractor::extractVariables() {
    shared_ptr<unordered_set<string>> result = make_shared<unordered_set<string>>();
    for (auto p: this->programNode->procedureList) {
        this->extractVariablesFromProcedure(p, result);

    }
    return result;

}

void DesignExtractor::saveVariableToPKB() {
    auto set = this->extractVariables();
    list<string> lst;
    auto it = lst.begin();
    for (const auto& elem: *set) {
        it = lst.insert(it, elem);
    }
    this->dataModifier.saveVariables(lst);
}

DesignExtractor::DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode) : dataModifier(dataModifier), programNode(programNode) {

}

void DesignExtractor::extractVariablesFromProcedure(shared_ptr<ProcedureNode> procedureNode,
                                                    shared_ptr<unordered_set<string>> set) {
    queue<shared_ptr<StmtNode>> queue;

    for (auto stmtNode: procedureNode->stmtList) {
        queue.push(stmtNode);
    }
    while (!queue.empty()) {
        auto stmtNode = queue.front();
        queue.pop();
        NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
        switch (nodeType) {
            case AST::PRINT_NODE: {
                shared_ptr<PrintNode> printNode = dynamic_pointer_cast<PrintNode>(stmtNode);
                set->insert(printNode->variableNode->variable);
                break;
            }
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(stmtNode);
                set->insert(assignNode->variableNode->variable);
                unordered_set<string> variables = getVariablesFromExprString(assignNode->expressionNode->expression);
                set->insert(variables.begin(), variables.end());
                break;
            }
            case AST::CALL_NODE: {
                shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                set->insert(callNode->variableNode->variable);
                break;
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                unordered_set<string> variables = getVariablesFromExprString(ifNode->condExpr->condExpr);
                set->insert(variables.begin(), variables.end());
                for (auto childStmtNode: ifNode->ifStmtList) {
                    queue.push(childStmtNode);
                }
                for (auto childStmtNode: ifNode->elseStmtList) {
                    queue.push(childStmtNode);
                }
                break;
            }
            case AST::READ_NODE:{
                shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(stmtNode);
                set->insert(readNode->variableNode->variable);
                break;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                unordered_set<string> variables = getVariablesFromExprString(whileNode->condExpr->condExpr);
                set->insert(variables.begin(), variables.end());
                for (auto childStmtNode: whileNode->stmtList) {
                    queue.push(childStmtNode);
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

unordered_set<string> DesignExtractor::getVariablesFromExprString(string expr) {
    unordered_set<string> ans;
    unordered_set<char> ignored = {'*', '/', '%', '+', '-', '(', ')', ' ', '=', '<', '>', '!', '|', '&'};
    string acc = "";
    for (int i = 0 ; i < expr.length(); i++) {
        if (ignored.find(expr[i]) == ignored.end()) { // char not in ignored
            acc += expr[i];
        } else {
            if (!acc.empty() && !is_number(acc)) {
                ans.insert(acc);
            }
            acc = "";
        }
    }
    if (!acc.empty() && !is_number(acc)) {
        ans.insert(acc);
    }
    return ans;
}

bool DesignExtractor::is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}


