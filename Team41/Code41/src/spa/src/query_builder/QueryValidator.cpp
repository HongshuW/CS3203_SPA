//
// Created by Xingchen Lin on 31/8/22.
//

#include "QueryValidator.h"

#include <utility>
#include "query_builder/exceptions/Exceptions.h"
#include "query_builder/commons/Ref.h"

using namespace QB;

QueryValidator::QueryValidator(shared_ptr<QB::Query> query) : query(std::move(query)){}

void QueryValidator::validateNoDuplicateDeclarations() const {
    //! Validate no duplicate synonym for declarations
    unordered_set<string> synonymSet;
    for (auto declaration : *(query->declarations)) {
        auto synonym = declaration.getSynonym().synonym;
        if (synonymSet.count(synonym)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_DUPLICATE_SYNONYM_DECLARATION + synonym);
        } else {
            synonymSet.insert(synonym);
        }
    }
}
void QueryValidator::validateSynonymDeclaredSelectClause() const {
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
                    QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                    synonym->synonym);
        }

        if (attrRef && !Declaration::findDeclaration(attrRef->synonym, declarations)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED +
                    attrRef->synonym.synonym);
        }
    }
}

void QueryValidator::validateDesignEntityAttrNamePairSelectClause() const {
    shared_ptr<SelectClause> selectClause = query->selectClause;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    shared_ptr<vector<Elem>> elemList = selectClause->returnResults;
    if (selectClause->isBoolean()) return;
    for (auto &elem : *elemList) {
        auto attrRef = get_if<AttrRef>(&elem);
        //! Only need to check if it is a AttrRef
        if (attrRef) {
            auto declaration = Declaration::findDeclaration(attrRef->synonym, declarations);
            if (!declaration) {
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED +
                        attrRef->synonym.synonym);
            }
            unordered_set<AttrName> allowedAttrNameSet =
                    getAllowedAttrNameSetFromDesignEntity(declaration->getDesignEntity());
            if (!allowedAttrNameSet.count(attrRef->attrName)) {
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED +
                        attrRef->synonym.synonym);
            }
        }
    }
}

void QueryValidator::validateSelectClause() {
    validateSynonymDeclaredSelectClause();
    //! Validate the correct match of design entity and AttrName in select clause
    validateDesignEntityAttrNamePairSelectClause();
}

void QueryValidator::validateSynonymDeclaredSuchThatClause() const {
    //! Validate that synonyms are declared for Such that Clause
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(const auto& suchThat : *suchThatClauses) {
        auto arg1 = get_if<Synonym>(&suchThat->arg1);
        auto arg2 = get_if<Synonym>(&suchThat->arg2);

        if (arg1 && !Declaration::findDeclaration(*arg1, declarations)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                    arg1->synonym);
        }

        if (arg2 && !Declaration::findDeclaration(*arg2, declarations)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                    arg2->synonym);
        }
    }
}

void QueryValidator::validateArgRefTypeSuchThatClause() const {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(const auto& suchThat : *suchThatClauses) {
        RelationType relationType = suchThat->relationType;
        pair<RefTypeSet, RefTypeSet> validArgsTypes = getSuchThatArgsRefTypeFromRelationType(relationType);

        auto arg1RefIndex= suchThat->arg1.index();
        auto arg2RefIndex= suchThat->arg2.index();

        RefTypeSet validFirstArgTypes = validArgsTypes.first;
        RefTypeSet validSecondArgTypes = validArgsTypes.second;

        if (!validFirstArgTypes.count(arg1RefIndex) || !validSecondArgTypes.count(arg2RefIndex)) {
            throw PQLParseException(
                    QueryValidatorConstants::PQL_VALIDATION_INVALID_REF_TYPES +
                            getStrFromRelationType(relationType));
        }
    }
}

void QueryValidator::validateUsesModifiesNoUnderscoreForFirstArg() const {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    for(const auto& suchThat : *suchThatClauses) {
        RelationType relationType = suchThat->relationType;
        if (relationType != RelationType::MODIFIES && relationType != RelationType::USES) {
            continue;
        }
        if (get_if<Underscore>(&suchThat->arg1)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_FIRST_ARG_UNDERSCORE);
        }
    }
}

void QueryValidator::validateSynonymTypeSuchThatClause() const {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(const auto& suchThat : *suchThatClauses) {
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
                        QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY +
                            getDesignEntityString(designEntity));
            }
        }

        if (arg2) {
            //! arg2 is a synonym, need to check for the correct DesignEntity lhsType
            auto declaration = Declaration::findDeclaration(*arg2, declarations);
            auto designEntity = declaration->getDesignEntity();
            if (!validSecondArgSynonymTypes.count(designEntity)) {
                //! correct design entity not found, throw semantic error
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY +
                        getDesignEntityString(designEntity));
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

void QueryValidator::validateSynonymDeclaredPatternClause() const {
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& patternClause : *patternClauses) {
        //! Validation for agr1
        if (!Declaration::findDeclaration(patternClause->arg1, declarations)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                            patternClause->arg1.synonym);
        }

        //! Validation for agr2 (if arg2 is a Synonym)
        auto arg2 = get_if<Synonym>(&patternClause->arg2);
        if (arg2 && !Declaration::findDeclaration(*arg2, declarations)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                        arg2->synonym);
        }
    }
}

void QueryValidator::validateArgRefTypePatternClause() const {
    //! arg2 for pattern clause must be an entRef, e.g. synonym, _ or ident
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& patternClause : *patternClauses) {
        auto arg2RefIndex = patternClause->arg2.index();
        if (!entRefIndexSet.count(arg2RefIndex)) {
            throw PQLParseException(
                    QueryValidatorConstants::PQL_VALIDATION_SECOND_ARG_INTEGER);
        }
    }
}

