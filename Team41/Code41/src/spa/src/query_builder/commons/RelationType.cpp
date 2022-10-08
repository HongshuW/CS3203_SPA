//
// Created by Xingchen Lin on 31/8/22.
//

#include "RelationType.h"
#include "query_builder/exceptions/Exceptions.h"
#include "Ref.h"

namespace QB {
    const string FOLLOWS = "Follows";
    const string FOLLOWS_T = "Follows*";
    const string PARENT = "Parent";
    const string PARENT_T = "Parent*";
    const string MODIFIES = "Modifies";
    const string MODIFIES_S = "Modifies_S";
    const string MODIFIES_P = "Modifies_P";
    const string USES = "Uses";
    const string USES_S = "Uses_S";
    const string USES_P = "Uses_P";
    const string CALLS = "Calls";
    const string CALLS_T = "Calls*";
    const string NEXT = "Next";
    const string NEXT_T = "Next*";
    const string AFFECTS = "Affects";
    const string AFFECTS_T = "Affects*";

    unordered_map<string, RelationType> stringToRelationTypeMap({
        {FOLLOWS, RelationType::FOLLOWS},
        {FOLLOWS_T, RelationType::FOLLOWS_T},
        {PARENT, RelationType::PARENT},
        {PARENT_T, RelationType::PARENT_T},
        {MODIFIES, RelationType::MODIFIES},
        {MODIFIES_S, RelationType::MODIFIES_S},
        {MODIFIES_P, RelationType::MODIFIES_P},
        {USES, RelationType::USES},
        {USES_S, RelationType::USES_S},
        {USES_P, RelationType::USES_P},
        {CALLS, RelationType::CALLS},
        {CALLS_T, RelationType::CALLS_T},
        {NEXT, RelationType::NEXT},
        {NEXT_T, RelationType::NEXT_T},
        {AFFECTS, RelationType::AFFECTS},
        {AFFECTS_T, RelationType::AFFECTS_T}
    });

    RelationType getRelationTypeFromStr(string& str) {
        try {
            return stringToRelationTypeMap.at(str);
        } catch (const std::out_of_range& oor) {
            throw PQLParseException(CommonConstants::PQL_INVALID_RELATION_TYPE);
        }
    }
}