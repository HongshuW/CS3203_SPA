//
// Created by Nafour on 5/9/22.
//

#include "DataPreprocessor.h"

#include <utility>
#include <string>
#include "query_builder/commons/DesignEntity.h"
#include "QEUtils.h"
#include "utils/Utils.h"
#include "TableCombiner.h"
#include "query_builder/commons/WithRef.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsTClause.h"
#include "query_builder/clauses/such_that_clauses/CallsClause.h"
#include "query_builder/clauses/such_that_clauses/CallsTClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsTClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/NextClause.h"
#include "query_builder/clauses/such_that_clauses/NextTClause.h"
#include "query_builder/clauses/such_that_clauses/ParentClause.h"
#include "query_builder/clauses/such_that_clauses/ParentTClause.h"
#include "query_builder/clauses/such_that_clauses/UsesPClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "constants/ClauseVisitorConstants.h"

using namespace std;
using namespace QB;
namespace QE {

    vector<DesignEntity> notDirectlyAvailDEs = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
    vector<AttrName> notDirectlyAvailAttrs = {AttrName::PROC_NAME, AttrName::VAR_NAME};

    Table DataPreprocessor::getAllByDesignEntity(DesignEntity designEntity) {
        const int FIRST_COL_IDX = 0;
        vector<DesignEntity> directlyAvailEntities = {DesignEntity::STMT, DesignEntity::CONSTANT,
                                                      DesignEntity::VARIABLE, DesignEntity::PROCEDURE};
        if (count(directlyAvailEntities.begin(), directlyAvailEntities.end(), designEntity)) {
            Table resultTable = this->dataRetriever->getTableByDesignEntity(designEntity);
            for (int i = 1; i < resultTable.header.size(); i++) {
                resultTable = resultTable.dropCol(i);
            }
            return resultTable;
        }

        //assign, call, print, if, while , read
        Table resultTable = this->filerTableByDesignEntity(
                this->dataRetriever->getTableByDesignEntity(QB::DesignEntity::STMT),
                FIRST_COL_IDX, designEntity);
        for (int i = 1; i < resultTable.header.size(); i++) {
            resultTable = resultTable.dropCol(i);
        }
        return resultTable;
    }

    Table
    DataPreprocessor::filerTableByDesignEntity(const Table &table, int colIdx, DesignEntity designEntity) {
        vector<DesignEntity> directlyAvailEntities = {DesignEntity::STMT, DesignEntity::CONSTANT,
                                                      DesignEntity::VARIABLE, DesignEntity::PROCEDURE};
        if (count(directlyAvailEntities.begin(), directlyAvailEntities.end(), designEntity)) return table;
        Table filteredTable = Table();
        filteredTable.header = table.header;
        for (auto row: table.rows) {
            string val = row[colIdx];
            //stmt types
            //assume val is a valid number
            DesignEntity entityType = this->dataRetriever->getDesignEntityOfStmt(stoi(val));
            if (designEntity == entityType) {
                filteredTable.appendRow(row);
            }
        }
        return filteredTable;
    }

    DesignEntity DataPreprocessor::getDesignEntityOfSyn(Synonym synonym) {
        for (auto d: *declarations) {
            if (synonym.synonym == d.getSynonym().synonym) {
                return d.getDesignEntity();
            }
        }
        return static_cast<DesignEntity>(NULL);
    }

    vector<int> DataPreprocessor::getStmtNumsByDesignEntity(DesignEntity designEntity) {
        Table table = this->getAllByDesignEntity(designEntity);
        int STMT_NO_COL_IDX = 0;
        auto stmtNums = table.getColumnByName(table.header[STMT_NO_COL_IDX]);
        vector<int> ans;
        for (const auto &string: stmtNums) {
            ans.push_back(stoi(string));
        }
        return ans;
    }

    vector<string> DataPreprocessor::getEntityNames(DesignEntity designEntity) {
        vector<DesignEntity> directlyAvailEntities = {DesignEntity::CONSTANT, DesignEntity::VARIABLE,
                                                      DesignEntity::PROCEDURE};
        if (count(directlyAvailEntities.begin(), directlyAvailEntities.end(), designEntity)) {
            int ENTITY_NAME_COL_IDX = 0;
            auto table = this->dataRetriever->getTableByDesignEntity(designEntity);
            return table.getColumnByName(table.header[ENTITY_NAME_COL_IDX]);
        }
        return vector<string>();
    }

