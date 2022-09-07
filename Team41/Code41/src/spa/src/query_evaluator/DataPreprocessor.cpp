//
// Created by Nafour on 5/9/22.
//

#include "DataPreprocessor.h"

#include <utility>
#include <string>
using namespace std;
using namespace QB;
namespace QE {
    unordered_map<RefType, string> refTypeToColNameMap({
                                                               {RefType::INTEGER, "$integer_col"},
                                                               {RefType::IDENT, "$ident_col"},
                                                               {RefType::UNDERSCORE, "$underscore_col"}
    });
    Table DataPreprocessor::getAllByDesignEntity(DesignEntity designEntity) {
        switch (designEntity) {

            case DesignEntity::STMT: {
                break;
            }
            case DesignEntity::ASSIGN: {
                break;
            }
            case DesignEntity::CONSTANT: {
                break;
            }
            case DesignEntity::CALL: {
                break;
            }
            case DesignEntity::PRINT: {
                break;
            }
            case DesignEntity::IF: {
                break;
            }
            case DesignEntity::WHILE: {
                break;
            }
            case DesignEntity::PROCEDURE: {
                break;
            }
            case DesignEntity::READ: {
                break;
            }
            case DesignEntity::VARIABLE: {
                return this->dataRetriever->getVariables();
            }
        }
        return Table();
    }

    Table
    DataPreprocessor::getTableByRelation(SuchThatClause suchThatClause) {
        return this->getRelationTable(suchThatClause.arg1, suchThatClause.arg2, suchThatClause.relationType);
    }

    DataPreprocessor::DataPreprocessor(shared_ptr<FakeDataRetriever> dataRetriever) {
        this->dataRetriever = std::move(dataRetriever);
    }

    Table DataPreprocessor::getRelationTable(Ref ref1, Ref ref2, RelationType relationType) {
        //RefType can be synonym, integer, underscore or string
        RefType ref1Type = getRefType(ref1);
        RefType ref2Type = getRefType(ref2);
        string col1Name = ref1Type == QB::RefType::SYNONYM ? get<Synonym>(ref1).synonym : refTypeToColNameMap.at(ref1Type);
        string col2Name = ref2Type == QB::RefType::SYNONYM ? get<Synonym>(ref2).synonym : refTypeToColNameMap.at(ref2Type);
        Table relationTable = this->dataRetriever->getTableByRelationType(relationType);
        vector<string> newHeaders = vector<string>{col1Name, col2Name};
        relationTable.renameHeader(newHeaders);
        //TODO::add declaration array dependency (not in synonym)
        //TODO: check design entity of synonym
        if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::SYNONYM) {
            //1
            //assuming the table has two cols
            Synonym syn1 = get<Synonym>(ref1);
            Synonym syn2 = get<Synonym>(ref2);
            vector<string> newHeaders = vector<string>{syn1.synonym, syn2.synonym};
            relationTable.renameHeader(newHeaders);
            relationTable = this->filerTableByDesignEntity(relationTable,
                                                           syn1.synonym,
                                                           this->getDesignEntity(syn1,
                                                                                 declarations));
            relationTable = this->filerTableByDesignEntity(relationTable,
                                                           syn2.synonym,
                                                           this->getDesignEntity(syn2,
                                                                                 declarations));

        } else if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::INTEGER) {
            //2

            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, QE::refTypeToColNameMap.at(ref2Type), to_string(ref2Val));

        } else if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::IDENT) {
            //3

            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", ref2Val.identStr);

        } else if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::UNDERSCORE) {
            //4

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::SYNONYM) {
            //5

            int ref1Val = get<int>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, QE::refTypeToColNameMap.at(ref1Type), to_string(ref1Val));

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::INTEGER) {
            //6

            int ref1Val = get<int>(ref1);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, QE::refTypeToColNameMap.at(ref1Type), to_string(ref1Val));
            relationTable = this->filerTableByColumnValue(relationTable, QE::refTypeToColNameMap.at(ref2Type), to_string(ref2Val));

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::UNDERSCORE) {
            //7

            int ref1Val = get<int>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref1Val));

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::IDENT) {
            //8

            int ref1Val = get<int>(ref1);
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref1Val));
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", ref2Val.identStr);

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::SYNONYM) {
            //9

            Ident ref1Val = get<Ident>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::INTEGER) {
            //10

            Ident ref1Val = get<Ident>(ref1);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref2Val));

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::UNDERSCORE) {
            //11

            Ident ref1Val = get<Ident>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::IDENT) {
            //12
            Ident ref1Val = get<Ident>(ref1);
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col_2", (ref2Val.identStr));

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::SYNONYM) {
            //13


        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::INTEGER) {
            //14

            Ident ref1Val = get<Ident>(ref1);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref2Val));

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::IDENT) {
            //15
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref2Val.identStr));

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::UNDERSCORE) {
            //16

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
                default: {
                    if (!this->is_number(val)) {
                        cout << "Error: stmt line is not a number" << endl;
                    }
                    if (designEntity == this->dataRetriever->getDesignEntityOfStmt(stoi(val))) {
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