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
#include "design_extractor/save_extracted_design/ExecuteSaveExtractedDesign.h"
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
            this->dataModifier->saveVariables(entityResultList);
            break;
        }
        case QB::DesignEntity::CONSTANT: {
            this->dataModifier->saveConstants(entityResultList);
            break;
        }
        case QB::DesignEntity::PROCEDURE: {
            this->dataModifier->saveProcedures(entityResultList);
            break;
        }
        default: {
            break;
        }
    }
}

void DesignExtractor::saveRelationToPKB(RelationType relationType) {
    list<vector<string>> relations = *this->extractRelations(relationType);
    auto iterator = relations.begin();
    while (iterator != relations.end()) {
        switch (relationType) {
            case RelationType::PARENT: {
                this->dataModifier->saveParent(*iterator);
                break;
            }
            case RelationType::PARENT_T: {
                this->dataModifier->saveParentT(*iterator);
                break;
            }
            case RelationType::FOLLOWS: {
                this->dataModifier->saveFollows(*iterator);
                break;
            }
            case RelationType::FOLLOWS_T: {
                this->dataModifier->saveFollowsT(*iterator);
                break;
            }
            case RelationType::MODIFIES_S: {
                this->dataModifier->saveModifiesS(*iterator);
                break;
            }
            case RelationType::USES_S: {
                this->dataModifier->saveUsesS(*iterator);
                break;
            }
            case RelationType::USES_P: {
                this->dataModifier->saveUsesP(*iterator);
                break;
            }
            case RelationType::MODIFIES_P: {
                this->dataModifier->saveModifiesP(*iterator);
                break;
            }
            case RelationType::CALLS: {
                this->dataModifier->saveCalls(*iterator);
                break;
            }
            case RelationType::CALLS_T: {
                this->dataModifier->saveCallsT(*iterator);
                break;
            }
            case RelationType::NEXT: {
                this->dataModifier->saveNext(*iterator);
                break;
            }
            default:
                break;
        }
        advance(iterator, 1);
    }
}

DesignExtractor::DesignExtractor(shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
        : dataModifier(dataModifier), programNode(programNode) {
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
    //save entities
    auto entitiesToSave = vector<DesignEntity>{DesignEntity::VARIABLE, DesignEntity::CONSTANT, DesignEntity::PROCEDURE};
    for (auto designEntity: entitiesToSave) {
        this->saveEntityToPKB(designEntity);
    }

    //save statement lines and types
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> nodeToLine = ASTUtils::getNodePtrToLineNumMap(programNode);
    list<vector<string>> payload;
    auto it = payload.begin();
    for (std::pair<shared_ptr<StmtNode>, int> myPair : *nodeToLine)
    {
        shared_ptr<StmtNode> node = myPair.first;
        vector<string> stmtPair = {to_string(myPair.second), getDesignEntityString(ASTUtils::getStmtNodeDesignEntity(node))};
        it = payload.insert(it, stmtPair);
    }
    this->dataModifier->saveStatements(payload);

    auto saveRelations = ExecuteSaveExtractedDesign(this->programNode, this->dataModifier);
    saveRelations.executeSave();

    //save patterns
    this->savePatternsToPKB();

    //save if and while patterns
    this->saveConditionalPatternsToPKB();
}

vector<pair<pair<int, string>, std::shared_ptr<AssignNode>>> DesignExtractor::extractPatterns() {
    return PatternExtractor::extractPattern(this->programNode);
}

list<vector<string>> DesignExtractor::extractIfPatterns() {
    return PatternExtractor::extractIfPattern(this->programNode);
}

list<vector<string>> DesignExtractor::extractWhilePatterns() {
    return PatternExtractor::extractWhilePattern(this->programNode);
}

void DesignExtractor::savePatternsToPKB() {
    // TODO: generalise this method
    auto resultList = this->extractPatterns();
    for (auto resultRow: resultList) {
        string lineNumStr = to_string(resultRow.first.first);
        string varName = resultRow.first.second;
        auto exprNode = resultRow.second->expressionNode;
        this->dataModifier->saveAssignPattern({lineNumStr, varName}, exprNode);
    }
}

void DesignExtractor::saveConditionalPatternsToPKB() {
    auto ifPatternList = this->extractIfPatterns();
    for (auto entry: ifPatternList) {
        dataModifier->saveIfPattern(entry);
    }

    auto whilePatternList = this->extractWhilePatterns();
    for (auto entry: whilePatternList) {
        dataModifier->saveWhilePattern(entry);
    }
}

vector<string> DE::DesignExtractor::getNextStarRelations(StmtNoArgs args) {
    return NextExtractor::extractNextStar(this->programNode, args);
}

vector<string> DE::DesignExtractor::getAffectsRelations(StmtNoArgs args)
{
    return AffectsExtractor::extractAffects(this->programNode, args);
}

vector<string> DE::DesignExtractor::getAffectsStarRelations(StmtNoArgs args)
{
    return AffectsExtractor::extractAffectsStar(this->programNode, args);
}

list<vector<string>> DE::DesignExtractor::getAllNextStarRelations() {
    return NextExtractor::extractAllNextStarInProgram(this->programNode);
}

list<vector<string>> DE::DesignExtractor::getAllAffectsRelations() {
    return AffectsExtractor::extractAllAffectsInProgram(this->programNode);
}

list<vector<string>> DE::DesignExtractor::getAllAffectsStarRelations()
{
    return AffectsExtractor::extractAllAffectsStarInProgram(this->programNode);
}