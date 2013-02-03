#pragma once

#ifndef QUERYEVALUATOR_H
#define QUERYEVALUATOR_H

#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>

#include "QueryPreprocessor.h"
#include "PKBController.h"
using namespace std;

class QueryEvaluator
{
	PKBController* pkb;
	vector<QueryClause> qClauseList;
	vector<QueryVariable> qVariableList;
	map<int, vector<int>> dependencymap;
	
	bool optimise();
	bool evaluateQuery();
	bool evaluateClause(QueryClause);
	
	bool getVectors(vector<int>*, vector<int>*, int, int, int*);
	bool getOtherVector(vector<int>*, int, int*, vector<int>*);
	bool getVector(vector<int>*, int, int*);
	bool intersect(vector<int>*, vector<int>*, int, int, int);
	bool intersectDependencyMap(vector<int>*, int);
	bool cartesianProduct(vector<int>*, vector<int>*);
	bool getResult(list<string>&, int);

	vector<int> removeDuplicates(vector<int>);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);
	
	bool evaluate(vector<QueryClause>, vector<QueryVariable>, int, list<string>&);
};

#endif