    Table DataPreprocessor::getTableByWith(shared_ptr<WithClause> withClause) {
        bool hasSynonym =
                withClause->lhsType() == WithRefType::ATTR_REF || withClause->rhsType() == WithRefType::ATTR_REF;
        //if the with clause does not have synonym, dont have to care what's inside since
        //I have checked this clause evaluates to true
        const string DUMMY_HEADER = "$dummy_header";
        const string DUMMY_VALUE = "$dummy_value";
        Table dummyTable = Table();
        dummyTable.renameHeader({DUMMY_HEADER}) ;
        dummyTable.rows = vector<vector<string>>({{DUMMY_VALUE}});
        if (!hasSynonym) return dummyTable;

        int intRef;
        string identRef;
        bool isIntType = true;

        vector<WithRef> withRefs = {withClause->lhs, withClause->rhs};
        Table resultTable = Table();
        vector<int> comparedCols = {};
        int colOffSet = 0;


        for (auto withRef: withRefs) {
            WithRefType withRefType = WithClause::getWithRefType(withRef.index());
            if (withRefType == QB::WithRefType::INTEGER) intRef = get<WithClause::WITHREF_INT_IDX>(withRef);
            if (withRefType == QB::WithRefType::IDENT) {
                isIntType = false;
                identRef = get<WithClause::WITHREF_IDENT_IDX>(withRef).identStr;
            }
            if (withRefType != WithRefType::ATTR_REF) continue;

            AttrRef attrRef = get<WithClause::WITHREF_ATTR_REF_IDX>(withRef);
            DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym);

            bool processingNeeded = std::count(notDirectlyAvailDEs.begin(), notDirectlyAvailDEs.end(), designEntity)
                                    && std::count(notDirectlyAvailAttrs.begin(), notDirectlyAvailAttrs.end(),
                                                  attrRef.attrName);

            const int TABLE_HEADER_SIZE = 2;
            if (!processingNeeded) {
                Table table = getAllByDesignEntity(designEntity);
                if (table.isBodyEmpty()) return Table();
                const int FIRST_COL_IDX = 0;
                const int DROP_COL_FROM = 1;
                for (int i = DROP_COL_FROM; i < table.header.size(); i++) {
                    table = table.dropCol(i);
                }
                //get two identical cols with different names
                table = table.dupCol(FIRST_COL_IDX);
                table.renameHeader({attrRef.synonym.synonym, attrRef.toString()});
                resultTable = TableCombiner().crossProduct( resultTable, table);

                //since two cols are identical, compare either is alright
                comparedCols.push_back(colOffSet);
                colOffSet += (int) TABLE_HEADER_SIZE;
            } else {
                const int ATTR_NAME_COL_INX = 1;

                Table intermediateTable; //get results
                if (designEntity == QB::DesignEntity::CALL) intermediateTable = dataRetriever->getCallsProcedureNames();
                if (designEntity == QB::DesignEntity::READ) intermediateTable = dataRetriever->getReadVariableNames();
                if (designEntity == QB::DesignEntity::PRINT) intermediateTable = dataRetriever->getPrintVariableNames();

                intermediateTable.renameHeader({attrRef.synonym.synonym, attrRef.toString()});
                resultTable = TableCombiner().crossProduct( resultTable, intermediateTable);

                //compare the second col which is the attribute value
                comparedCols.push_back(colOffSet + ATTR_NAME_COL_INX);
                colOffSet += (int) TABLE_HEADER_SIZE;
            }
        }
        if ( withClause->lhsType() == WithRefType::ATTR_REF && withClause->rhsType() == WithRefType::ATTR_REF) {
            resultTable = filterTableByColValueEquality(resultTable, comparedCols);
        } else {
            const int ATTR_NAME_COL_IDX = 1;
            resultTable = filerTableByColumnIdx(resultTable, ATTR_NAME_COL_IDX, isIntType ? to_string(intRef) : identRef);
        }

