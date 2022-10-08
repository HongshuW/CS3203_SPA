//
// Created by Xingchen Lin on 19/9/22.
//

#include "AttrRef.h"

#include <utility>

AttrRef::AttrRef(Synonym synonym, AttrName attrName) : synonym(std::move(synonym)), attrName(attrName) {}
namespace QB {
    const string PROC_NAME = "procName";
    const string VAR_NAME = "varName";
    const string VALUE = "value";
    const string STMT_NUMBER = "stmt#";
}

unordered_map<AttrName, string> attrNameToStringMap({
    {AttrName::PROC_NAME, PROC_NAME},
    {AttrName::VAR_NAME, VAR_NAME},
    {AttrName::VALUE, VALUE},
    {AttrName::STMT_NUMBER, STMT_NUMBER}
});

unordered_map<string, AttrName> stringToAttrNameMap({
    {PROC_NAME, AttrName::PROC_NAME},
    {VAR_NAME, AttrName::VAR_NAME},
    {VALUE, AttrName::VALUE},
    {STMT_NUMBER, AttrName::STMT_NUMBER}
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
        throw PQLParseException(CommonConstants::PQL_INVALID_ATTR_REF);
    }
}

string AttrRef::getStrOfAttrName() const {
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

string AttrRef::toString() const {
    return synonym.synonym + CommonConstants::DOT + getStrOfAttrName();
}

