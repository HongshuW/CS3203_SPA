//
// Created by Xingchen Lin on 31/8/22.
//

#include "QueryValidator.h"
#include "Exceptions.h"
#include "query_builder/commons/Ref.h"

using namespace QB;

QueryValidator::QueryValidator(Query* query)
        : query(query) {};

void QueryValidator::validateNoDuplicateDeclarations() {
    // Validate no duplicate synonym for declarations
    unordered_set<string> synonymSet;
    for (auto declaration : *(query->declarations)) {
        auto synonym = declaration.getSynonym().synonym;
        if (synonymSet.count(synonym)) {
            throw PQLValidationException(
                    "Found duplicated synonym for declaration: " + synonym);
        } else {
            synonymSet.insert(synonym);
        }
    }
}
void QueryValidator::validateSynonymDeclaredSelectClause() {
    // Validate that synonym is declared for Select Clause
    SelectClause* selectClause = query->selectClause;
    vector<Declaration>* declarations = query->declarations;
    if (!Declaration::findDeclaration(selectClause->synonym, declarations)) {
        throw PQLValidationException(
                "Synonym: " + selectClause->synonym.synonym + " is not defined for Select Clause");
    }
}

void QueryValidator::validateSynonymDeclaredSuchThatClause() {
    // Validate that synonyms are declared for Such that Clause
    vector<SuchThatClause*>* suchThatClauses = query->suchThatClauses;
    vector<Declaration>* declarations = query->declarations;

    for(auto suchThat : *suchThatClauses) {
        auto arg1 = get_if<Synonym>(&suchThat->arg1);
        auto arg2 = get_if<Synonym>(&suchThat->arg2);

        if (arg1 && !Declaration::findDeclaration(*arg1, declarations)) {
            throw PQLValidationException(
                    "Synonym: " + arg1->synonym + " is not defined for Such That Clause");
        }

        if (arg2 && !Declaration::findDeclaration(*arg2, declarations)) {
            throw PQLValidationException(
                    "Synonym: " + arg2->synonym + " is not defined for Such That Clause");
        }
    }
}

void QueryValidator::validateArgRefTypeSuchThatClause() {
    vector<SuchThatClause*>* suchThatClauses = query->suchThatClauses;
    vector<Declaration>* declarations = query->declarations;

    for(auto suchThat : *suchThatClauses) {
        RelationType relationType = suchThat->relationType;
        int arg1RefIndex= suchThat->arg1.index();
        int arg2RefIndex= suchThat->arg2.index();

        // The first argument for Modifies and Uses cannot be _
        if (relationType == RelationType::MODIFIES || relationType == RelationType::USES) {
            // relationType == Modifies, Uses
            if (get_if<Underscore>(&suchThat->arg1)) {
                throw PQLValidationException(
                        "First argument cannot be underscore for Such That Clause");
            }

            // First arg can be either stmtRef or entRef
            if (!refIndexSet.count(arg1RefIndex)) {
                throw PQLValidationException(
                        "Argument 1 must be stmtRef for " +
                        refTypeToString(static_cast<RefType>(relationType)) + "Clause");
            }
            // Second arg must be entRef
            if (!entRefIndexSet.count(arg2RefIndex)) {
                throw PQLValidationException(
                        "Argument 2 must be stmtRef" +
                        refTypeToString(static_cast<RefType>(relationType)) + "Clause");
            }
        } else {
            // relationType == Parent, ParentT, Follows, FollowsT
            if (!stmtRefIndexSet.count(arg1RefIndex)) {
                throw PQLValidationException(
                        "Argument 1 must be stmtRef for " +
                        refTypeToString(static_cast<RefType>(relationType)) + "Clause");
            }

            if (!stmtRefIndexSet.count(arg2RefIndex)) {
                throw PQLValidationException(
                        "Argument 2 must be stmtRef" +
                        refTypeToString(static_cast<RefType>(relationType)) + "Clause");
            }
        }
    }
}

void QueryValidator::validateSuchThatClause() {
    // Validate relationType, arg1 and arg2
    validateSynonymDeclaredSuchThatClause();
    validateArgRefTypeSuchThatClause();
}

void QueryValidator::validateQuery() {
    // Validation for declaration
    validateNoDuplicateDeclarations();
    // Validation for select clause
    validateSynonymDeclaredSelectClause();
    // Validation for such that clause (only Follows, and FollowsT)
    validateSuchThatClause();
}