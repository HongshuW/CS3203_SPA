//
// Created by Xingchen Lin on 28/9/22.
//

#include "ClauseVisitor.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/WithClauses.h"
#include "TableCombiner.h"
#include "constants/ClauseVisitorConstants.h"
#include <numeric>
#include "constants/ClauseVisitorConstants.h"

QE::ClauseReturnType QE::ClauseVisitor::setReturnTable() {
    returnType = TABLE;
    return returnType;
};
QE::ClauseReturnType QE::ClauseVisitor::setReturnBool() {
    returnType = BOOL;
    return returnType;
}

QE::ClauseVisitor::ClauseVisitor(shared_ptr<DataPreprocessor> dataPreprocessor, shared_ptr<DataRetriever> dataRetriever,
                                 Declarations declarations)
        : dataPreprocessor(dataPreprocessor), declarations(declarations), dataRetriever(dataRetriever) {

}

//Table QE::ClauseVisitor::operator()(shared_ptr<PatternClause> patternClause) {
//    return dataPreprocessor->getTableByPattern(patternClause);
//}

Table QE::ClauseVisitor::operator()(shared_ptr<SuchThatClause> suchThatClause) {
    return dataPreprocessor->getTableByRelation(*suchThatClause);
}


Table QE::ClauseVisitor::operator()(shared_ptr<WithClause> withClause) {
    if (this->returnType == TABLE) return dataPreprocessor->getTableByWith(withClause, declarations);

    Value valLHS;
    Value valRHS;

    vector<WithRef> withRefs = {withClause->lhs, withClause->rhs};
    shared_ptr<vector<Value>> values = make_shared<vector<Value>>();
    values->push_back(valLHS);
    values->push_back(valRHS);

    //get values from each ref and store in val1 and val2
    getWithValues(withRefs, values);
    valLHS = values->at(0);
    valRHS = values->at(1);

    //compare two values, assumption made: two values are of the same lhsType, i.e either int or string
    switch (valLHS.index()) {
        case ClauseVisitorConstants::STR_VAL_IDX: {
            string lhsStr = get<ClauseVisitorConstants::STR_VAL_IDX>(valLHS);
            if (valRHS.index() == ClauseVisitorConstants::STR_VAL_IDX) return
            lhsStr == get<ClauseVisitorConstants::STR_VAL_IDX>(valRHS)
                       ? ClauseVisitorConstants::TRUE_TABLE
                       : ClauseVisitorConstants::FALSE_TABLE;

            if (valRHS.index() == ClauseVisitorConstants::VECTOR_STR_VAL_IDX) {
                vector<string> strs = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valRHS);
                return count(strs.begin(), strs.end(), lhsStr)
                       ? ClauseVisitorConstants::TRUE_TABLE
                       : ClauseVisitorConstants::FALSE_TABLE;
            }
            break;
        }
        case ClauseVisitorConstants::INT_VAL_IDX: {
            int lhsInt = get<ClauseVisitorConstants::INT_VAL_IDX>(valLHS);
            if (valRHS.index() == ClauseVisitorConstants::INT_VAL_IDX)
                return lhsInt == get<ClauseVisitorConstants::INT_VAL_IDX>(valRHS) ? ClauseVisitorConstants::TRUE_TABLE
                                                          : ClauseVisitorConstants::FALSE_TABLE;
            if (valRHS.index() == ClauseVisitorConstants::VECTOR_INT_VAL_IDX) {
                vector<int> ints = get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valRHS);
                return count(ints.begin(), ints.end(), lhsInt) ? ClauseVisitorConstants::TRUE_TABLE
                                                               : ClauseVisitorConstants::FALSE_TABLE;
            }
            break;
        }
        case ClauseVisitorConstants::VECTOR_STR_VAL_IDX: {
            vector<string> lhsVecStr = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valLHS);
            if (valRHS.index() == ClauseVisitorConstants::STR_VAL_IDX) {
                string rhsStr = get<ClauseVisitorConstants::STR_VAL_IDX>(valRHS);
                return count(lhsVecStr.begin(), lhsVecStr.end(), rhsStr) ? ClauseVisitorConstants::TRUE_TABLE
                                                                         : ClauseVisitorConstants::FALSE_TABLE;
            }
            if (valRHS.index() == ClauseVisitorConstants::VECTOR_STR_VAL_IDX) {
                vector<string> rhsVecStr = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valRHS);
                return !intersection(lhsVecStr, rhsVecStr).empty() ? ClauseVisitorConstants::TRUE_TABLE
                                                                   : ClauseVisitorConstants::FALSE_TABLE;
            }
            break;
        }
        case ClauseVisitorConstants::VECTOR_INT_VAL_IDX: {
            vector<int> lhsVecInt = get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valLHS);
            if (valRHS.index() == ClauseVisitorConstants::INT_VAL_IDX) {
                int rhsInt = get<ClauseVisitorConstants::INT_VAL_IDX>(valRHS);
                return count(lhsVecInt.begin(), lhsVecInt.end(), rhsInt) ? ClauseVisitorConstants::TRUE_TABLE
                                                                         : ClauseVisitorConstants::FALSE_TABLE;
            }
            if (valRHS.index() == ClauseVisitorConstants::VECTOR_INT_VAL_IDX) {
                vector<int> rhsVecInt = get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valRHS);
                return !intersection(lhsVecInt, rhsVecInt).empty() ? ClauseVisitorConstants::TRUE_TABLE
                                                                   : ClauseVisitorConstants::FALSE_TABLE;
            }
            break;
        }
    }

    return ClauseVisitorConstants::FALSE_TABLE;

}

