//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"
#include <queue>
#include "EntityExtractor.h"
#include "RelationExtractor.h"
#include "PatternExtractor.h"
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

void DesignExtractor::saveEntityToPKB(DesignEntity designEntity) {
    auto set = this->extractEntities(designEntity);
    list<string> entityResultList;
    auto it = entityResultList.begin();
    for (const auto& elem: *set) {
        it = entityResultList.insert(it, elem);
    }

    switch (designEntity) {
        case QB::DesignEntity::VARIABLE: {
            this->dataModifier.saveVariables(entityResultList);
            break;
        }
        case QB::DesignEntity::CONSTANT: {
            this->dataModifier.saveConstants(entityResultList);
            break;
        }
        case QB::DesignEntity::PROCEDURE: {
            this->dataModifier.saveProcedures(entityResultList);
            break;
        }
        default: {
            break;
        }

    }


    this->dataModifier.saveVariables(entityResultList);
}

void DesignExtractor::saveRelationToPKB(RelationType relationType) {
    list<vector<string>> relations = *this->extractRelations(relationType);
    auto iterator = relations.begin();
    while (iterator != relations.end()) {
        switch (relationType) {
            case RelationType::PARENT: {
                this->dataModifier.saveParent(*iterator);
            }
            case RelationType::PARENT_T: {
                this->dataModifier.saveParentT(*iterator);
            }
            case RelationType::FOLLOWS: {
                this->dataModifier.saveFollows(*iterator);
            }
            case RelationType::FOLLOWS_T: {
                this->dataModifier.saveFollowsT(*iterator);
            }
            case RelationType::MODIFIES_S: {
                this->dataModifier.saveModifiesS(*iterator);
            }
            case RelationType::USES_S: {
                this->dataModifier.saveUsesS(*iterator);
            }
            default:
                break;
        }
        advance(iterator, 1);
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
            default: {
                break;
            }
        }
    }
}

void DesignExtractor::run() {
    auto entitiesToSave = vector<DesignEntity>{DesignEntity::VARIABLE, DesignEntity::CONSTANT, DesignEntity::PROCEDURE};
    for (auto designEntity: entitiesToSave) {
        this->saveEntityToPKB(designEntity);
    }

    //save statements
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> nodeToLine = ASTUtils::getNodePtrToLineNumMap(programNode);
    list<vector<string>> payload;
    auto it = payload.begin();
    for (std::pair<shared_ptr<StmtNode>, int> myPair : *nodeToLine)
    {
        shared_ptr<StmtNode> node = myPair.first;
        vector<string> stmtPair = {to_string(myPair.second), getDesignEntityString(ASTUtils::getStmtNodeDesignEntity(node))};
        it = payload.insert(it, stmtPair);
    }
    this->dataModifier.saveStatements(payload);



}

vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> DesignExtractor::extractPatterns() {
    return PatternExtractor::extractPattern(this->programNode);
}

void DesignExtractor::savePatternsToPKB() {
    auto resultList = this->extractPatterns();
    for (auto resultRow: resultList) {
        string lineNumStr = to_string(resultRow.first.first);
        string varName = resultRow.first.second;
        auto exprNode = resultRow.second->expressionNode;
        this->dataModifier.savePattern({lineNumStr, varName}, exprNode);
    }
}