void QueryValidator::validateArg1DesignEntityPatternClause() const {
    //! Validate arg1 for pattern clause is declared as assign, if or while
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& patternClause : *patternClauses) {
        auto declaration = Declaration::findDeclaration(patternClause->arg1, declarations);
        if (declaration && !ALLOW_SYNONYM_PATTERN.count(declaration->getDesignEntity())) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_INVALID_SYNONYM_PATTERN +
                            getDesignEntityString(declaration->getDesignEntity()));
        }
    }
}

void QueryValidator::validateArg2DesignEntityPatternClause() const {
    //! Validate if agr2 is a synonym, it must be declared as variable
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& patternClause : *patternClauses) {
        auto arg2 = get_if<Synonym>(&patternClause->arg2);
        if (arg2) {
            auto declaration = Declaration::findDeclaration(*arg2, declarations);
            if (declaration && declaration->getDesignEntity() != DesignEntity::VARIABLE) {
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY_PATTERN +
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


void QueryValidator::validateDesignEntityAttrNamePairWithClause() const {
    shared_ptr<vector<shared_ptr<WithClause>>> withClauses = query->withClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& withClause : *withClauses) {
        WithRef withRefLHS = withClause->lhs;
        auto lhs = get_if<AttrRef>(&withRefLHS);
        //! Only need to check if it is a AttrRef
        if (lhs) {
            auto declaration = Declaration::findDeclaration(lhs->synonym, declarations);
            if (!declaration) {
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                                lhs->synonym.synonym);
            }
            unordered_set<AttrName> allowedAttrNameSet =
                    getAllowedAttrNameSetFromDesignEntity(declaration->getDesignEntity());
            if (!allowedAttrNameSet.count(lhs->attrName)) {
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_INVALID_ATTR_REF +
                                getDesignEntityString(declaration->getDesignEntity()) +
                                QueryValidatorConstants::DOT +
                                AttrRef::getStrFromAttrName(lhs->attrName));
            }
        }

        WithRef withRefRHS = withClause->rhs;
        auto rhs = get_if<AttrRef>(&withRefRHS);
        //! Only need to check if it is a AttrRef
        if (rhs) {
            auto declaration = Declaration::findDeclaration(rhs->synonym, declarations);
            if (!declaration) {
                throw PQLValidationException(
                        QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                                lhs->synonym.synonym);
            }
            unordered_set<AttrName> allowedAttrNameSet =
                    getAllowedAttrNameSetFromDesignEntity(declaration->getDesignEntity());
            if (!allowedAttrNameSet.count(rhs->attrName)) {
                throw PQLValidationException(QueryValidatorConstants::PQL_VALIDATION_INVALID_ATTR_NAME +
                    getDesignEntityString(declaration->getDesignEntity()) +
                    QueryValidatorConstants::DOT +
                    AttrRef::getStrFromAttrName(rhs->attrName));
            }
        }
    }
}

void QueryValidator::validateSameWithRefWithClause() const {
    //! Two WithRef comparison must be of the same type, both NAME, or both INTEGER
    //! WithRef can be Ident, int or AttrRef
    shared_ptr<vector<shared_ptr<WithClause>>> withClauses = query->withClauses;
    for (const auto& withClause : *withClauses) {
        WithRef withRefLHS = withClause->lhs;
        WithRefType withRefTypeLHS = getWithRefTypeFromIndex((int) withRefLHS.index());
        //! Default value
        WithComparingType withComparingTypeLHS = WithComparingType::NAME;
        if (withRefTypeLHS == WithRefType::IDENT) {
            withComparingTypeLHS = WithComparingType::NAME;
        } else if (withRefTypeLHS == WithRefType::INTEGER) {
            withComparingTypeLHS = WithComparingType::INTEGER;
        } else if (withRefTypeLHS == WithRefType::ATTR_REF) {
            auto lhs = get_if<AttrRef>(&withRefLHS);
            withComparingTypeLHS = AttrRef::getWithComparingTypeFromAttrName(lhs->attrName);
        }

        WithRef withRefRHS = withClause->rhs;
        WithRefType withRefTypeRHS = getWithRefTypeFromIndex((int) withRefRHS.index());
        //! Default value
        WithComparingType withComparingTypeRHS = WithComparingType::NAME;
        if (withRefTypeRHS == WithRefType::IDENT) {
            withComparingTypeRHS = WithComparingType::NAME;
        } else if (withRefTypeRHS == WithRefType::INTEGER) {
            withComparingTypeRHS = WithComparingType::INTEGER;
        } else if (withRefTypeRHS == WithRefType::ATTR_REF) {
            auto rhs = get_if<AttrRef>(&withRefRHS);
            withComparingTypeRHS = AttrRef::getWithComparingTypeFromAttrName(rhs->attrName);
        }

        if (withComparingTypeLHS != withComparingTypeRHS) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_DIFFERENT_WTIH_REF);
        }
    }
}


void QueryValidator::validateWithClause() {
    //! Validate the correct match of design entity and AttrName in with clause
    validateDesignEntityAttrNamePairWithClause();
    //! Two WithRef comparison must be of the same type, both NAME, or both INTEGER
    validateSameWithRefWithClause();
}

void QueryValidator::validateQuery() {
    //! Validation for declaration
    validateNoDuplicateDeclarations();
    //! Validation for select clause
    validateSelectClause();
    //! Validation for such that clause
    validateSuchThatClause();
    //! Validation for pattern clause
    validatePatternClause();
    //! Validation for with clause
    validateWithClause();
}