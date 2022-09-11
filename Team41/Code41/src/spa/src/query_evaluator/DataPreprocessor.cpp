//
// Created by Nafour on 5/9/22.
//

#include "DataPreprocessor.h"

#include <utility>
#include <string>
#include "query_builder/commons/DesignEntity.h"
#include "Utils.h"
using namespace std;
using namespace QB;
namespace QE {
    Table DataPreprocessor::getAllByDesignEntity(DesignEntity designEntity) {
        vector<DesignEntity> directlyAvailEntities = {DesignEntity::STMT, DesignEntity::CONSTANT, DesignEntity::VARIABLE, DesignEntity::PROCEDURE};
        if (count(directlyAvailEntities.begin(), directlyAvailEntities.end(), designEntity)) {
            return this->dataRetriever->getTableByDesignEntity(designEntity);
        }

        //assign, call, print, if, while , read
        return this->filerTableByDesignEntity(this->dataRetriever->getTableByDesignEntity(QB::DesignEntity::STMT), EntityManager::STATEMENT_TABLE_COL1_NAME, designEntity);
    }

    Table
    DataPreprocessor::getTableByRelation(SuchThatClause suchThatClause) {
        return this->getRelationTable(suchThatClause.arg1, suchThatClause.arg2, suchThatClause.relationType,
                                      suchThatClause.declarations);
    }

    DataPreprocessor::DataPreprocessor(shared_ptr<DataRetriever> dataRetriever) {
        this->dataRetriever = std::move(dataRetriever);
    }

    Table DataPreprocessor::getRelationTable(Ref ref1, Ref ref2, RelationType relationType,
                                             shared_ptr<vector<Declaration>> declarations) {
        //RefType can be synonym, integer, underscore or string
        RefType ref1Type = getRefType(ref1);
        RefType ref2Type = getRefType(ref2);
        string col1Name = ref1Type == QB::RefType::SYNONYM ? get<Synonym>(ref1).synonym : Utils::getColNameByRefType(ref1Type);
        string col2Name = ref2Type == QB::RefType::SYNONYM ? get<Synonym>(ref2).synonym : Utils::getColNameByRefType(ref2Type);
        Table relationTable = this->dataRetriever->getTableByRelationType(relationType);
        vector<string> newHeaders = vector<string>{col1Name, col2Name};
        relationTable.renameHeader(newHeaders);
        switch (ref1Type) {
            case RefType::SYNONYM: {
                Synonym syn1 = get<Synonym>(ref1);
                relationTable = this->filerTableByDesignEntity(relationTable,col1Name,this->getDesignEntity(syn1, declarations));
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
                relationTable = this->filerTableByDesignEntity(relationTable,col2Name,this->getDesignEntity(syn2, declarations));
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

    Table DataPreprocessor::filerTableByColumnValue(const Table& table, const string& colName, const string& value) {
        long colIdx = this->getIndex(table.header, colName);
        Table filteredTable = Table();
        filteredTable.header = table.header;
        for (auto row: table.rows) {
            if (row[colIdx] == value) {
                filteredTable.appendRow(row);
            }
        }
        return filteredTable;
    }


    long DataPreprocessor::getIndex(vector<string> v, const string& K) {
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

    Table DataPreprocessor::filerTableByDesignEntity(const Table &table, const string &colName, DesignEntity designEntity) {
        long colIdx = this->getIndex(table.header, colName);
        Table filteredTable = Table();
        filteredTable.header = table.header;
        for (auto row: table.rows) {
            string val = row[colIdx];
            switch (designEntity) {
                case DesignEntity::VARIABLE: {
                    if (!this->is_number(val)) {
                        filteredTable.appendRow(row);
                    }
                    break;
                }
                case DesignEntity::CONSTANT: {
                    if (this->is_number(val)) {
                        filteredTable.appendRow(row);
                    }
                    break;
                }
                case DesignEntity::PROCEDURE: {
                    //todo: implement procedure filter
                    break;
                }
                default: {//stmt types
                    if (!this->is_number(val)) {
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

    bool DataPreprocessor::is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it)) ++it;
        return !s.empty() && it == s.end();
    }

    DesignEntity DataPreprocessor::getDesignEntity(Synonym synonym, shared_ptr<vector<Declaration>> declarations) {
        for (auto d: *declarations) {
            if (synonym.synonym == d.getSynonym().synonym) {
                return d.getDesignEntity();
            }
        }
        return static_cast<DesignEntity>(NULL);
    }
} // QE