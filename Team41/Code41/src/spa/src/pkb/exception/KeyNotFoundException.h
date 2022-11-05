//
// Created by hongshu wang on 9/9/22.
//

#ifndef SPA_KEYNOTFOUNDEXCEPTION_H
#define SPA_KEYNOTFOUNDEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class KeyNotFoundException : public exception {
 public:
  explicit KeyNotFoundException(const string& columnName, const string& key);
  char* what();

 private:
  string message;
};

#endif  // SPA_KEYNOTFOUNDEXCEPTION_H
