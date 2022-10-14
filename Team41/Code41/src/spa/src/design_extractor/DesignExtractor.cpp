//
// Created by Tan Wei En on 3/9/22.
//

#include "DesignExtractor.h"
#include "pkb/DataModifier.h"
#include "AST/utils/ASTUtils.h"
#include <queue>
#include "EntityExtractor.h"
#include "PatternExtractor.h"
#include "design_extractor/save_extractions/save_to_pkb/ExecuteSaveToPKB.h"
using namespace AST;
using namespace DE;
using namespace std;

DesignExtractor::DesignExtractor(shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode)
        : dataModifier(dataModifier), programNode(programNode) {
}

void DesignExtractor::run() {

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

    // save relations and patterns and entities
    auto saveDesigns = ExecuteSaveToPKB(this->programNode, this->dataModifier);
    saveDesigns.executeSave();
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