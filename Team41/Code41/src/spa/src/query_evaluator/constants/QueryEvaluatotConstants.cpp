//
// Created by Nafour on 12/10/22.
//

#include "QueryEvaluatotConstants.h"

namespace QE {
     const vector<DesignEntity> QueryEvaluatotConstants::entitiesToFilter = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
     const vector<AttrName> QueryEvaluatotConstants::attrsToFilter = {AttrName::VAR_NAME, AttrName::PROC_NAME};
} // QE