void QE::ClauseVisitor::getWithValues(vector<WithRef> withRefs, shared_ptr<vector<Value>> values) {

    for (int i = 0; i < withRefs.size(); i++) {
        if (withRefs[i].index() == WithClause::WITHREF_IDENT_IDX) {
            values->at(i) = get<WithClause::WITHREF_IDENT_IDX>(withRefs[i]).identStr;
            continue;
        }
        if (withRefs[i].index() == WithClause::WITHREF_INT_IDX) {
            values->at(i) = get<WithClause::WITHREF_INT_IDX>(withRefs[i]);
            continue;
        }
        //withRefs[i].index() == WithClause::WITHREF_ATTR_REF_IDX
        AttrRef attrRef = std::get<AttrRef>(withRefs[i]);
        DesignEntity designEntity = getDesignEntity(attrRef.synonym);
        vector<DesignEntity> notDirectlyAvailDEs = {DesignEntity::CALL, DesignEntity::READ,
                                                    DesignEntity::PRINT};
        vector<AttrName> notDirectlyAvailAttrs = {AttrName::PROC_NAME, AttrName::VAR_NAME};
        bool processingNeeded = std::count(notDirectlyAvailDEs.begin(), notDirectlyAvailDEs.end(), designEntity)
                                && std::count(notDirectlyAvailAttrs.begin(), notDirectlyAvailAttrs.end(),
                                              attrRef.attrName);
        if (!processingNeeded) {
            if (attrRef.attrName == AttrName::PROC_NAME) {
                values->at(i) = dataPreprocessor->getEntityNames(designEntity);
                continue;
            }

            if (attrRef.attrName == AttrName::VAR_NAME) {
                values->at(i) = dataPreprocessor->getEntityNames(designEntity);//design entity must be variable
                continue;
            }

            if (attrRef.attrName == AttrName::STMT_NUMBER) {
                values->at(i) = dataPreprocessor->getStmtNumsByDesignEntity(designEntity); //vector<int>
                continue;
            }
            //attrRef.attrName == AttrName::VALUE
            values->at(i) = dataPreprocessor->getEntityNames(
                    designEntity);//this can only be constant values with return type vector<string>
            vector<int> constVals;
            vector<string> strVals = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(values->at(i));
            for (auto strVal: strVals) {
                constVals.push_back(stoi(strVal));
            }
            values->at(i) = constVals;
        } else {
            //todo: process call.procName, read.varname and print.varname
        }
    }
}

