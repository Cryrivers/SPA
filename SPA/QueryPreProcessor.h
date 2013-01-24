#pragma once

#ifndef QUERYPREPROCESSOR_H
#define QUERYPREPROCESSOR_H

#include "PKBController.h"
#include <iostream>
#include <string>
#include <vector>
#define MAX_VAR 10
#define MAX_SUB 10

using namespace std;

class Query_variable{
public:
	enum queryVariableType{
		QV_VARIABLE,	//0
		QV_STMT,		//1
		QV_ASSIGN,		//2
		QV_WHILE,		//3
		QV_STMT_NUMBER,	//4
		QV_CONSTANT,	//5
		QV_BOOLEAN,		//6
		QV_UNDERSCORE,	//7
		QV_CONSTANTVAR	//8
	};
	queryVariableType type; 
	int dependency;
	int countAppear;
	string content;
};

class Query_clause{
public:
	enum queryClauseType{		
		QC_PARENT,		//0
		QC_PARENTT,		//1
		QC_FOLLOWS,		//2
		QC_FOLLOWST,	//3
		QC_PATTERN,		//4
		QC_MODIFIES,	//5
		QC_USES			//6
	};
	queryClauseType relation;
	int index;
	int variable1;
	int variable2;
	string variable3; 
};


class QueryPreprocessor{ 
public:
	//constructor
	QueryPreprocessor(void);
	~QueryPreprocessor(void);
	//parser 
	int parse(string input);
	//returned results
	int getTargetVariable();
	vector<Query_variable> getQueryVariableTable();
	vector<Query_clause> getQueryClauseTable(); 
};

#endif