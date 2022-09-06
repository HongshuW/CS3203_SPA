//
// Created by Nafour on 5/9/22.
//

#include "DataPreprocessor.h"

#include <utility>
#include <string>
using namespace std;
using namespace QB;
namespace QE {
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

    Table DataPreprocessor::getTableByRelation(SuchThatClause suchThatClause) {
        return this->getRelationTable(suchThatClause.arg1, suchThatClause.arg2, suchThatClause.relationType);
    }

    DataPreprocessor::DataPreprocessor(shared_ptr<FakeDataRetriever> dataRetriever) {
        this->dataRetriever = std::move(dataRetriever);
    }

    Table DataPreprocessor::getRelationTable(Ref ref1, Ref ref2, RelationType relationType) {
        //RefType can be synonym, integer, underscore or string
        RefType ref1Type = getRefType(ref1);
        RefType ref2Type = getRefType(ref2);
        Table relationTable = this->dataRetriever->getTableByRelationType(relationType);
        //TODO::add declaration array dependency (not in synonym)
        //TODO: check design entity of synonym
        if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::SYNONYM) {
            //1
            //assuming the table has two cols
            vector<string> newHeaders = vector<string>{get<Synonym>(ref1).synonym, get<Synonym>(ref2).synonym};
            relationTable.renameHeader(newHeaders);
        } else if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::INTEGER) {
            //2

            vector<string> newHeaders = vector<string>{get<Synonym>(ref1).synonym, "$integer_col"};
            relationTable.renameHeader(newHeaders);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref2Val));

        } else if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::IDENT) {
            //3

            vector<string> newHeaders = vector<string>{get<Synonym>(ref1).synonym, "$string_col"};
            relationTable.renameHeader(newHeaders);
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", ref2Val.identStr);

        } else if (ref1Type == RefType::SYNONYM && ref2Type == QB::RefType::UNDERSCORE) {
            //4

            vector<string> newHeaders = vector<string>{get<Synonym>(ref1).synonym, "$underscore_col"};
            relationTable.renameHeader(newHeaders);

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::SYNONYM) {
            //5

            vector<string> newHeaders = vector<string>{"$integer_col", get<Synonym>(ref2).synonym};
            relationTable.renameHeader(newHeaders);
            int ref1Val = get<int>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref1Val));

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::INTEGER) {
            //6

            vector<string> newHeaders = vector<string>{"$integer_col", "$integer_col_2"};
            relationTable.renameHeader(newHeaders);
            int ref1Val = get<int>(ref1);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref1Val));
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col_2", to_string(ref2Val));

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::UNDERSCORE) {
            //7

            vector<string> newHeaders = vector<string>{"$integer_col", "$underscore_col"};
            relationTable.renameHeader(newHeaders);
            int ref1Val = get<int>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref1Val));

        } else if (ref1Type == RefType::INTEGER && ref2Type == RefType::IDENT) {
            //8

            vector<string> newHeaders = vector<string>{"$integer_col", "$string_col"};
            relationTable.renameHeader(newHeaders);
            int ref1Val = get<int>(ref1);
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref1Val));
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", ref2Val.identStr);

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::SYNONYM) {
            //9

            vector<string> newHeaders = vector<string>{"$string_col", get<Synonym>(ref2).synonym};
            relationTable.renameHeader(newHeaders);
            Ident ref1Val = get<Ident>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::INTEGER) {
            //10

            vector<string> newHeaders = vector<string>{"$string_col", "$integer_col"};
            relationTable.renameHeader(newHeaders);
            Ident ref1Val = get<Ident>(ref1);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref2Val));

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::UNDERSCORE) {
            //11

            vector<string> newHeaders = vector<string>{"$string_col", "$underscore_col"};
            relationTable.renameHeader(newHeaders);
            Ident ref1Val = get<Ident>(ref1);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));

        } else if (ref1Type == RefType::IDENT && ref2Type == RefType::IDENT) {
            //12
            vector<string> newHeaders = vector<string>{"$string_col", "$string_col_2"};
            relationTable.renameHeader(newHeaders);
            Ident ref1Val = get<Ident>(ref1);
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col_2", (ref2Val.identStr));

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::SYNONYM) {
            //13

            vector<string> newHeaders = vector<string>{"$underscore_col", get<Synonym>(ref2).synonym};
            relationTable.renameHeader(newHeaders);

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::INTEGER) {
            //14

            vector<string> newHeaders = vector<string>{"$underscore_col", "$integer_col"};
            relationTable.renameHeader(newHeaders);
            Ident ref1Val = get<Ident>(ref1);
            int ref2Val = get<int>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref1Val.identStr));
            relationTable = this->filerTableByColumnValue(relationTable, "$integer_col", to_string(ref2Val));

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::IDENT) {
            //15
            vector<string> newHeaders = vector<string>{"$underscore_col", "$string_col"};
            relationTable.renameHeader(newHeaders);
            Ident ref2Val = get<Ident>(ref2);
            relationTable = this->filerTableByColumnValue(relationTable, "$string_col", (ref2Val.identStr));

        } else if (ref1Type == RefType::UNDERSCORE && ref2Type == RefType::UNDERSCORE) {
            //16
            vector<string> newHeaders = vector<string>{"$underscore_col", "$underscore_col_2"};
            relationTable.renameHeader(newHeaders);

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
} // QE