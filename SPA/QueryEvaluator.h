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
	map<int, vector<QueryClause>> qClauseList;
	vector<QueryVariable> qVariableList;
	vector<QueryTarget> qTargetList;
	map<int, map<int, vector<int>>> dependencymap;
	map<int, map<int, vector<string>>> targetmap;
	
	bool evaluateQuery();
	bool evaluateClause(QueryClause);

	bool getVectors(vector<int>*, vector<int>*, int, int, int*);
	bool getVector(vector<int>*, int, int*);
	bool intersect(vector<int>*, vector<int>*, int, int, int);
	bool intersectDependencyMap(int, int, vector<int>*);
	bool intersectDependencyMapPair(int, int, vector<int>*, int, vector<int>*);
	bool cartesianProduct(vector<int>*, vector<int>*);
	bool getResult(list<string>&);
	bool getTarget(QueryTarget);

	vector<int> removeDuplicates(vector<int>);
	int getWithType(int);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);
	
	bool evaluate(map<int, vector<QueryClause>>, vector<QueryVariable>, vector<QueryTarget>, list<string>&);
};

#endif