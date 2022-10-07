//
// Created by hongshu wang on 10/9/22.
//

#include "AST/utils/ASTUtils.h"
#include "RelationExtractor.h"
#include "FollowsExtractor.h"
#include <queue>
#include "utils/Utils.h"
#include "ParentExtractor.h"
#include "ModifiesExtractor.h"
#include "UsesExtractor.h"
#include "CallsExtractor.h"
namespace DE {
    list<vector<string>> RelationExtractor::extractRelation(
            shared_ptr<ProgramNode> rootPtr, RelationType relationType) {
        switch (relationType) {
            case RelationType::PARENT: {
                return *ParentExtractor::extractParent(rootPtr);
            }
            case RelationType::PARENT_T: {
                return *ParentExtractor::extractParentT(rootPtr);
            }
            case RelationType::FOLLOWS: {
                return *FollowsExtractor().extractFollows(rootPtr);
            }
            case RelationType::FOLLOWS_T: {
                return *FollowsExtractor().extractFollowsStar(rootPtr);
            }
            case RelationType::MODIFIES_S: {
                return *ModifiesExtractor::extractModifiesS(rootPtr);
            }
            case RelationType::MODIFIES_P: {
                return *ModifiesExtractor::extractModifiesP(rootPtr);
            }
            case RelationType::USES_S: {
                return *UsesExtractor::extractUsesS(rootPtr);
            }
            case RelationType::USES_P: {
                return *UsesExtractor::extractUsesP(rootPtr);
            }
            case RelationType::CALLS: {
                return *CallsExtractor::extractCalls(rootPtr);
            }
            case RelationType::CALLS_T: {
                return *CallsExtractor::extractCallsStar(rootPtr);
            }
            case RelationType::NEXT: {
                return *NextExtractor::extractNext(rootPtr);
            }
            default: {
                list<vector<string>> emptyList;
                return emptyList;
            }
        }
    }
}
