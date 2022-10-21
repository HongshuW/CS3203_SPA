//
// Created by hongshu wang on 9/9/22.
//

#include "KeyNotFoundException.h"

KeyNotFoundException::KeyNotFoundException(string columnName, string key) {
  message = "Key " + key + " is not found in column " + columnName + ".";
}

char* KeyNotFoundException::what() {
  char* output = new char[message.size()];
  strcpy(output, message.c_str());
  return output;
}
