//
// Created by Aaron on 1/10/22.
//

#ifndef SPA_NEXTSTMTNOARGS_H
#define SPA_NEXTSTMTNOARGS_H


class NextStmtNoArgs {
public:
    int getStartStmtNo() { return startStmtNo; };
    int getEndStmtNo() { return endStmtNo; }

    void setStartStmtNo(int start) {startStmtNo = start; };
    void setEndStmtNo(int end) {endStmtNo = end; };
private:
    int startStmtNo = 0;
    int endStmtNo = 0;
};


#endif //SPA_NEXTSTMTNOARGS_H
