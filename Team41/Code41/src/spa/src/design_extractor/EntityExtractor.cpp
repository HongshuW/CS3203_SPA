//
// Created by Nafour on 10/9/22.
//

#include "EntityExtractor.h"


namespace DE {
    unordered_set<string>
    EntityExtractor::extractDesignEntity(shared_ptr<StmtNode> node, QB::DesignEntity designEntityType) {
        unordered_set<string> ans = unordered_set<string>();
        switch (designEntityType) {
            case QB::DesignEntity::VARIABLE: {
                return *extractVariables(node);
            }
            case QB::DesignEntity::CONSTANT: {
                return *extractConstants(node);
            }
            default:{
                break;
            }
        }
        return ans;
    }
    shared_ptr<unordered_set<string>> EntityExtractor::extractVariables(shared_ptr<StmtNode> stmtNode) {
        shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();

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
                break;
            }
            default: {
                break;
            }
        }
        return set;

    }

    shared_ptr<unordered_set<string>> EntityExtractor::extractConstants(shared_ptr<StmtNode> stmtNode) {
        shared_ptr<unordered_set<string>> set = make_shared<unordered_set<string>>();

        NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
        switch (nodeType) {
            case AST::ASSIGN_NODE: {
                shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(stmtNode);
                unordered_set<string> constants = getConstantsFromExprString(assignNode->expressionNode->expression);
                set->insert(constants.begin(), constants.end());
                break;
            }
            case AST::IF_NODE: {
                shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                unordered_set<string> constants = getConstantsFromExprString(ifNode->condExpr->condExpr);
                set->insert(constants.begin(), constants.end());
                break;
            }
            case AST::WHILE_NODE: {
                shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                unordered_set<string> constants = getConstantsFromExprString(whileNode->condExpr->condExpr);
                set->insert(constants.begin(), constants.end());
                break;
            }
            default: {
                break;
            }
        }
        return set;

    }

    unordered_set<string> EntityExtractor::getVariablesFromExprString(string expr) {
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

    unordered_set<string> EntityExtractor::getConstantsFromExprString(string expr) {
        unordered_set<string> ans;
        unordered_set<char> ignored = {'*', '/', '%', '+', '-', '(', ')', ' ', '=', '<', '>', '!', '|', '&'};
        string acc = "";
        for (int i = 0 ; i < expr.length(); i++) {
            if (ignored.find(expr[i]) == ignored.end()) { // char not in ignored
                acc += expr[i];
            } else {
                if (!acc.empty() && is_number(acc)) {
                    ans.insert(acc);
                }
                acc = "";
            }
        }
        if (!acc.empty() && is_number(acc)) {
            ans.insert(acc);
        }
        return ans;
    }

    bool EntityExtractor::is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }
} // DE