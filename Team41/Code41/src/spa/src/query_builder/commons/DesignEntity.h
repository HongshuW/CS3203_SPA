//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_DESIGNENTITY_H
#define SPA_DESIGNENTITY_H

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "AttrRef.h"

using namespace std;

namespace QB {
enum class DesignEntity {
  STMT,
  READ,
  PRINT,
  CALL,
  WHILE,
  IF,
  ASSIGN,
  VARIABLE,
  CONSTANT,
  PROCEDURE,
};

const unordered_set<DesignEntity> STMT_SYNONYMS = {
    DesignEntity::STMT, DesignEntity::READ,  DesignEntity::ASSIGN,
    DesignEntity::CALL, DesignEntity::WHILE, DesignEntity::IF,
    DesignEntity::PRINT};

const unordered_set<DesignEntity> PROCEDURE_SYNONYM = {DesignEntity::PROCEDURE};

const unordered_set<DesignEntity> VARIABLE_SYNONYM = {DesignEntity::VARIABLE};

DesignEntity getDesignEntity(string&);
string getDesignEntityString(DesignEntity);
unordered_set<AttrName> getAllowedAttrNameSetFromDesignEntity(
    DesignEntity designEntity);

}  // namespace QB

#endif  // SPA_DESIGNENTITY_H