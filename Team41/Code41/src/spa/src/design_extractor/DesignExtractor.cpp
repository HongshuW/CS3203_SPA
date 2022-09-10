//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"
#include <queue>
#include "EntityExtractor.h"
#include "RelationExtractor.h"

using namespace AST;
using namespace DE;
using namespace std;

shared_ptr<unordered_set<string>> DesignExtractor::extractEntities(DesignEntity designEntityType) {
    shared_ptr<unordered_set<string>> result = make_shared<unordered_set<string>>();
    for (auto p: this->programNode->procedureList) {
        this->extractEntitiesFromProcedure(p, result, designEntityType);

    }
    return result;
}

shared_ptr<list<vector<string>>> DesignExtractor::extractRelations(RelationType relationType) {
    list<vector<string>> relationList = RelationExtractor::extractRelation(this->programNode, relationType);
    return make_shared<list<vector<string>>>(relationList);
}

void DesignExtractor::saveVariableToPKB() {
    auto set = this->extractEntities(DesignEntity::PROCEDURE);
    list<string> lst;
    auto it = lst.begin();
    for (const auto& elem: *set) {
        it = lst.insert(it, elem);
    }
    this->dataModifier.saveVariables(lst);
}

void DesignExtractor::saveParentToPKB() {
    list<vector<string>> parentRelations = *this->extractRelations(RelationType::PARENT);
    auto parentIterator = parentRelations.begin();
    while (parentIterator != parentRelations.end()) {
        this->dataModifier.saveParent(*parentIterator);
        advance(parentIterator, 1);
    }
}

DesignExtractor::DesignExtractor(DataModifier, shared_ptr<ProgramNode> programNode) : dataModifier(dataModifier), programNode(programNode) {

}

void DesignExtractor::extractEntitiesFromProcedure(shared_ptr<ProcedureNode> procedureNode,
                                                   shared_ptr<unordered_set<string>> set,
                                                   DesignEntity designEntityType) {
    if (designEntityType == QB::DesignEntity::PROCEDURE) {
        set->insert(procedureNode->procedureName);
        return;
    }

    queue<shared_ptr<StmtNode>> queue;

    for (auto stmtNode: procedureNode->stmtList) {
        queue.push(stmtNode);
    }
    while (!queue.empty()) {
        auto stmtNode = queue.front();
        queue.pop();
        //extract entity from current node
        unordered_set<string> entities = EntityExtractor::extractDesignEntity(stmtNode, designEntityType);
        set->insert(entities.begin(), entities.end());

        //bfs for child nodes
        NodeType nodeType = ASTUtils::getNodeTypeByName(stmtNode);
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
