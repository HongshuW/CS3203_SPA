//
// Created by Xingchen Lin on 31/8/22.
//

#include "RelationType.h"
#include "query_builder/exceptions/Exceptions.h"
#include "Ref.h"

namespace QB {
    unordered_map<RelationType, string> relationTypeToStringMap({
        {RelationType::FOLLOWS, "Follows"},
        {RelationType::FOLLOWS_T, "Follows*"},
        {RelationType::PARENT, "Parent"},
        {RelationType::PARENT_T, "Parent*"},
        {RelationType::MODIFIES, "Modifies"},
        {RelationType::MODIFIES_S, "Modifies_S"},
        {RelationType::MODIFIES_P, "Modifies_P"},
        {RelationType::USES, "Uses"},
        {RelationType::USES_S, "Uses_S"},
        {RelationType::USES_P, "Uses_P"},
        {RelationType::CALLS, "Calls"},
        {RelationType::CALLS_T, "Calls*"},
        {RelationType::NEXT, "Next"},
        {RelationType::NEXT_T, "Next*"},
        {RelationType::AFFECTS, "Affects"},
        {RelationType::AFFECTS_T, "Affects*"}
    });

    unordered_map<string, RelationType> stringToRelationTypeMap({
        {"Follows", RelationType::FOLLOWS},
        {"Follows*", RelationType::FOLLOWS_T},
        {"Parent", RelationType::PARENT},
        {"Parent*", RelationType::PARENT_T},
        {"Modifies", RelationType::MODIFIES},
        {"Modifies_S", RelationType::MODIFIES_S},
        {"Modifies_P", RelationType::MODIFIES_P},
        {"Uses", RelationType::USES},
        {"Uses_S", RelationType::USES_S},
        {"Uses_P", RelationType::USES_P},
        {"Calls", RelationType::CALLS},
        {"Calls*", RelationType::CALLS_T},
        {"Next", RelationType::NEXT},
        {"Next*", RelationType::NEXT_T},
        {"Affects", RelationType::AFFECTS},
        {"Affects*", RelationType::AFFECTS_T}
    });

    unordered_map<RelationType, pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>>
            relationTypesToSuchThatArgsSynonymTypeMap({
                {RelationType::FOLLOWS, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::FOLLOWS_T, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::PARENT, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::PARENT_T, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::USES_S, make_pair(STMT_SYNONYMS, VARIABLE_SYNONYM)},
                {RelationType::USES_P, make_pair(PROCEDURE_SYNONYM, VARIABLE_SYNONYM)},
                {RelationType::MODIFIES_S, make_pair(STMT_SYNONYMS, VARIABLE_SYNONYM)},
                {RelationType::MODIFIES_P, make_pair(PROCEDURE_SYNONYM, VARIABLE_SYNONYM)},
                {RelationType::CALLS, make_pair(PROCEDURE_SYNONYM, PROCEDURE_SYNONYM)},
                {RelationType::CALLS_T, make_pair(PROCEDURE_SYNONYM, PROCEDURE_SYNONYM)},
                {RelationType::NEXT, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::NEXT_T, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::AFFECTS, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)},
                {RelationType::AFFECTS_T, make_pair(STMT_SYNONYMS, STMT_SYNONYMS)}
                });

    unordered_map<RelationType, pair<RefTypeSet, RefTypeSet>> relationTypesToSuchThatArgsRefTypeMap({
        {RelationType::FOLLOWS, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::FOLLOWS_T, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::PARENT, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::PARENT_T, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::USES_S, make_pair(stmtRefIndexSet, entRefIndexSet)},
        {RelationType::USES_P, make_pair(entRefIndexSet, entRefIndexSet)},
        {RelationType::MODIFIES_S, make_pair(stmtRefIndexSet, entRefIndexSet)},
        {RelationType::MODIFIES_P, make_pair(entRefIndexSet, entRefIndexSet)},
        {RelationType::CALLS, make_pair(entRefIndexSet, entRefIndexSet)},
        {RelationType::CALLS_T, make_pair(entRefIndexSet, entRefIndexSet)},
        {RelationType::NEXT, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::NEXT_T, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::AFFECTS, make_pair(stmtRefIndexSet, stmtRefIndexSet)},
        {RelationType::AFFECTS_T, make_pair(stmtRefIndexSet, stmtRefIndexSet)}
        });

    RelationType getRelationTypeFromStr(string& str) {
        try {
            return stringToRelationTypeMap.at(str);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException("Cannot find a RelationType called " +
                                    str);
        }
    }

    string getStrFromRelationType(RelationType relationType) {
        return relationTypeToStringMap.at(relationType);
    }

    pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> getSuchThatSynonymArgsTypeFromRelationType(RelationType relationType) {
        return relationTypesToSuchThatArgsSynonymTypeMap.at(relationType);
    }

    pair<RefTypeSet, RefTypeSet> getSuchThatArgsRefTypeFromRelationType(RelationType relationType) {
        return relationTypesToSuchThatArgsRefTypeMap.at(relationType);
    }

}