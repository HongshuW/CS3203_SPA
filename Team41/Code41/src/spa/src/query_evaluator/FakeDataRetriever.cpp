//
// Created by Nafour on 5/9/22.
//

#include "FakeDataRetriever.h"

namespace QE {
    Table FakeDataRetriever::getTableByRelationType(RelationType relationType) {
        Table followsTable = Table();
        followsTable.renameHeader({"$followed", "$following"});
        followsTable.appendRow({"1", "2"});
        followsTable.appendRow({"2", "3"});
        followsTable.appendRow({"3", "4"});


        return followsTable;
    }

    DesignEntity FakeDataRetriever::getDesignEntityOfStmt(int line_no) {
        if (line_no > 1 && line_no < 4) return QB::DesignEntity::ASSIGN;
        return DesignEntity::IF;
    }
} // QE