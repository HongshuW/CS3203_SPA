#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <iostream>
#include <list>
#include <string>

#include "spa_manager/SPAManager.h"

// include your other headers here
#include "AbstractWrapper.h"
#include "pkb/PKBStorage.h"

class TestWrapper : public AbstractWrapper {
 public:
  // default constructor
  TestWrapper();

  // destructor
  ~TestWrapper();

  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);

  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

 private:
  shared_ptr<SPAManager> spaManager;
};

#endif
