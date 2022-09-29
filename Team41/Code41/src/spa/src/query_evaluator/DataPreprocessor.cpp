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
using namespace std;
using namespace QB;
namespace QE {

    vector<DesignEntity> notDirectlyAvailDEs = {DesignEntity::CALL, DesignEntity::READ, DesignEntity::PRINT};
    vector<AttrName> notDirectlyAvailAttrs = {AttrName::PROC_NAME, AttrName::VAR_NAME};
    vector<DesignEntity> stmtEntities = {DesignEntity::STMT, DesignEntity::READ, DesignEntity::PRINT, DesignEntity::CALL, DesignEntity::WHILE, DesignEntity::IF, DesignEntity::ASSIGN};


    Table
    DataPreprocessor::getTableByRelation(SuchThatClause suchThatClause) {
        return this->getTableByRelationHelper(suchThatClause.arg1, suchThatClause.arg2, suchThatClause.relationType,
                                              suchThatClause.declarations);
    }

    Table DataPreprocessor::getTableByRelationHelper(Ref ref1, Ref ref2, RelationType relationType,
                                                     shared_ptr<vector<Declaration>> declarations) {
        //RefType can be synonym, integer, underscore or string
        RefType ref1Type = getRefType(ref1);
        RefType ref2Type = getRefType(ref2);
        string col1Name = ref1Type == QB::RefType::SYNONYM ? get<Synonym>(ref1).synonym : QEUtils::getColNameByRefType(ref1Type);
        string col2Name = ref2Type == QB::RefType::SYNONYM ? get<Synonym>(ref2).synonym : QEUtils::getColNameByRefType(ref2Type);

        if (col1Name == col2Name) {
            col2Name = col2Name + "_2";
        }

        Table relationTable = this->dataRetriever->getTableByRelationType(relationType);
        vector<string> newHeaders = vector<string>{col1Name, col2Name};
        relationTable.renameHeader(newHeaders);
        switch (ref1Type) {
            case RefType::SYNONYM: {
                Synonym syn1 = get<Synonym>(ref1);
                relationTable = this->filerTableByDesignEntity(relationTable,col1Name,
                                                               this->getDesignEntityOfSyn(syn1, declarations));
                break;
            }
            case RefType::UNDERSCORE:
                break;
            case RefType::INTEGER: {
                int int1 = get<int>(ref1);
                relationTable = this->filerTableByColumnValue(relationTable, col1Name, to_string(int1));
                break;
            }
            case RefType::IDENT:
                Ident ident1 = get<Ident>(ref1);
                relationTable = this->filerTableByColumnValue(relationTable, col1Name, ident1.identStr);
                break;
        }
        switch (ref2Type) {
            case RefType::SYNONYM: {
                Synonym syn2 = get<Synonym>(ref2);
                relationTable = this->filerTableByDesignEntity(relationTable,col2Name,
                                                               this->getDesignEntityOfSyn(syn2, declarations));

                //Same synonym such as: stmt s; variable v; Select v such that Follows(s, s)
                if (ref1Type == QB::RefType::SYNONYM) {
                    Synonym syn1 = get<Synonym>(ref1);
                    if (syn1 == syn2) {
                        return Table();
                    }
                }

                break;
            }
            case RefType::UNDERSCORE:
                break;
            case RefType::INTEGER: {
                int int2 = get<int>(ref2);
                relationTable = this->filerTableByColumnValue(relationTable, col2Name, to_string(int2));
                break;
            }
            case RefType::IDENT:
                Ident ident2 = get<Ident>(ref2);
                relationTable = this->filerTableByColumnValue(relationTable, col2Name, ident2.identStr);
                break;
        }
        return relationTable;
    }

