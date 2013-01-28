#include "stdafx.h"
#include <cppunit/config/SourcePrefix.h>
#include "QueryPreprocessor.h"
#include "TestQueryPreprocessor.h"
#include <iostream>
#include <string>

using namespace std;

void 
	QueryPreprocessorTest::setUp(){
}


void 
	QueryPreprocessorTest::tearDown(){
}

//Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryPreprocessorTest ); 

void QueryPreprocessorTest::f0() {

	QueryPreprocessor *qp = new QueryPreprocessor();
	//qp->flagerr=1;
	int showerr=1;
	int res = 0;
	string query = "";

	query += "assign a;\n";
	query += "Select a such that Modifies(a, \"x\");";  

	int parser_result = qp->parse(query);
	CPPUNIT_ASSERT(parser_result == 0);

	int query_target_result = qp->getTargetVariable(); 
	CPPUNIT_ASSERT(query_target_result == 0);

	vector<Query_variable> var_table_result = qp->getQueryVariableTable();
	CPPUNIT_ASSERT(var_table_result[1].content == "7");
	CPPUNIT_ASSERT(var_table_result.size() == 2);

	vector<Query_clause> cla_table_result = qp->getQueryClauseTable();
	CPPUNIT_ASSERT(cla_table_result.size() == 1);

	query.clear(); 

}