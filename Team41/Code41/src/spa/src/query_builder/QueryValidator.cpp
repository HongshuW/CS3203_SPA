//
// Created by Xingchen Lin on 31/8/22.
//

#include "QueryValidator.h"
#include "query_builder/exceptions/Exceptions.h"
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
    //! If select BOOLEAN, no need to check for this condition
    if (selectClause->isBoolean()) return;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (auto elem : *selectClause->returnResults) {
        auto synonym = get_if<Synonym>(&elem);
        auto attrRef = get_if<AttrRef>(&elem);

        if (synonym && !Declaration::findDeclaration(*synonym, declarations)) {
            throw PQLValidationException(
                    "Synonym: " + synonym->synonym + " is not defined for Select Clause");
        }

        if (attrRef && !Declaration::findDeclaration(attrRef->synonym, declarations)) {
            throw PQLValidationException(
                    "Synonym: " + attrRef->synonym.synonym + " is not defined for Select Clause");
        }
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
            throw PQLParseException(
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
            //! arg1 is a synonym, need to check for the correct DesignEntity lhsType
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
            //! arg2 is a synonym, need to check for the correct DesignEntity lhsType
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
    //! Should throw syntax error not semantic error
    validateArgRefTypeSuchThatClause();
    //! Validate the correct synonym types for such that clause
    validateSynonymTypeSuchThatClause();
}

void QueryValidator::validateSynonymDeclaredPatternClause() {
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (auto patternClause : *patternClauses) {
        //! Validation for agr1
        if (!Declaration::findDeclaration(patternClause->arg1, declarations)) {
            throw PQLValidationException(
                    "Synonym: " + patternClause->arg1.synonym + " is not defined for Pattern Clause first argument");
        }

        //! Validation for agr2 (if arg2 is a Synonym)
        auto arg2 = get_if<Synonym>(&patternClause->arg2);
        if (arg2 && !Declaration::findDeclaration(*arg2, declarations)) {
            throw PQLValidationException(
                    "Synonym: " + arg2->synonym + " is not defined for Pattern Clause second argument");
        }
    }
}

void QueryValidator::validateArgRefTypePatternClause() {
    //! arg2 for pattern clause must be an entRef, e.g. synonym, _ or ident
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (auto patternClause : *patternClauses) {
        int arg2RefIndex = patternClause->arg2.index();
        if (!entRefIndexSet.count(arg2RefIndex)) {
            throw PQLParseException(
                    "Second argument of Pattern Clause cannot be Integer");
        }
    }
}

void QueryValidator::validateArg1DesignEntityPatternClause() {
    //! Validate arg1 for pattern clause is declared as assign, if or while
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (auto patternClause : *patternClauses) {
        auto declaration = Declaration::findDeclaration(patternClause->arg1, declarations);
        if (declaration && ALLOW_SYNONYM_PATTERN.count(declaration->getDesignEntity())) {
            throw PQLValidationException(
                    "Expect pattern clause arg1 to be declared as assign, got " +
                    getDesignEntityString(declaration->getDesignEntity()));
        }
    }
}

void QueryValidator::validateArg2DesignEntityPatternClause() {
    //! Validate if agr2 is a synonym, it must be declared as variable
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (auto patternClause : *patternClauses) {
        auto arg2 = get_if<Synonym>(&patternClause->arg2);
        if (arg2) {
            auto declaration = Declaration::findDeclaration(*arg2, declarations);
            if (declaration && declaration->getDesignEntity() != DesignEntity::VARIABLE) {
                throw PQLValidationException(
                        "Expect pattern clause arg2 to be declared as variable, got " +
                        getDesignEntityString(declaration->getDesignEntity()));
            }
        }
    }
}

void QueryValidator::validatePatternClause() {
    //! Validate synonym for arg1 and arg2 are declared
    validateSynonymDeclaredPatternClause();
    //! Validate the correct RefType for pattern clause arg2
    //! Should throw syntax error not semantic error
    validateArgRefTypePatternClause();
    //! Validate arg1 for pattern clause is declared as assign, if or while
    // already check this in Query Parser
    // validateArg1DesignEntityPatternClause();
    //! Validate if agr2 is a synonym, it must be declared as variable
    validateArg2DesignEntityPatternClause();
}

void QueryValidator::validateSameWithRefWithClause() {
    //! Two WithRef comparison must be of the same lhsType
    shared_ptr<vector<shared_ptr<WithClause>>> withClauses = query->withClauses;
    for (auto withClause : *withClauses) {
        if (!withClause->isSameWithRefType()) {
            throw PQLValidationException(
                    "Two WithRef are different, no comparison can be made");
        }
    }
}

void QueryValidator::validateQuery() {
    //! Validation for declaration
    validateNoDuplicateDeclarations();
    //! Validation for select clause
    validateSynonymDeclaredSelectClause();
    //! Validation for such that clause
    validateSuchThatClause();
    //! Validation for pattern clause
    validatePatternClause();
    //! Validation for with clause
    validateSameWithRefWithClause();
}