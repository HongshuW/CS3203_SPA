//
// Created by Xingchen Lin on 31/8/22.
//

#include "RelationType.h"
#include "query_builder/Exceptions.h"

namespace QB {
    unordered_map<RelationType, std::string> relationTypeToStringMap({
        {RelationType::FOLLOWS, "Follows"},
        {RelationType::FOLLOWS_T, "Follows*"},
        {RelationType::PARENT, "Parent"},
        {RelationType::PARENT_T, "Parent*"},
        {RelationType::MODIFIES, "Modifies"},
        {RelationType::MODIFIES_S, "Modifies_S"},
        {RelationType::MODIFIES_P, "Modifies_P"},
        {RelationType::USES, "Uses"},
        {RelationType::USES_S, "Uses_S"},
        {RelationType::USES_P, "Uses_P"}
    });

    unordered_map<std::string, RelationType> stringToRelationTypeMap({
        {"Follows", RelationType::FOLLOWS},
        {"Follows*", RelationType::FOLLOWS_T},
        {"Parent", RelationType::PARENT},
        {"Parent*", RelationType::PARENT_T},
        {"Modifies", RelationType::MODIFIES},
        {"Modifies_S", RelationType::MODIFIES_S},
        {"Modifies_P", RelationType::MODIFIES_P},
        {"Uses", RelationType::USES},
        {"Uses_S", RelationType::USES_S},
        {"Uses_P", RelationType::USES_P}
    });

    RelationType getRelationTypeFromStr(string& str) {
        try {
            return stringToRelationTypeMap.at(str);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException("Cannot find a RelationType called " +
                                    str);
        }
    }

    string getStrFromRelationType(RelationType relationType) {
        return relationTypeToStringMap.at(relationType);
    }
}