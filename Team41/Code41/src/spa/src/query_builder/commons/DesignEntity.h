//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_DESIGNENTITY_H
#define SPA_DESIGNENTITY_H

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
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
            DesignEntity::STMT, DesignEntity::READ,
            DesignEntity::ASSIGN, DesignEntity::CALL,
            DesignEntity::WHILE, DesignEntity::IF,
            DesignEntity::PRINT
    };

    const unordered_set<DesignEntity> PROCEDURE_SYNONYM = {
            DesignEntity::PROCEDURE
    };

    const unordered_set<DesignEntity> VARIABLE_SYNONYM = {
            DesignEntity::VARIABLE
    };

    const unordered_set<DesignEntity> ALLOW_SYNONYM_PATTERN = {
            DesignEntity::ASSIGN, DesignEntity::IF,
            DesignEntity::WHILE
    };

    const unordered_set<DesignEntity> CONSTANT_SYNONYM = {
            DesignEntity::CONSTANT
    };

    const unordered_set<DesignEntity> SYNONYMS_WITH_NAME = {
            DesignEntity::PROCEDURE, DesignEntity::CALL,
            DesignEntity::VARIABLE, DesignEntity::READ,
            DesignEntity::PRINT
    };


    DesignEntity getDesignEntity(string&);
    string getDesignEntityString(DesignEntity);
    string getDesignEntitySetString(unordered_set<DesignEntity> set);
    unordered_set<AttrName> getAllowedAttrNameSetFromDesignEntity(DesignEntity designEntity);

}  // namespace QE

#endif //SPA_DESIGNENTITY_H