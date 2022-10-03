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
        if (!attrRef) return;
        auto declaration = Declaration::findDeclaration(attrRef->synonym, declarations);
        if (!declaration) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED +
                    attrRef->synonym.synonym);
        }
        unordered_set<AttrName> allowedAttrNameSet =
                getAllowedAttrNameSetFromDesignEntity(declaration->getDesignEntity());
        if (!allowedAttrNameSet.count(attrRef->attrName)) {
            throw PQLValidationException(QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED +
                attrRef->synonym.synonym);
        }
    }
}

void QueryValidator::validateSelectClause() {
    validateSynonymDeclaredSelectClause();
    //! Validate the correct match of design entity and AttrName in select clause
    validateDesignEntityAttrNamePairSelectClause();
}

void QueryValidator::checkSynonymDeclared(Synonym& synonym,
                                          const shared_ptr<vector<Declaration>>& declarations) {
    if (!Declaration::findDeclaration(synonym, declarations)) {
        throw PQLValidationException(
                QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                        synonym.synonym);
    }
}

void QueryValidator::validateSynonymDeclaredSuchThatClause() {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(const auto& suchThat : *suchThatClauses) {
        auto arg1 = get_if<Synonym>(&suchThat->arg1);
        auto arg2 = get_if<Synonym>(&suchThat->arg2);

        if (arg1) {
            checkSynonymDeclared(*arg1, declarations);
        }

        if (arg2) {
            checkSynonymDeclared(*arg2, declarations);
        }
    }
}

void QueryValidator::validateArgRefTypeSuchThatClause() const {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(const auto& suchThat : *suchThatClauses) {
        shared_ptr<Validatable> clause = dynamic_pointer_cast<Validatable>(suchThat);
        pair<RefTypeSet, RefTypeSet> validArgsTypes = clause->getAllowedArgsRefType();

        auto arg1RefIndex= suchThat->arg1.index();
        auto arg2RefIndex= suchThat->arg2.index();

        RefTypeSet validFirstArgTypes = validArgsTypes.first;
        RefTypeSet validSecondArgTypes = validArgsTypes.second;

        if (!validFirstArgTypes.count(arg1RefIndex) || !validSecondArgTypes.count(arg2RefIndex)) {
            throw PQLParseException(
                    QueryValidatorConstants::PQL_VALIDATION_INVALID_REF_TYPES);
        }
    }
}

void QueryValidator::validateUsesModifiesNoUnderscoreForFirstArg() const {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    for(const auto& suchThat : *suchThatClauses) {
        auto modifiesP = dynamic_pointer_cast<ModifiesPClause>(suchThat);
        auto modifiesS = dynamic_pointer_cast<ModifiesSClause>(suchThat);
        auto usesP = dynamic_pointer_cast<UsesPClause>(suchThat);
        auto usesS = dynamic_pointer_cast<UsesSClause>(suchThat);
        if (!modifiesP && !modifiesS && !usesP && !usesS) continue;
        if (get_if<Underscore>(&suchThat->arg1)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_FIRST_ARG_UNDERSCORE);
        }
    }
}

void QueryValidator::checkCorrectDesignEntity(Synonym synonym, const unordered_set<DesignEntity>& validSynonymTypes,
                                              const shared_ptr<vector<Declaration>>& declarations) {
    auto declaration = Declaration::findDeclaration(synonym, declarations);
    auto designEntity = declaration->getDesignEntity();
    if (!validSynonymTypes.count(designEntity)) {
        //! correct design entity not found, throw semantic error
        throw PQLValidationException(
                QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY +
                getDesignEntityString(designEntity));
    }
}

