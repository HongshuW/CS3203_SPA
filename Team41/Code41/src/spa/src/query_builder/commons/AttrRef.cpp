//
// Created by Xingchen Lin on 19/9/22.
//

#include "AttrRef.h"

AttrRef::AttrRef(Synonym synonym, AttrName attrName) : synonym(synonym), attrName(attrName){};

unordered_map<AttrName, string> attrNameToStringMap({
    {AttrName::PROC_NAME, "procName"},
    {AttrName::VAR_NAME, "varName"},
    {AttrName::VALUE, "value"},
    {AttrName::STMT_NUMBER, "stmt#"}
});

unordered_map<string, AttrName> stringToAttrNameMap({
    {"procName", AttrName::PROC_NAME},
    {"varName", AttrName::VAR_NAME},
    {"value", AttrName::VALUE},
    {"stmt#", AttrName::STMT_NUMBER}
});

unordered_map<AttrName, WithComparingType> attrNameToWithComparingType({
    {AttrName::PROC_NAME, WithComparingType::NAME},
    {AttrName::VAR_NAME, WithComparingType::NAME},
    {AttrName::VALUE, WithComparingType::INTEGER},
    {AttrName::STMT_NUMBER, WithComparingType::INTEGER}
});

AttrName AttrRef::getAttrNameFromStr(string& str) {
    try {
        return stringToAttrNameMap.at(str);
    } catch (const std::out_of_range& oor) {
        throw PQLParseException("Cannot find the AttrRef called " + str);
    }
}

string AttrRef::getStrOfAttrName() {
    return attrNameToStringMap.at(this->attrName);
}

string AttrRef::getStrFromAttrName(AttrName& attrName) {
    return attrNameToStringMap.at(attrName);
}

WithComparingType AttrRef::getWithComparingTypeFromAttrName(AttrName& attrName) {
    return attrNameToWithComparingType.at(attrName);
}

bool AttrRef::operator==(const AttrRef& attrRef1) const {
    return synonym == attrRef1.synonym && attrName == attrRef1.attrName;
}

string AttrRef::toString() {
    const string CONCAT_SYMBOL = ".";
    return synonym.synonym + CONCAT_SYMBOL + getStrOfAttrName();
}

