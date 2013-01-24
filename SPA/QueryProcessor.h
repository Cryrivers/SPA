#pragma once

#ifndef QUERYPROCESSOR_H
#define QUERYPROCESSOR_H

#include "QueryPreprocessor.h"
#include "QueryEvaluator.h"

#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

class QueryProcessor{
	private:
		QueryPreprocessor qPre;
		QueryEvaluator qEval;
		/*void printTargetVariable(int s);
		void printQueryVariableTable(vector<Query_variable> v);
		void printQueryClauseTable(vector<Query_clause> v);*/
	public:
		QueryProcessor(void);
		~QueryProcessor(void);
		void evaluate(string, list<string>&);
		//int test();
};

#endif