/*
* TestFollows.cpp
*
*  Created on: 30 Oct 2012
*      Author: ray
*/

#include <cppunit/config/SourcePrefix.h>
#include "stdafx.h"
#include "Parser.h"
#include "PKBController.h"
#include "TestParserPKB.h"
#include <iostream>
#include <string>

void 
	ParserPKBTest::setUp()
{
}


void 
	ParserPKBTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserPKBTest ); // Note 4 

void ParserPKBTest::f0() {
	vector<int> stmt1;
	vector<int> stmt2;
	vector<int> vars;
	int count;
	AST * ast = new AST();
	PKBController* pkb = PKBController::createInstance();
	Parser *parser = new Parser(ast);
	string program = "procedure test1{\na=b+c;\nb=c+d;\nwhile i{\ni=i+1;\nj=j-1;\n}\nz=x+y;\n}";

	parser->preprocessProgram(program);
	parser->parseLine();
	ast->calculateHashcode();
	pkb->completePKB();
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
}