//
// Created by Xingchen Lin on 31/8/22.
//

#include "QueryValidator.h"
#include "Exceptions.h"
#include "query_builder/commons/Ref.h"

using namespace QB;

QueryValidator::QueryValidator(shared_ptr<QB::Query> query) : query(query){}

void QueryValidator::validateNoDuplicateDeclarations() {
    //! Validate no duplicate synonym for declarations
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
    //! Validate that synonym is declared for Select Clause
    shared_ptr<SelectClause> selectClause = query->selectClause;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    if (!Declaration::findDeclaration(selectClause->synonym, declarations)) {
        throw PQLValidationException(
                "Synonym: " + selectClause->synonym.synonym + " is not defined for Select Clause");
    }
}

void QueryValidator::validateSynonymDeclaredSuchThatClause() {
    //! Validate that synonyms are declared for Such that Clause
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

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
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(auto suchThat : *suchThatClauses) {
        RelationType relationType = suchThat->relationType;
        pair<RefTypeSet, RefTypeSet> validArgsTypes = getSuchThatArgsRefTypeFromRelationType(relationType);

        int arg1RefIndex= suchThat->arg1.index();
        int arg2RefIndex= suchThat->arg2.index();

        RefTypeSet validFirstArgTypes = validArgsTypes.first;
        RefTypeSet validSecondArgTypes = validArgsTypes.second;

        if (!validFirstArgTypes.count(arg1RefIndex) || !validSecondArgTypes.count(arg2RefIndex)) {
            throw PQLValidationException(
                    "Argument types of " +
                    getStrFromRelationType(relationType) +
                    " do not match the expected RefTypes");
        }
    }
}

void QueryValidator::validateUsesModifiesNoUnderscoreForFirstArg() {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    for(auto suchThat : *suchThatClauses) {
        RelationType relationType = suchThat->relationType;
        if (relationType != RelationType::MODIFIES && relationType != RelationType::USES) {
            continue;
        }
        if (get_if<Underscore>(&suchThat->arg1)) {
            throw PQLValidationException(
                    "First argument cannot be underscore for Uses/Modifies Clause as it leads to ambiguity");
        }
    }
}

void QueryValidator::validateSynonymTypeSuchThatClause() {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(auto suchThat : *suchThatClauses) {
        RelationType relationType = suchThat->relationType;
        pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> validArgsSynonymTypes =
                getSuchThatSynonymArgsTypeFromRelationType(relationType);

        unordered_set<DesignEntity> validFirstArgSynonymTypes = validArgsSynonymTypes.first;
        unordered_set<DesignEntity> validSecondArgSynonymTypes = validArgsSynonymTypes.second;

        auto arg1 = get_if<Synonym>(&suchThat->arg1);
        auto arg2 = get_if<Synonym>(&suchThat->arg2);

        if (arg1) {
            //! arg1 is a synonym, need to check for the correct DesignEntity type
            auto declaration = Declaration::findDeclaration(*arg1, declarations);
            auto designEntity = declaration->getDesignEntity();
            if (!validFirstArgSynonymTypes.count(designEntity)) {
                //! correct design entity not found, throw semantic error
                throw PQLValidationException(
                        "Current design entity arg 1: " +
                        getDesignEntityString(designEntity) +
                        ", does not match the allowed design entities: " +
                        getDesignEntitySetString(validFirstArgSynonymTypes) +
                        "for " +
                        getStrFromRelationType(relationType) +
                        " clause");
            }
        }

        if (arg2) {
            //! arg2 is a synonym, need to check for the correct DesignEntity type
            auto declaration = Declaration::findDeclaration(*arg2, declarations);
            auto designEntity = declaration->getDesignEntity();
            if (!validSecondArgSynonymTypes.count(designEntity)) {
                //! correct design entity not found, throw semantic error
                throw PQLValidationException(
                        "Current design entity for arg 2: " +
                        getDesignEntityString(designEntity) +
                        ", does not match the allowed design entities: " +
                        getDesignEntitySetString(validSecondArgSynonymTypes) +
                        "for " +
                        getStrFromRelationType(relationType) +
                        " clause");
            }
        }
    }
}

void QueryValidator::validateSuchThatClause() {
    //! Validate relationType, arg1 and arg2
    validateSynonymDeclaredSuchThatClause();
    //! First arg for Uses and Modifies cannot be Underscore
    validateUsesModifiesNoUnderscoreForFirstArg();
    //! Validate the correct RefType for such that clause
    //! e.g. Follows : (stmtRef, stmtRef), UsesS : (stmtRef, entRef)
    validateArgRefTypeSuchThatClause();
    //! Validate the correct synonym types for such that clause
    validateSynonymTypeSuchThatClause();
}

void QueryValidator::validateQuery() {
    //! Validation for declaration
    validateNoDuplicateDeclarations();
    //! Validation for select clause
    validateSynonymDeclaredSelectClause();
    //! Validation for such that clause (only Follows, and FollowsT)
    validateSuchThatClause();
}