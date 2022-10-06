//
// Created by Aaron on 5/10/22.
//

#ifndef SPA_STMTNOARGS_H
#define SPA_STMTNOARGS_H


class StmtNoArgs {
public:
    StmtNoArgs();
    int getStartStmtNo() { return startStmtNo; };
    int getEndStmtNo() { return endStmtNo; }

    void setStartStmtNo(int start) {startStmtNo = start; };
    void setEndStmtNo(int end) {endStmtNo = end; };
    void setStartAndEndStmtNo(int start, int end) {
        startStmtNo = start;
        endStmtNo = end;
    }

    bool startAndEndExists();
    bool startExistsOnly();
    bool endExistsOnly();
    bool noArgsProvided();

private:
    int startStmtNo = 0;
    int endStmtNo = 0;
};


#endif //SPA_STMTNOARGS_H
