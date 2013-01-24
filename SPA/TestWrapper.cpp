#include "TestWrapper.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	ast = new AST();
	parser = new Parser(ast);
	queryProcessor = new QueryProcessor();
}


TestWrapper::~TestWrapper()
{
	delete(ast);
	delete(parser);
	delete(queryProcessor);
}


// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
    // ...rest of your code...
	string program, line;
	program = "";
	ifstream myfile(filename);
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline(myfile,line);
			program += line;
			program += "\n";
		}
		myfile.close();
	}
	
	parser->parse(program);

}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  //cout << query << endl;
	queryProcessor->evaluate(query,results);
}
