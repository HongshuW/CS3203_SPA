//
// Created by Xingchen Lin on 28/8/22.
//

#include "DesignEntity.h"
#include "query_builder/exceptions/Exceptions.h"

namespace QB {
    unordered_map<DesignEntity, std::string> designEntityToStringMap({
        {DesignEntity::STMT, "stmt"},
        {DesignEntity::READ, "read"},
        {DesignEntity::PRINT, "print"},
        {DesignEntity::CALL, "call"},
        {DesignEntity::WHILE, "while"},
        {DesignEntity::IF, "if"},
        {DesignEntity::ASSIGN, "assign"},
        {DesignEntity::VARIABLE, "variable"},
        {DesignEntity::CONSTANT, "constant"},
        {DesignEntity::PROCEDURE, "procedure"}
    });

    unordered_map<std::string, DesignEntity> stringToDesignEntityMap({
        {"stmt", DesignEntity::STMT},
        {"read", DesignEntity::READ},
        {"print", DesignEntity::PRINT},
        {"call", DesignEntity::CALL},
        {"while", DesignEntity::WHILE},
        {"if", DesignEntity::IF},
        {"assign", DesignEntity::ASSIGN},
        {"variable", DesignEntity::VARIABLE},
        {"constant", DesignEntity::CONSTANT},
        {"procedure", DesignEntity::PROCEDURE}
    });

    DesignEntity getDesignEntity(std::string& str) {
        try {
            return stringToDesignEntityMap.at(str);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException("Cannot find a Design Entity called " +
                                            str);
        }
    }

    string getDesignEntityString(DesignEntity designEntity) {
        return designEntityToStringMap.at(designEntity);
    }

    string getDesignEntitySetString(unordered_set<DesignEntity> set) {
        string str = "";
        for (auto designEntity : set) {
            str += getDesignEntityString(designEntity);
            str += ", ";
        }
        return str;
    }
}