    Table DataPreprocessor::getAllByDesignEntity(DesignEntity designEntity) {
        vector<DesignEntity> directlyAvailEntities = {DesignEntity::STMT, DesignEntity::CONSTANT, DesignEntity::VARIABLE, DesignEntity::PROCEDURE};
        if (count(directlyAvailEntities.begin(), directlyAvailEntities.end(), designEntity)) {
            Table resultTable = this->dataRetriever->getTableByDesignEntity(designEntity);
            for (int i = 1; i < resultTable.header.size(); i++) {
                resultTable = resultTable.dropCol(i);
            }
            return resultTable;
        }

        //assign, call, print, if, while , read
        Table resultTable = this->filerTableByDesignEntity(this->dataRetriever->getTableByDesignEntity(QB::DesignEntity::STMT), PKBStorage::STATEMENT_TABLE_COL1_NAME, designEntity);
        for (int i = 1; i < resultTable.header.size(); i++) {
            resultTable = resultTable.dropCol(i);
        }
        return resultTable;
    }

    Table DataPreprocessor::filerTableByColumnValue(const Table& table, const string& colName, const string& value) {
        long colIdx = this->getColIndexByColName(table.header, colName);
        Table filteredTable = Table();
        filteredTable.header = table.header;
        for (auto row: table.rows) {
            if (row[colIdx] == value) {
                filteredTable.appendRow(row);
            }
        }
        return filteredTable;
    }

    Table DataPreprocessor::filerTableByDesignEntity(const Table &table, const string &colName, DesignEntity designEntity) {
        long colIdx = this->getColIndexByColName(table.header, colName);
        Table filteredTable = Table();
        filteredTable.header = table.header;
        for (auto row: table.rows) {
            string val = row[colIdx];
            switch (designEntity) {
                case DesignEntity::VARIABLE: {
                    return table;
                }
                case DesignEntity::CONSTANT: {
                    return table;
                }
                case DesignEntity::PROCEDURE: {
                    //todo: implement procedure filter
                    return table;
                }
                case DesignEntity::STMT: {
                    return table;
                }
                default: {//stmt types
                    if (!Utils::isValidNumber(val)) {
                        cout << "Error: stmt line is not a number" << endl;
                    }
                    DesignEntity entityType = this->dataRetriever->getDesignEntityOfStmt(stoi(val));
                    if (designEntity == entityType) {
                        filteredTable.appendRow(row);
                    }
                }

            }
        }
        return filteredTable;
    }

    long DataPreprocessor::getColIndexByColName(vector<string> v, const string& K) {
        auto it = find(v.begin(), v.end(), K);

        // If element was found
        if (it != v.end())
        {

            // calculating the index
            // of K
            auto index = it - v.begin();
            return index;
        }
        else {
            // If the element is not
            // present in the vector
            return -1;
        }
    }

    DesignEntity DataPreprocessor::getDesignEntityOfSyn(Synonym synonym, shared_ptr<vector<Declaration>> declarations) {
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
        for (const auto& string: stmtNums) {
            ans.push_back(stoi(string));
        }
        return ans;
    }

    vector<string> DataPreprocessor::getEntityNames(DesignEntity designEntity) {
        vector<DesignEntity> directlyAvailEntities = {DesignEntity::CONSTANT, DesignEntity::VARIABLE, DesignEntity::PROCEDURE};
        if (count(directlyAvailEntities.begin(), directlyAvailEntities.end(), designEntity)) {
            int ENTITY_NAME_COL_IDX = 0;
            auto table = this->dataRetriever->getTableByDesignEntity(designEntity);
            return table.getColumnByName(table.header[ENTITY_NAME_COL_IDX]);
        }
        return vector<string>();
    }

