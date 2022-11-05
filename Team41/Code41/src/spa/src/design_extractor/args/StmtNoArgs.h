//
// Created by Aaron on 5/10/22.
//

#ifndef SPA_STMTNOARGS_H
#define SPA_STMTNOARGS_H

class StmtNoArgs {
 public:
  StmtNoArgs();
  [[nodiscard]] int getStartStmtNo() const;
  [[nodiscard]] int getEndStmtNo() const;

  void setStartStmtNo(int start);
  void setEndStmtNo(int end);
  void setStartAndEndStmtNo(int start, int end);

  [[nodiscard]] bool startAndEndExists() const;
  [[nodiscard]] bool startExistsOnly() const;
  [[nodiscard]] bool endExistsOnly() const;

 private:
  int startStmtNo = 0;
  int endStmtNo = 0;
};

#endif  // SPA_STMTNOARGS_H
