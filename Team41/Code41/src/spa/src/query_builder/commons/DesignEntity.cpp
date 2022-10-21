//
// Created by Xingchen Lin on 28/8/22.
//

#include "DesignEntity.h"

#include "query_builder/exceptions/Exceptions.h"

namespace QB {
const string STMT = "stmt";
const string READ = "read";
const string PRINT = "print";
const string CALL = "call";
const string WHILE = "while";
const string IF = "if";
const string ASSIGN = "assign";
const string VARIABLE = "variable";
const string CONSTANT = "constant";
const string PROCEDURE = "procedure";

unordered_map<DesignEntity, std::string> designEntityToStringMap(
    {{DesignEntity::STMT, STMT},
     {DesignEntity::READ, READ},
     {DesignEntity::PRINT, PRINT},
     {DesignEntity::CALL, CALL},
     {DesignEntity::WHILE, WHILE},
     {DesignEntity::IF, IF},
     {DesignEntity::ASSIGN, ASSIGN},
     {DesignEntity::VARIABLE, VARIABLE},
     {DesignEntity::CONSTANT, CONSTANT},
     {DesignEntity::PROCEDURE, PROCEDURE}});

unordered_map<std::string, DesignEntity> stringToDesignEntityMap(
    {{STMT, DesignEntity::STMT},
     {READ, DesignEntity::READ},
     {PRINT, DesignEntity::PRINT},
     {CALL, DesignEntity::CALL},
     {WHILE, DesignEntity::WHILE},
     {IF, DesignEntity::IF},
     {ASSIGN, DesignEntity::ASSIGN},
     {VARIABLE, DesignEntity::VARIABLE},
     {CONSTANT, DesignEntity::CONSTANT},
     {PROCEDURE, DesignEntity::PROCEDURE}});

unordered_map<DesignEntity, unordered_set<AttrName>> designEntityToAttrNameMap(
    {{DesignEntity::STMT, {AttrName::STMT_NUMBER}},
     {DesignEntity::READ, {AttrName::STMT_NUMBER, AttrName::VAR_NAME}},
     {DesignEntity::PRINT, {AttrName::STMT_NUMBER, AttrName::VAR_NAME}},
     {DesignEntity::CALL, {AttrName::STMT_NUMBER, AttrName::PROC_NAME}},
     {DesignEntity::WHILE, {AttrName::STMT_NUMBER}},
     {DesignEntity::IF, {AttrName::STMT_NUMBER}},
     {DesignEntity::ASSIGN, {AttrName::STMT_NUMBER}},
     {DesignEntity::VARIABLE, {AttrName::VAR_NAME}},
     {DesignEntity::CONSTANT, {AttrName::VALUE}},
     {DesignEntity::PROCEDURE, {AttrName::PROC_NAME}}});

DesignEntity getDesignEntity(std::string& str) {
  try {
    return stringToDesignEntityMap.at(str);
  } catch (const std::out_of_range& oor) {
    throw PQLParseException(CommonConstants::PQL_INVALID_DESIGN_ENTITY);
  }
}

string getDesignEntityString(DesignEntity designEntity) {
  return designEntityToStringMap.at(designEntity);
}

unordered_set<AttrName> getAllowedAttrNameSetFromDesignEntity(
    DesignEntity designEntity) {
  return designEntityToAttrNameMap.at(designEntity);
}
}  // namespace QB