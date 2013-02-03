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
	vector<QueryTarget> qTargetList;
	map<int, map<int, vector<int>>> dependencymap;
	
	bool evaluateQuery();
	bool evaluateClause(QueryClause);
	bool evaluateRelationClause(QueryClause);
	bool evaluateWithClause(QueryClause);

	bool getVectors(vector<int>*, vector<int>*, int, int, int*);
	bool getVector(vector<int>*, int, int*);
	bool intersect(vector<int>*, vector<int>*, int, int, int);
	bool intersectDependencyMap(int, int, vector<int>*);
	bool intersectDependencyMapPair(int, int, vector<int>*, int, vector<int>*);
	bool cartesianProduct(vector<int>*, vector<int>*);
	bool getResult(list<string>&);

	vector<int> removeDuplicates(vector<int>);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);
	
	bool evaluate(vector<QueryClause>, vector<QueryVariable>, vector<QueryTarget>, list<string>&);
};

#endif