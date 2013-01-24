#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include "AST.h"
#include "Parser.h"
#include "QueryProcessor.h"

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
 
 private:
	 AST *ast;
	 Parser *parser;
	 QueryProcessor *queryProcessor;
 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
