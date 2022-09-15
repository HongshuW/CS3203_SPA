//
// Created by Nafour on 5/9/22.
//

#include "DataPreprocessor.h"

#include <utility>
#include <string>
#include "query_builder/commons/DesignEntity.h"
#include "QEUtils.h"
#include "utils/Utils.h"

using namespace std;
using namespace QB;
namespace QE {

    DataPreprocessor::DataPreprocessor(shared_ptr<DataRetriever> dataRetriever) {
        this->dataRetriever = std::move(dataRetriever);
    }

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
            return this->dataRetriever->getTableByDesignEntity(designEntity);
        }

        //assign, call, print, if, while , read
        return this->filerTableByDesignEntity(this->dataRetriever->getTableByDesignEntity(QB::DesignEntity::STMT), PKBStorage::STATEMENT_TABLE_COL1_NAME, designEntity);
    }

    Table DataPreprocessor::getTableByPattern(shared_ptr<PatternClause> patternClause) {

        //table of stmtNo, varname
        Table resultTable = this->dataRetriever->getTableByPattern(patternClause->arg3);

        //process result table: rename headers + filter
        string col1Name = patternClause->arg1.synonym; //arg1 must be an assign synonym
        Ref ref2 = patternClause->arg2;
        RefType ref2Type = getRefType(ref2); //arg2 can be syn, _ or ident
        string col2Name = ref2Type == QB::RefType::SYNONYM ? get<Synonym>(patternClause->arg2).synonym : QEUtils::getColNameByRefType(ref2Type);
        resultTable.renameHeader({col1Name, col2Name});

        if (ref2Type == QB::RefType::IDENT) {
            Ident ident2 = get<Ident>(ref2);
            resultTable = this->filerTableByColumnValue(resultTable, col2Name, ident2.identStr);
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


} // QE