    Table DataPreprocessor::getTableByWith(shared_ptr<WithClause> withClause,  shared_ptr<vector<Declaration>> declarations) {

        bool hasSynonym = withClause->lhsType() == WithRefType::ATTR_REF || withClause->rhsType() == WithRefType::ATTR_REF;
        if (!hasSynonym) return Table();
        vector<WithRef> withRefs = {withClause->lhs, withClause->rhs};
        Table resultTable = Table();
        vector<int> comparedCols = {};
        int colOffSet = 0;
        for (auto withRef: withRefs) {
            WithRefType withRefType = WithClause::getWithRefType(withRef.index());
            if (withRefType != WithRefType::ATTR_REF) continue;
            AttrRef attrRef = get<WithClause::WITHREF_ATTR_REF_IDX>(withRef);
            DesignEntity designEntity = getDesignEntityOfSyn(attrRef.synonym, declarations);
            bool processingNeeded = std::count(notDirectlyAvailDEs.begin(), notDirectlyAvailDEs.end(), designEntity)
                                    && std::count(notDirectlyAvailAttrs.begin(), notDirectlyAvailAttrs.end(), attrRef.attrName);

            if (!processingNeeded) {
                Table table = getAllByDesignEntity(designEntity);
                if (table.isBodyEmpty()) return Table();
                const int DROP_COL_FROM = 1;
                for (int i = DROP_COL_FROM; i < table.header.size(); i++) {
                    table = table.dropCol(i);
                }
                table.renameHeader({attrRef.synonym.synonym});
                resultTable = TableCombiner().crossProduct(table, resultTable);
                comparedCols.push_back(colOffSet);
                colOffSet += (int) table.header.size();
            } else {
                //todo: query from pkb
                const int TABLE_HEADER_SIZE = 2;
                const int ATTR_NAME_COL_INX = 1;

                Table dummyPKBTable; //get results
                string attrName;
                //todo: remove magic strings
                attrName.append("$");
                attrName.append(attrRef.synonym.synonym);
                attrName.append(".");
                attrName.append(attrRef.getStrOfAttrName());
                dummyPKBTable.renameHeader({attrRef.synonym.synonym, attrName});
                resultTable = TableCombiner().crossProduct(dummyPKBTable, resultTable);
                comparedCols.push_back(colOffSet + ATTR_NAME_COL_INX);
                colOffSet += (int) TABLE_HEADER_SIZE;
            }
        }
        resultTable = filterTableByColValueEquality(resultTable, comparedCols);
        return resultTable;
    }

    Table DataPreprocessor::filterTableByColValueEquality(Table table, vector<int> comparedCols) {
        Table filteredTable = table;

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
        string col1Name = ref1Type == QB::RefType::SYNONYM ? get<Synonym>(ref1).synonym : QEUtils::getColNameByRefType(ref1Type);
        string col2Name = ref2Type == QB::RefType::SYNONYM ? get<Synonym>(ref2).synonym : QEUtils::getColNameByRefType(ref2Type);

        vector<string> newHeaders = vector<string>{col1Name, col2Name};
        table.renameHeader(newHeaders);
        switch (ref1Type) {
            case RefType::SYNONYM: {
                Synonym syn1 = get<Synonym>(ref1);
                table = this->filerTableByDesignEntity(table,col1Name,
                                                       this->getDesignEntityOfSyn(syn1, declarations));
                break;
            }
            case RefType::UNDERSCORE:
                break;
            case RefType::INTEGER: {
                int int1 = get<int>(ref1);
                table = this->filerTableByColumnValue(table, col1Name, to_string(int1));
                break;
            }
            case RefType::IDENT:
                Ident ident1 = get<Ident>(ref1);
                table = this->filerTableByColumnValue(table, col1Name, ident1.identStr);
                break;
        }
        switch (ref2Type) {
            case RefType::SYNONYM: {
                Synonym syn2 = get<Synonym>(ref2);
                table = this->filerTableByDesignEntity(table,col2Name,
                                                       this->getDesignEntityOfSyn(syn2, declarations));

                //synonyms are the same: equality check between the two cols
                if (ref1Type != QB::RefType::SYNONYM) break;
                Synonym syn1 = get<Synonym>(ref1);
                if (!(syn1 == syn2)) break;
                const int FIRST_COL_IDX = 0;
                const int SECOND_COL_IDX = 1;
                return this->filterTableByColValueEquality(table, {FIRST_COL_IDX, SECOND_COL_IDX});
            }
            case RefType::UNDERSCORE:
                break;
            case RefType::INTEGER: {
                int int2 = get<int>(ref2);
                table = this->filerTableByColumnValue(table, col2Name, to_string(int2));
                break;
            }
            case RefType::IDENT:
                Ident ident2 = get<Ident>(ref2);
                table = this->filerTableByColumnValue(table, col2Name, ident2.identStr);
                break;
        }
        return table;
    }

    DataPreprocessor::DataPreprocessor(shared_ptr<DataRetriever> dataRetriever, Declarations declarations): dataRetriever(dataRetriever), declarations(declarations) {

    }


} // QE