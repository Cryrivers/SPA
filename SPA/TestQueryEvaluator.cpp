
#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "Parser.h"
#include "PKBController.h"
#include "QueryEvaluator.h"
//#include "QueryPreProcessor.h"
#include "TestQueryEvaluator.h"
#include <iostream>
#include <string>

void 
	QueryEvaluatorTest::setUp()
{
}


void 
	QueryEvaluatorTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest ); // Note 4 

void QueryEvaluatorTest::f0() {
	//vector<int> stmt1;
	//vector<int> stmt2;
	//vector<int> vars;
	//int count;
	vector<Query_clause> qcl;
	vector<Query_variable> qvl;
	list<string> r;
	QueryEvaluator qe;
	//AST * ast = new AST();
	//PKBController* pkb = PKBController::createInstance();
	//Parser *parser = new Parser(ast);
	//string program = "procedure qe{\nb=c+3;\nwhile k{\ni=k+1;\nj=k-1;\nwhile w{\nx=2;\nz=x+y;\n}\na=a+111;}\nwhile q{\na=1+9;\n}}";

	// build pkb
	//parser->preprocessProgram(program);
	//parser->parseLine();
	//ast->calculateHashcode();
	//pkb->completePKB();

	// query 1 -> assign a; Select a 
	// result 1 2 4 5 6
	Query_variable var0, var1, var2; 
	Query_clause cl0;

	var0.type= var0.QV_ASSIGN; 
	var0.dependency=-1;
	qvl.push_back(var0);
	string mystr[] = {"1", "2", "4", "5", "6"};

	qe.evaluate(qcl, qvl, 0, r);
	CPPUNIT_ASSERT(r == list<string>(mystr, mystr + 5));

	// query 2 -> assign a; Select a such that Modifies(2,"y")
	// result empty
	r.clear();

	// var0 remains unchanged
	var1.type= var1.QV_STMT_NUMBER; 
	var1.dependency=-1;
	var1.content="2";
	qvl.push_back(var1);

	var2.type= var2.QV_CONSTANTVAR; 
	var2.dependency=-1;
	var2.content="8";
	qvl.push_back(var2);

	cl0.relation = cl0.QC_MODIFIES;
	cl0.variable1 = 1;
	cl0.variable2 = 2;
	qcl.push_back(cl0);

	qe.evaluate(qcl, qvl, 0, r);
	CPPUNIT_ASSERT(r == list<string>());

	/*
	pkb->follows(&stmt1, &stmt2, 3);
	count = 0;
	for(int i = 0; i < stmt1.size(); i++){
	if(stmt1.at(i) == 1) {CPPUNIT_ASSERT(stmt2.at(i) == 2);count++;}
	if(stmt1.at(i) == 2) {CPPUNIT_ASSERT(stmt2.at(i) == 3);count++;}
	if(stmt1.at(i) == 3) {CPPUNIT_ASSERT(stmt2.at(i) == 6);count++;}
	if(stmt1.at(i) == 4) {CPPUNIT_ASSERT(stmt2.at(i) == 5);count++;}
	}
	CPPUNIT_ASSERT(stmt1.size() == 4);
	CPPUNIT_ASSERT(stmt2.size() == 4);
	CPPUNIT_ASSERT(count == 4);

	stmt1.clear();
	stmt2.clear();
	count = 0;
	pkb->parent(&stmt1, &stmt2, 3);
	for(int i = 0; i < stmt1.size(); i++){
	if(stmt2.at(i) == 4) {CPPUNIT_ASSERT(stmt1.at(i) == 3);count++;}
	if(stmt2.at(i) == 5) {CPPUNIT_ASSERT(stmt1.at(i) == 3);count++;}
	}
	CPPUNIT_ASSERT(count == 2);
	CPPUNIT_ASSERT(stmt2.size() == 2);
	CPPUNIT_ASSERT(stmt1.size() == 2);

	stmt1.clear();
	stmt2.clear();
	count = 0;
	pkb->modifies(&stmt1, &vars, 3);
	for(int i = 0; i < stmt1.size(); i++){
	if(stmt1.at(i) == 1) {CPPUNIT_ASSERT(vars.at(i) == 0);count++;}
	if(stmt1.at(i) == 2) {CPPUNIT_ASSERT(vars.at(i) == 1);count++;}
	if(stmt1.at(i) == 4) {CPPUNIT_ASSERT(vars.at(i) == 4);count++;}
	if(stmt1.at(i) == 5) {CPPUNIT_ASSERT(vars.at(i) == 5);count++;}
	if(stmt1.at(i) == 6) {CPPUNIT_ASSERT(vars.at(i) == 6);count++;}
	if(stmt1.at(i) == 3) {CPPUNIT_ASSERT(vars.at(i) == 4 || vars.at(i) == 5);count++;}
	}
	CPPUNIT_ASSERT(count == 7);
	CPPUNIT_ASSERT(stmt1.size() == 7);
	CPPUNIT_ASSERT(vars.size() == 7);

	stmt1.clear();
	vars.clear();
	count = 0;
	pkb->uses(&stmt1, &vars, 3);
	for(int i = 0; i < stmt1.size(); i++){
	if(stmt1.at(i) == 1) {CPPUNIT_ASSERT(vars.at(i) == 1||vars.at(i) == 2);count++;}
	if(stmt1.at(i) == 2) {CPPUNIT_ASSERT(vars.at(i) == 2||vars.at(i) == 3);count++;}
	if(stmt1.at(i) == 4) {CPPUNIT_ASSERT(vars.at(i) == 4);count++;}
	if(stmt1.at(i) == 5) {CPPUNIT_ASSERT(vars.at(i) == 5);count++;}
	if(stmt1.at(i) == 6) {CPPUNIT_ASSERT(vars.at(i) == 7||vars.at(i) == 8);count++;}
	if(stmt1.at(i) == 3) {CPPUNIT_ASSERT(vars.at(i) == 5||vars.at(i) == 4);count++;}
	}
	cout<<stmt1.size()<<endl;
	CPPUNIT_ASSERT(count == 10);
	CPPUNIT_ASSERT(stmt1.size() == 10);
	CPPUNIT_ASSERT(vars.size() ==10);
	*/
}