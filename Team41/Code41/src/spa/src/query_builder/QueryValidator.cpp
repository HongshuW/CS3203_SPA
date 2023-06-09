//
// Created by Xingchen Lin on 31/8/22.
//

#include "QueryValidator.h"

#include <utility>

#include "query_builder/commons/Ref.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

QueryValidator::QueryValidator(shared_ptr<QB::Query> query)
    : query(std::move(query)) {}

void QueryValidator::validateNoDuplicateDeclarations() const {
  //! Validate no duplicate synonym for declarations
  unordered_set<string> synonymSet;
  for (auto declaration : *(query->declarations)) {
    auto synonym = declaration.getSynonym().synonym;
    if (synonymSet.count(synonym)) {
      throw PQLValidationException(
          QueryValidatorConstants::
              PQL_VALIDATION_DUPLICATE_SYNONYM_DECLARATION +
          synonym);
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

    if (attrRef &&
        !Declaration::findDeclaration(attrRef->synonym, declarations)) {
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
  for (auto& elem : *elemList) {
    auto attrRef = get_if<AttrRef>(&elem);
    //! Only need to check if it is a AttrRef
    if (!attrRef) continue;
    auto declaration =
        Declaration::findDeclaration(attrRef->synonym, declarations);
    unordered_set<AttrName> allowedAttrNameSet =
        getAllowedAttrNameSetFromDesignEntity(declaration->getDesignEntity());
    if (!allowedAttrNameSet.count(attrRef->attrName)) {
      throw PQLValidationException(
          QueryValidatorConstants::PQL_VALIDATION_ATTR_REF_NOT_DECLARED +
          attrRef->synonym.synonym);
    }
  }
}

void QueryValidator::validateSelectClause() {
  validateSynonymDeclaredSelectClause();
  //! Validate the correct match of design entity and AttrName in select clause
  validateDesignEntityAttrNamePairSelectClause();
}

void QueryValidator::checkSynonymDeclared(
    Synonym& synonym, const shared_ptr<vector<Declaration>>& declarations) {
  if (!Declaration::findDeclaration(synonym, declarations)) {
    throw PQLValidationException(
        QueryValidatorConstants::PQL_VALIDATION_SYNONYM_NOT_DECLARED +
        synonym.synonym);
  }
}

void QueryValidator::validateSynonymDeclaredSuchThatClause() {
  shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses =
      query->suchThatClauses;
  shared_ptr<vector<Declaration>> declarations = query->declarations;

  for (const auto& suchThat : *suchThatClauses) {
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

void QueryValidator::validateUsesModifiesNoUnderscoreForFirstArg() const {
  shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses =
      query->suchThatClauses;
  for (const auto& suchThat : *suchThatClauses) {
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

void QueryValidator::checkCorrectDesignEntity(
    Synonym synonym, const unordered_set<DesignEntity>& validSynonymTypes,
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
  shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses =
      query->suchThatClauses;
  shared_ptr<vector<Declaration>> declarations = query->declarations;

  for (const auto& suchThat : *suchThatClauses) {
    shared_ptr<Validatable> clause =
        dynamic_pointer_cast<Validatable>(suchThat);
    pair<unordered_set<DesignEntity>, unordered_set<DesignEntity>>
        validArgsSynonymTypes = clause->getAllowedArgsSynonym();

    unordered_set<DesignEntity> validFirstArgSynonymTypes =
        validArgsSynonymTypes.first;
    unordered_set<DesignEntity> validSecondArgSynonymTypes =
        validArgsSynonymTypes.second;

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
  //! Validate the correct synonym types for such that clause
  validateSynonymTypeSuchThatClause();
}

void QueryValidator::validateSynonymDeclaredPatternClause() {
  shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses =
      query->patternClauses;
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
  shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses =
      query->patternClauses;
  shared_ptr<vector<Declaration>> declarations = query->declarations;
  const unordered_set<DesignEntity> ALLOWED_DE = {
      DesignEntity::ASSIGN, DesignEntity::IF, DesignEntity::WHILE};

  shared_ptr<vector<shared_ptr<PatternClause>>> newPatternClauseVector =
      make_shared<vector<shared_ptr<PatternClause>>>();

  for (const auto& patternClause : *patternClauses) {
    auto dummyPatternClause =
        dynamic_pointer_cast<DummyPatternClause>(patternClause);
    checkCorrectDesignEntity(dummyPatternClause->arg1, ALLOWED_DE,
                             declarations);
    //! If no error is throw, construct the correct pattern clause
    auto declaration =
        Declaration::findDeclaration(dummyPatternClause->arg1, declarations);
    auto designEntity = declaration->getDesignEntity();
    shared_ptr<PatternClause> newPatternClause = nullptr;

    if (designEntity == DesignEntity::ASSIGN) {
      //! arg4 cannot be an underscore
      if (dummyPatternClause->isArg4Underscore) {
        throw PQLValidationException(
            QueryValidatorConstants::PQL_VALIDATION_INVALID_PATTERN_SYNTAX);
      }
      //! arg3 is an underscore, the expressionSpecType must be ANY_MATCH
      if (dummyPatternClause->isArg3Underscore &&
          dummyPatternClause->arg3.expressionSpecType !=
              ExpressionSpecType::ANY_MATCH) {
        throw PQLValidationException(
            QueryValidatorConstants::PQL_VALIDATION_INVALID_PATTERN_SYNTAX);
      }
      //! Otherwise the expressionSpecType must be PARTIAL_MATCH or FULL_MATCH
      //! and exprNode cannot be a nullptr
      if (dummyPatternClause->arg3.expressionSpecType ==
              ExpressionSpecType::PARTIAL_MATCH ||
          dummyPatternClause->arg3.expressionSpecType ==
              ExpressionSpecType::FULL_MATCH) {
        if (dummyPatternClause->arg3.exprNode == nullptr) {
          throw PQLValidationException(
              QueryValidatorConstants::PQL_VALIDATION_INVALID_PATTERN_SYNTAX);
        }
      }
      newPatternClause = make_shared<AssignPatternClause>(
          dummyPatternClause->arg1, dummyPatternClause->arg2,
          dummyPatternClause->arg3);
    } else if (designEntity == DesignEntity::IF) {
      //! arg3 and arg4 must be an underscore
      if (!(dummyPatternClause->isArg3Underscore &&
            dummyPatternClause->isArg4Underscore)) {
        throw PQLValidationException(
            QueryValidatorConstants::PQL_VALIDATION_INVALID_PATTERN_SYNTAX);
      }
      newPatternClause = make_shared<IfPatternClause>(dummyPatternClause->arg1,
                                                      dummyPatternClause->arg2);
    } else if (designEntity == DesignEntity::WHILE) {
      //! arg3 must be an underscore and arg4 must not be an underscore
      if (!(dummyPatternClause->isArg3Underscore &&
            !dummyPatternClause->isArg4Underscore)) {
        throw PQLValidationException(
            QueryValidatorConstants::PQL_VALIDATION_INVALID_PATTERN_SYNTAX);
      }
      newPatternClause = make_shared<WhilePatternClause>(
          dummyPatternClause->arg1, dummyPatternClause->arg2);
    }
    newPatternClauseVector->push_back(newPatternClause);
  }
  query->patternClauses = newPatternClauseVector;
}

void QueryValidator::validateArg2DesignEntityPatternClause() const {
  //! Validate if agr2 is a synonym, it must be declared as variable
  shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses =
      query->patternClauses;
  shared_ptr<vector<Declaration>> declarations = query->declarations;
  for (const auto& patternClause : *patternClauses) {
    auto arg2 = get_if<Synonym>(&patternClause->arg2);
    if (!arg2) continue;
    auto declaration = Declaration::findDeclaration(*arg2, declarations);
    if (declaration &&
        declaration->getDesignEntity() != DesignEntity::VARIABLE) {
      throw PQLValidationException(
          QueryValidatorConstants::
              PQL_VALIDATION_INVALID_DESIGN_ENTITY_PATTERN +
          getDesignEntityString(declaration->getDesignEntity()));
    }
  }
}

void QueryValidator::validatePatternClause() {
  //! Validate synonym for arg1 and arg2 are declared
  validateSynonymDeclaredPatternClause();
  //! Validate design entity for arg1 can only be assign, while and if
  //! create different pattern clauses respectively
  validateAllowedDesignEntityPatternClause();
  //! Validate if agr2 is a synonym, it must be declared as variable
  validateArg2DesignEntityPatternClause();
}

void QueryValidator::checkDesignEntityAttrNamePair(
    WithRef& withRef, const shared_ptr<vector<Declaration>>& declarations) {
  //! Only need to check if it is a AttrRef
  auto attrRef = get_if<AttrRef>(&withRef);
  if (!attrRef) return;
  auto declaration =
      Declaration::findDeclaration(attrRef->synonym, declarations);
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

WithComparingType QueryValidator::getWithComparingType(
    WithRef withRef, const shared_ptr<WithClause>& withClause) {
  WithRefType withRefType = getWithRefTypeFromIndex((int)withRef.index());
  //! Default value
  WithComparingType withComparingType = WithComparingType::NAME;
  if (withRefType == WithRefType::IDENT)
    withComparingType = WithComparingType::NAME;
  if (withRefType == WithRefType::INTEGER)
    withComparingType = WithComparingType::INTEGER;
  if (withRefType == WithRefType::ATTR_REF) {
    auto attrRef = get_if<AttrRef>(&withRef);
    withComparingType =
        AttrRef::getWithComparingTypeFromAttrName(attrRef->attrName);
  }
  return withComparingType;
}

void QueryValidator::validateSameWithRefWithClause() {
  //! Two WithRef comparison must be of the same type, both NAME, or both
  //! INTEGER WithRef can be Ident, int or AttrRef
  shared_ptr<vector<shared_ptr<WithClause>>> withClauses = query->withClauses;
  for (const auto& withClause : *withClauses) {
    WithComparingType withComparingTypeLHS =
        getWithComparingType(withClause->lhs, withClause);
    WithComparingType withComparingTypeRHS =
        getWithComparingType(withClause->rhs, withClause);
    if (withComparingTypeLHS != withComparingTypeRHS) {
      throw PQLValidationException(
          QueryValidatorConstants::PQL_VALIDATION_DIFFERENT_WITH_REF);
    }
  }
}

void QueryValidator::validateWithClause() {
  //! Validate the correct match of design entity and AttrName in with clause
  validateDesignEntityAttrNamePairWithClause();
  //! Two WithRef comparison must be of the same type, both NAME, or both
  //! INTEGER
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