void QueryValidator::validateSynonymTypeSuchThatClause() {
    shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses = query->suchThatClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;

    for(const auto& suchThat : *suchThatClauses) {
        shared_ptr<Validatable> clause = dynamic_pointer_cast<Validatable>(suchThat);
        pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>> validArgsSynonymTypes =
                clause->getAllowedArgsSynonym();

        unordered_set<DesignEntity> validFirstArgSynonymTypes = validArgsSynonymTypes.first;
        unordered_set<DesignEntity> validSecondArgSynonymTypes = validArgsSynonymTypes.second;

        auto arg1 = get_if<Synonym>(&suchThat->arg1);
        auto arg2 = get_if<Synonym>(&suchThat->arg2);

        if (arg1) {
            checkCorrectDesignEntity(*arg1, validFirstArgSynonymTypes, declarations);
        }

        if (arg2) {
            checkCorrectDesignEntity(*arg2, validSecondArgSynonymTypes, declarations);
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
    for (const auto& patternClause : *patternClauses) {
        checkSynonymDeclared(patternClause->arg1, declarations);
        //! Validation for agr2 (if arg2 is a Synonym)
        auto arg2 = get_if<Synonym>(&patternClause->arg2);
        if (arg2) {
            checkSynonymDeclared(*arg2, declarations);
        }
    }
}

void QueryValidator::validateAllowedDesignEntityPatternClause() {
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    for (const auto& patternClause : *patternClauses) {
        if (dynamic_pointer_cast<InvalidPatternClause>(patternClause)) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_INVALID_SYNONYM_PATTERN);
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

void QueryValidator::validateArg2DesignEntityPatternClause() const {
    //! Validate if agr2 is a synonym, it must be declared as variable
    shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses = query->patternClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& patternClause : *patternClauses) {
        auto arg2 = get_if<Synonym>(&patternClause->arg2);
        if (!arg2) continue;
        auto declaration = Declaration::findDeclaration(*arg2, declarations);
        if (declaration && declaration->getDesignEntity() != DesignEntity::VARIABLE) {
            throw PQLValidationException(
                    QueryValidatorConstants::PQL_VALIDATION_INVALID_DESIGN_ENTITY_PATTERN +
                    getDesignEntityString(declaration->getDesignEntity()));
        }
    }
}

void QueryValidator::validatePatternClause() {
    //! Validate synonym for arg1 and arg2 are declared
    validateSynonymDeclaredPatternClause();
    //! Validate design entity for arg1 can only be assign, while and if
    validateAllowedDesignEntityPatternClause();
    //! Validate the correct RefType for pattern clause arg2
    //! Should throw syntax error not semantic error
    validateArgRefTypePatternClause();
    //! Validate if agr2 is a synonym, it must be declared as variable
    validateArg2DesignEntityPatternClause();
}

void QueryValidator::checkDesignEntityAttrNamePair(WithRef& withRef,
                                                   const shared_ptr<vector<Declaration>>& declarations) {
    //! Only need to check if it is a AttrRef
    auto attrRef = get_if<AttrRef>(&withRef);
    if (!attrRef) return;
    auto declaration = Declaration::findDeclaration(attrRef->synonym, declarations);
    if (!declaration) {
        throw PQLValidationException(
                QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
                        attrRef->synonym.synonym);
    }
    unordered_set<AttrName> allowedAttrNameSet =
            getAllowedAttrNameSetFromDesignEntity(declaration->getDesignEntity());
    if (!allowedAttrNameSet.count(attrRef->attrName)) {
        throw PQLValidationException(
                QueryValidatorConstants::PQL_VALIDATION_INVALID_ATTR_REF +
                getDesignEntityString(declaration->getDesignEntity()) +
                QueryValidatorConstants::DOT +
                AttrRef::getStrFromAttrName(attrRef->attrName));
    }
}

void QueryValidator::validateDesignEntityAttrNamePairWithClause() {
    shared_ptr<vector<shared_ptr<WithClause>>> withClauses = query->withClauses;
    shared_ptr<vector<Declaration>> declarations = query->declarations;
    for (const auto& withClause : *withClauses) {
        checkDesignEntityAttrNamePair(withClause->lhs, declarations);
        checkDesignEntityAttrNamePair(withClause->rhs, declarations);
    }
}

WithComparingType QueryValidator::getWithComparingType(WithRef withRef, const shared_ptr<WithClause>& withClause) {
    WithRefType withRefType = getWithRefTypeFromIndex((int) withRef.index());
    //! Default value
    WithComparingType withComparingType = WithComparingType::NAME;
    switch (withRefType) {
        case WithRefType::IDENT:
            withComparingType = WithComparingType::NAME;
            break;
        case WithRefType::INTEGER:
            withComparingType = WithComparingType::INTEGER;
            break;
        case WithRefType::ATTR_REF:
            auto attrRef = get_if<AttrRef>(&withRef);
            withComparingType = AttrRef::getWithComparingTypeFromAttrName(attrRef->attrName);
            break;
    }
    return withComparingType;
}

void QueryValidator::validateSameWithRefWithClause() {
    //! Two WithRef comparison must be of the same type, both NAME, or both INTEGER
    //! WithRef can be Ident, int or AttrRef
    shared_ptr<vector<shared_ptr<WithClause>>> withClauses = query->withClauses;
    for (const auto& withClause : *withClauses) {
        WithComparingType withComparingTypeLHS = getWithComparingType(withClause->lhs, withClause);
        WithComparingType withComparingTypeRHS = getWithComparingType(withClause->rhs, withClause);
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