Table QE::ClauseVisitor::operator()(shared_ptr<AssignPatternClause> assignPatternClause) {
    Table table = dataRetriever->getTableByExprPattern(assignPatternClause->arg3);
    return dataPreprocessor->filterSingleClauseResultTable(assignPatternClause->arg1, assignPatternClause->arg2, table);
}

Table QE::ClauseVisitor::operator()(shared_ptr<IfPatternClause> ifPatternClause) {
    Table table = dataRetriever->getTableByCondExprPattern(DesignEntity::IF);
    return dataPreprocessor->filterSingleClauseResultTable(ifPatternClause->arg1, ifPatternClause->arg2, table);
}

Table QE::ClauseVisitor::operator()(shared_ptr<WhilePatternClause> whilePatternClause) {
    Table table = dataRetriever->getTableByCondExprPattern(DesignEntity::WHILE);
    return dataPreprocessor->filterSingleClauseResultTable(whilePatternClause->arg1, whilePatternClause->arg2, table);
}

//for no condition query
Table QE::ClauseVisitor::operator()(shared_ptr<SelectClause> selectClause) {
    TableCombiner tableCombiner = TableCombiner();
    Table resultTable;
    for (auto elem: *selectClause->returnResults) {
        if (elem.index() == SelectClause::ELEM_SYN_IDX) {
            Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
            DesignEntity designEntity = getDesignEntity(synonym);

            Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);
            if (intermediateTable.isBodyEmpty()) {
                return Table();
            }
            intermediateTable.renameHeader({synonym.synonym});
            resultTable = tableCombiner.crossProduct(resultTable, intermediateTable);
        } else {
            //elem is a attrRef
            AttrRef attrRef = std::get<AttrRef>(elem);
            DesignEntity designEntity = getDesignEntity(attrRef.synonym);
            Table intermediateTable = dataPreprocessor->getAllByDesignEntity(designEntity);

            if (intermediateTable.isBodyEmpty()) {
                return Table();
            }

            vector<DesignEntity> notDirectlyAvailEntities = {DesignEntity::CALL, DesignEntity::READ,
                                                             DesignEntity::PRINT};
            vector<AttrName> notDirectlyAvailAttrNames = {AttrName::PROC_NAME, AttrName::VAR_NAME};


            bool processingNeeded =
                    std::count(notDirectlyAvailEntities.begin(), notDirectlyAvailEntities.end(), designEntity)
                    && std::count(notDirectlyAvailAttrNames.begin(), notDirectlyAvailAttrNames.end(), attrRef.attrName);

            //todo: query pkb for data
            if (processingNeeded) {

            } else {
                for (int i = 1; i < intermediateTable.header.size(); i++) {
                    intermediateTable = intermediateTable.dropCol(i);
                }
                string headerName;
                headerName.append("$");
                headerName.append(attrRef.synonym.synonym);
                headerName.append(".");
                headerName.append(attrRef.getStrOfAttrName());
                intermediateTable.renameHeader({headerName});
            }
            resultTable = tableCombiner.crossProduct(resultTable, intermediateTable);
        }
    }
    return resultTable;
}

DesignEntity QE::ClauseVisitor::getDesignEntity(Synonym synonym) {
    for (Declaration d: *declarations) {
        if (synonym.synonym == d.getSynonym().synonym) {
            return d.getDesignEntity();
        }
    }
    return static_cast<DesignEntity>(NULL);
}

std::vector<std::string> QE::ClauseVisitor::intersection(std::vector<std::string> v1,
                                                      std::vector<std::string> v2){
    std::vector<std::string> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

std::vector<int> QE::ClauseVisitor::intersection(std::vector<int> v1,
                                              std::vector<int> v2){
    std::vector<int> v3;

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}