        return resultTable;
    }

    Table DataPreprocessor::filterTableByColValueEquality(const Table& table, const vector<int>& comparedCols) {
        Table filteredTable = Table();

        filteredTable.renameHeader(table.header);
        for (auto row: table.rows) {
            unordered_set<string> set;
            for (int colIdx: comparedCols) {
                set.insert(row[colIdx]);
            }
            if (set.size() > 1) continue; //values are not the same
            filteredTable.appendRow(row);
        }
        return filteredTable;
    }

    Table DataPreprocessor::filterSingleClauseResultTable(Ref ref1, Ref ref2, Table table) {
        //RefType can be synonym, integer, underscore or string
        RefType ref1Type = getRefType(ref1);
        RefType ref2Type = getRefType(ref2);
        string col1Name =
                ref1Type == QB::RefType::SYNONYM ? get<Synonym>(ref1).synonym : QEUtils::getColNameByRefType(ref1Type);
        string col2Name =
                ref2Type == QB::RefType::SYNONYM ? get<Synonym>(ref2).synonym : QEUtils::getColNameByRefType(ref2Type);

        vector<string> newHeaders = vector<string>{col1Name, col2Name};
        table.renameHeader(newHeaders);

        const int FIRST_COL_IDX = 0;
        const int SECOND_COL_IDX = 1;

        switch (ref1Type) {
            case RefType::SYNONYM: {
                Synonym syn1 = get<Synonym>(ref1);
                table = this->filerTableByDesignEntity(table, FIRST_COL_IDX,
                                                       this->getDesignEntityOfSyn(syn1));
                break;
            }
            case RefType::UNDERSCORE:
                break;
            case RefType::INTEGER: {
                int int1 = get<int>(ref1);
                table = this->filerTableByColumnIdx(table, FIRST_COL_IDX, to_string(int1));
                break;
            }
            case RefType::IDENT:
                Ident ident1 = get<Ident>(ref1);
                table = this->filerTableByColumnIdx(table, FIRST_COL_IDX, ident1.identStr);
                break;
        }
        switch (ref2Type) {
            case RefType::SYNONYM: {
                Synonym syn2 = get<Synonym>(ref2);
                table = this->filerTableByDesignEntity(table, SECOND_COL_IDX,
                                                       this->getDesignEntityOfSyn(syn2));

                //synonyms are the same: equality check between the two cols
                if (ref1Type != QB::RefType::SYNONYM) break;
                Synonym syn1 = get<Synonym>(ref1);
                if (!(syn1 == syn2)) break;

                return this->filterTableByColValueEquality(table, {FIRST_COL_IDX, SECOND_COL_IDX});
            }
            case RefType::UNDERSCORE:
                break;
            case RefType::INTEGER: {
                int int2 = get<int>(ref2);
                table = this->filerTableByColumnIdx(table, SECOND_COL_IDX, to_string(int2));

                if (ref1Type != QB::RefType::INTEGER) break;
                int int1 = get<int>(ref1);
                if (int1 != int2) break;
                return this->filterTableByColValueEquality(table, {FIRST_COL_IDX, SECOND_COL_IDX});
            }
            case RefType::IDENT:
                Ident ident2 = get<Ident>(ref2);
                table = this->filerTableByColumnIdx(table, SECOND_COL_IDX, ident2.identStr);

                if (ref1Type != QB::RefType::IDENT) break;
                Ident ident1 = get<Ident>(ref1);
                if (!(ident1 == ident2)) break;
                return this->filterTableByColValueEquality(table, {FIRST_COL_IDX, SECOND_COL_IDX});
        }
        //drop unused columns
        for (int i = 0; i < table.header.size(); i++) {
            if (table.header[i].find('$') != std::string::npos) {
                table = table.dropCol(i);
            }
        }
        return table;
    }

    DataPreprocessor::DataPreprocessor(shared_ptr<DataRetriever> dataRetriever, Declarations declarations)
            : dataRetriever(dataRetriever), declarations(declarations) {

    }

    Table DataPreprocessor::getTableByAssignPattern(shared_ptr<QB::AssignPatternClause> assignPatternClause) {
        return filterSingleClauseResultTable(assignPatternClause->arg1, assignPatternClause->arg2,
                                             dataRetriever->getAssignPatternTable(assignPatternClause->arg3));
    }

    Table DataPreprocessor::getIfPatternTable(shared_ptr<QB::IfPatternClause> ifPatternClause) {
        return filterSingleClauseResultTable(ifPatternClause->arg1, ifPatternClause->arg2,
                                             dataRetriever->  getIfPatternTable());
    }

    Table DataPreprocessor::getWhilePatternTable(shared_ptr<WhilePatternClause> whilePatternClause) {
        return filterSingleClauseResultTable(whilePatternClause->arg1, whilePatternClause->arg2,
                                             dataRetriever->getWhilePatternTable());
    }

    Table DataPreprocessor::getTableByFollows(shared_ptr<FollowsClause> followsClause) {
        Table table = dataRetriever->getFollowsTable();
        return filterSingleClauseResultTable(followsClause->arg1, followsClause->arg2, table);
    }

    Table DataPreprocessor::getTableByFollowsT(shared_ptr<FollowsTClause> followsTClause) {
        Table table = dataRetriever->getFollowsTTable();
        return filterSingleClauseResultTable(followsTClause->arg1, followsTClause->arg2, table);
    }

    Table DataPreprocessor::getTableByAffects(shared_ptr<AffectsClause> affectsClause) {
        Table table = dataRetriever->getAffectsTable();
        return filterSingleClauseResultTable(affectsClause->arg1, affectsClause->arg2, table);
    }

    Table DataPreprocessor::getTableByAffectsT(shared_ptr<AffectsTClause> affectsTClause) {
        Table table = dataRetriever->getAffectsTTable();
        return filterSingleClauseResultTable(affectsTClause->arg1, affectsTClause->arg2, table);
    }

    Table DataPreprocessor::getTableByCalls(shared_ptr<CallsClause> callsClause) {
        Table table = dataRetriever->getCallsTable();
        return filterSingleClauseResultTable(callsClause->arg1, callsClause->arg2, table);
    }

    Table DataPreprocessor::getTableByCallsT(shared_ptr<CallsTClause> callsTClause) {
        Table table = dataRetriever->getCallsTTable();
        return filterSingleClauseResultTable(callsTClause->arg1, callsTClause->arg2, table);
    }

    Table DataPreprocessor::getTableByModifiesP(shared_ptr<ModifiesPClause> modifiesPClause) {
        Table table = dataRetriever->getModifiesPTable();
        return filterSingleClauseResultTable(modifiesPClause->arg1, modifiesPClause->arg2, table);
    }

    Table DataPreprocessor::getTableByModifiesS(shared_ptr<ModifiesSClause> modifiesSClause) {
        Table table = dataRetriever->getModifiesSTable();
        return filterSingleClauseResultTable(modifiesSClause->arg1, modifiesSClause->arg2, table);
    }

    Table DataPreprocessor::getTableByNext(shared_ptr<NextClause> nextClause) {
        Table table = dataRetriever->getNextTable();
        return filterSingleClauseResultTable(nextClause->arg1, nextClause->arg2, table);
    }

    Table DataPreprocessor::getTableByNextT(shared_ptr<NextTClause> nextTClause) {
        Table table = dataRetriever->getNextTTable();
        return filterSingleClauseResultTable(nextTClause->arg1, nextTClause->arg2, table);
    }

    Table DataPreprocessor::getTableByParent(shared_ptr<ParentClause> parentClause) {
        Table table = dataRetriever->getParentTable();
        return filterSingleClauseResultTable(parentClause->arg1, parentClause->arg2, table);
    }

    Table DataPreprocessor::getTableByParentT(shared_ptr<ParentTClause> parentTClause) {
        Table table = dataRetriever->getParentTTable();
        return filterSingleClauseResultTable(parentTClause->arg1, parentTClause->arg2, table);
    }

    Table DataPreprocessor::getTableByUsesP(shared_ptr<UsesPClause> usesPClause) {
        Table table = dataRetriever->getUsesPTable();
        return filterSingleClauseResultTable(usesPClause->arg1, usesPClause->arg2, table);
    }

    Table DataPreprocessor::getTableByUsesS(shared_ptr<UsesSClause> usesSClause) {
        Table table = dataRetriever->getUsesSTable();
        return filterSingleClauseResultTable(usesSClause->arg1, usesSClause->arg2, table);
    }

    Table DataPreprocessor::getCallsProcedureTable() {
        return dataRetriever->getCallsProcedureNames();
    }

    Table DataPreprocessor::getReadVariableTable() {
        return dataRetriever->getReadVariableNames();
    }

    Table DataPreprocessor::getPrintVariableTable() {
        return dataRetriever->getPrintVariableNames();
    }

    Table DataPreprocessor::getNoConditionSelectClauseResult(shared_ptr<SelectClause> selectClause) {
        TableCombiner tableCombiner = TableCombiner();
        Table resultTable;
        for (auto elem: *selectClause->returnResults) {
            if (elem.index() == SelectClause::ELEM_SYN_IDX) {
                Synonym synonym = std::get<SelectClause::ELEM_SYN_IDX>(elem);
                DesignEntity designEntity = getDesignEntityOfSyn(synonym);

                Table intermediateTable = getAllByDesignEntity(designEntity);
                if (intermediateTable.isBodyEmpty()) {
                    return Table();
                }
                intermediateTable.renameHeader({synonym.synonym});
                resultTable = tableCombiner.crossProduct(resultTable, intermediateTable);
            } else {
                //elem is a attrRef
                AttrRef attrRef = std::get<AttrRef>(elem);
                DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym);
                Table intermediateTable = getAllByDesignEntity(designEntity);

                if (intermediateTable.isBodyEmpty()) {
                    return Table();
                }

                vector<DesignEntity> notDirectlyAvailEntities = {DesignEntity::CALL, DesignEntity::READ,
                                                                 DesignEntity::PRINT};
                vector<AttrName> notDirectlyAvailAttrNames = {AttrName::PROC_NAME, AttrName::VAR_NAME};


                bool processingNeeded =
                        std::count(notDirectlyAvailEntities.begin(), notDirectlyAvailEntities.end(), designEntity)
                        && std::count(notDirectlyAvailAttrNames.begin(), notDirectlyAvailAttrNames.end(),
                                      attrRef.attrName);

                if (processingNeeded) {
                    if (designEntity == QB::DesignEntity::CALL) intermediateTable = getCallsProcedureTable();
                    if (designEntity == QB::DesignEntity::PRINT) intermediateTable = getPrintVariableTable();
                    if (designEntity == QB::DesignEntity::READ) intermediateTable = getReadVariableTable();
                    if (intermediateTable.isBodyEmpty()) return Table();
                } else {
                    for (int i = 1; i < intermediateTable.header.size(); i++) {
                        intermediateTable = intermediateTable.dropCol(i);
                    }
                    const int FIRST_COL_IDX = 0;
                    intermediateTable = intermediateTable.dupCol(FIRST_COL_IDX);
                }
                intermediateTable.renameHeader({attrRef.synonym.synonym, attrRef.toString()});
                resultTable = tableCombiner.crossProduct(intermediateTable, resultTable);
            }
        }
        return resultTable;
    }

    bool DataPreprocessor::hasResult(shared_ptr<WithClause> withClause) {
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
                if (valRHS.index() == ClauseVisitorConstants::STR_VAL_IDX)
                    return lhsStr ==
                           get<ClauseVisitorConstants::STR_VAL_IDX>(
                                   valRHS);

                if (valRHS.index() == ClauseVisitorConstants::VECTOR_STR_VAL_IDX) {
                    vector<string> strs = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valRHS);
                    return count(strs.begin(), strs.end(), lhsStr);
                }
                break;
            }
            case ClauseVisitorConstants::INT_VAL_IDX: {
                int lhsInt = get<ClauseVisitorConstants::INT_VAL_IDX>(valLHS);
                if (valRHS.index() == ClauseVisitorConstants::INT_VAL_IDX)
                    return lhsInt == get<ClauseVisitorConstants::INT_VAL_IDX>(valRHS);
                if (valRHS.index() == ClauseVisitorConstants::VECTOR_INT_VAL_IDX) {
                    vector<int> ints = get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valRHS);
                    return count(ints.begin(), ints.end(), lhsInt);
                }
                break;
            }
            case ClauseVisitorConstants::VECTOR_STR_VAL_IDX: {
                vector<string> lhsVecStr = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valLHS);
                if (valRHS.index() == ClauseVisitorConstants::STR_VAL_IDX) {
                    string rhsStr = get<ClauseVisitorConstants::STR_VAL_IDX>(valRHS);
                    return count(lhsVecStr.begin(), lhsVecStr.end(), rhsStr);
                }
                if (valRHS.index() == ClauseVisitorConstants::VECTOR_STR_VAL_IDX) {
                    vector<string> rhsVecStr = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(valRHS);
                    return !intersection(lhsVecStr, rhsVecStr).empty();
                }
                break;
            }

            case ClauseVisitorConstants::VECTOR_INT_VAL_IDX: {
                vector<int> lhsVecInt = get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valLHS);
                if (valRHS.index() == ClauseVisitorConstants::INT_VAL_IDX) {
                    int rhsInt = get<ClauseVisitorConstants::INT_VAL_IDX>(valRHS);
                    return count(lhsVecInt.begin(), lhsVecInt.end(), rhsInt);
                }
                if (valRHS.index() == ClauseVisitorConstants::VECTOR_INT_VAL_IDX) {
                    vector<int> rhsVecInt = get<ClauseVisitorConstants::VECTOR_INT_VAL_IDX>(valRHS);
                    return !intersection(lhsVecInt, rhsVecInt).empty();
                }
                break;
            }
        }
        return false;
    }

    std::vector<std::string> DataPreprocessor::intersection(std::vector<std::string> v1,
                                                            std::vector<std::string> v2) {
        std::vector<std::string> v3;

        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());

        std::set_intersection(v1.begin(), v1.end(),
                              v2.begin(), v2.end(),
                              back_inserter(v3));
        return v3;
    }

    std::vector<int> DataPreprocessor::intersection(std::vector<int> v1,
                                                    std::vector<int> v2) {
        std::vector<int> v3;

        std::sort(v1.begin(), v1.end());
        std::sort(v2.begin(), v2.end());

        std::set_intersection(v1.begin(), v1.end(),
                              v2.begin(), v2.end(),
                              back_inserter(v3));
        return v3;
    }

    void QE::DataPreprocessor::getWithValues(vector<WithRef> withRefs, shared_ptr<vector<Value>> values) {

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
            DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym);
            vector<DesignEntity> notDirectlyAvailDEs = {DesignEntity::CALL, DesignEntity::READ,
                                                        DesignEntity::PRINT};
            vector<AttrName> notDirectlyAvailAttrs = {AttrName::PROC_NAME, AttrName::VAR_NAME};
            bool processingNeeded = std::count(notDirectlyAvailDEs.begin(), notDirectlyAvailDEs.end(), designEntity)
                                    && std::count(notDirectlyAvailAttrs.begin(), notDirectlyAvailAttrs.end(),
                                                  attrRef.attrName);
            if (!processingNeeded) {
                if (attrRef.attrName == AttrName::PROC_NAME) {
                    values->at(i) = getEntityNames(designEntity);//design entity must be procedure
                    continue;
                }

                if (attrRef.attrName == AttrName::VAR_NAME) {
                    values->at(i) = getEntityNames(designEntity);//design entity must be variable
                    continue;
                }

                if (attrRef.attrName == AttrName::STMT_NUMBER) {
                    values->at(i) = getStmtNumsByDesignEntity(designEntity); //vector<int>
                    continue;
                }
                //attrRef.attrName == AttrName::VALUE
                values->at(i) = getEntityNames(
                        designEntity);//this can only be constant values with return type vector<string>
                vector<int> constVals;
                vector<string> strVals = get<ClauseVisitorConstants::VECTOR_STR_VAL_IDX>(values->at(i));
                for (auto strVal: strVals) {
                    constVals.push_back(stoi(strVal));
                }
                values->at(i) = constVals;
            } else {
                if (attrRef.attrName == AttrName::PROC_NAME) {
                    values->at(i) = getCallsProcedureTable().getColumnByName(PKBStorage::CALLS_TABLE_COL2_NAME);
                }
                if (attrRef.attrName == QB::AttrName::VAR_NAME && designEntity == QB::DesignEntity::READ) {
                    values->at(i) = getReadVariableTable().getColumnByName(PKBStorage::VARIABLE_TABLE_COL1_NAME);
                }
                if (attrRef.attrName == QB::AttrName::VAR_NAME && designEntity == QB::DesignEntity::PRINT) {
                    values->at(i) = getPrintVariableTable().getColumnByName(PKBStorage::VARIABLE_TABLE_COL1_NAME);
                }
            }
        }
    }

    Table DataPreprocessor::filerTableByColumnIdx(const Table &table, int colIdx, const string &value) {
        Table filteredTable = Table();
        filteredTable.header = table.header;
        for (auto row: table.rows) {
            if (row[colIdx] == value) {
                filteredTable.appendRow(row);
            }
        }
        return filteredTable;
    }

} // QE