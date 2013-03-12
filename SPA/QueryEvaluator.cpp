/**
 * \file	QueryEvaluator.cpp
 * \class	QueryEvaluator
 * \brief	Evaluates PQL query given a query tree
 * \author	Ong Zhen Ming
 */
#include "stdafx.h"
#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(void)
{
	pkb = PKBController::createInstance();
}


QueryEvaluator::~QueryEvaluator(void)
{
}

/**
 * \fn		QueryEvaluator::evaluate(vector<QueryClause> qcl, vector<QueryVariable> qvl, vector<QueryTarget> qtl, list<string>& r)
 * \brief	Public method to call for evaluating query 
 * \param [in]	qcl: list of query clause; 
 *				qvl: list of query variables; 
 *				qtl: list of query target.
 * \param [out]	r: results are stored in this list.
 * \return 	TRUE if query evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluate(map<int, vector<QueryClause>> qcl, vector<QueryVariable> qvl, vector<QueryTarget> qtl, list<string>& r)
{
	// prepare new query, initialise data members 
	qClauseList = qcl;
	qVariableList = qvl;
	qTargetList = qtl;
	dependencymap.clear();

	//optimise();
	
	if (!evaluateQuery()) 
		return false; // at least one query clause failed

	if (!getResult(r))
		return false; // result is empty

	return true;
}

/*
bool QueryEvaluator::optimise()
{
	return true;
}
*/	

/**
 * \fn		QueryEvaluator::evaluateQuery()
 * \brief	Evaluates query by querying clauses one at a time  
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluateQuery() 
{
	
	for (map<int, vector<QueryClause>>::iterator mit = qClauseList.begin(); mit != qClauseList.end(); ++mit) {
		for (vector<QueryClause>::iterator vit = (*mit).second.begin(); vit != (*mit).second.end(); ++vit) {
			if (!evaluateClause(*vit)) {
				return false; // query clause evaluated to false
			}
		}
	}
	return true;
}

/**
 * \fn		QueryEvaluator::evaluateClause(QueryClause qc)
 * \brief	Evaluates a given query clause, supports relation type Parent, ParentT, Follows, FollowsT, ModifiesS, ModifiesP, UsesS, UsesP, Calls, CallsT, Next NextT Pattern, With.  
 * \param [in]	qc: query clause. 
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluateClause(QueryClause qc) {

	vector<int> vectorA, vectorB;
	int arg;

	if (!getVectors(&vectorA, &vectorB, qc.variable1, qc.variable2, &arg))
		return false; // unable to get vectors

	switch (qc.relationType) {

		case RT_PARENT:
			
			if (!pkb->parent(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_PARENTT:

			if (!pkb->parentStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_FOLLOWS:
			
			if (!pkb->follows(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_FOLLOWST:

			if (!pkb->followsStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_MODIFIESS:
			
			if (!pkb->modifies(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case RT_MODIFIESP:

			if (!pkb->modifiesP(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_USESS:
			
			if (!pkb->uses(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case RT_USESP:
			
			if (!pkb->usesP(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case RT_CALLS:
			
			if (!pkb->calls(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case RT_CALLST:
		
			if (!pkb->callsStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case RT_NEXT:
			
			if (!pkb->next(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
			
		case RT_NEXTT:
			
			if (!pkb->nextStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
			
		case RT_AFFECTS:
		case RT_AFFECTST:
			
			break;
			
		case CT_PATTERN:
			
			if (!pkb->pattern(&vectorA, &vectorB, qc.variable3, qc.patternType, arg))
				return false; // can't find relation
			break;
		
		case CT_WITH:

			if (!pkb->with(&vectorA, &vectorB, getWithType(qc.attribute1), getWithType(qc.attribute2), arg))
				return false;
			break;

	}
	
	if (!intersect(&vectorA, &vectorB, qc.variable1, qc.variable2, arg))
		return false; // intersection is empty

	return true;
}

/**
 * \fn		QueryEvaluator::getVectors(vector<int>* vecA, vector<int>* vecB, int a, int b, int *arg)
 * \brief	Retrieves 2 vectors and dependency corresponding to the 2 variables a, b for querying clauses.  
 * \param [in]	a: 1st query variable index;
 *				b: 2nd query variable index.
 * \param [out]	vecA: vector corresponding to a;
 *				vecB: vector corresponding to b;
 *				arg:  the dependency of a and b. 
 * \return 	TRUE if vectors could be obtained, FALSE otherwise
 */
bool QueryEvaluator::getVectors(vector<int>* vecA, vector<int>* vecB, int a, int b, int *arg) {
	
	int argA, argB, depA, depB;
	
	depA = qVariableList.at(a).dependency; 
	depB = qVariableList.at(b).dependency;
	argA = 0;
	argB = 0;

	// checks if a and b exists in dependency map
	if (depA >= 0 && dependencymap.count(depA) == 1)
		argA = dependencymap[depA].count(a);
	if (depB >= 0 && dependencymap.count(depB) == 1)
		argB = dependencymap[depB].count(b);

	*arg = 2*argA + argB;

	switch (*arg) {

		case 3: // both a and b are found in dependency map
			
			*vecA = dependencymap[depA][a]; // get vector a
			*vecB = dependencymap[depB][b]; // get vector b
			
			// no need for cartesian product as both must belong to the same dependency
			return true;
		
		case 2: // only a is found in dependency map

			*vecA = removeDuplicates(dependencymap[depA][a]); // get vector a
			if (!getVector(vecB, b, &argB)) // get vector b 
				return false;
			if (argB == 1 && !vecB->empty() && !cartesianProduct(vecA, vecB)) // perform cartesian product if vecB is dependent and not empty
				return false;
			*arg += argB;
			return true;

		case 1: // only b is found in dependency map
			
			*vecB = removeDuplicates(dependencymap[depB][b]); // get vector b
			if (!getVector(vecA, a, &argA)) // get vector a
				return false;
			if (argA == 1 && !vecA->empty() && !cartesianProduct(vecA, vecB)) // perform cartesian product if vecA is dependent and not empty
				return false;
			*arg += 2*argA;
			return true;

		case 0: // both a and b are not found in dependency map
			
			if (!getVector(vecA, a, &argA)) // get vector a
				return false;
			if (!getVector(vecB, b, &argB)) // get vector b
				return false;
			*arg = 2*argA + argB;
			// if arg = 3 (both dependent) and vecA and vecB are both not empty, do cartesian product 
			if (*arg == 3 && !vecA->empty() && !vecB->empty() && !cartesianProduct(vecA, vecB))
				return false;
			return true;
	
	}

}
 
/**
 * \fn		QueryEvaluator::getVectorsgetVector(vector<int>* vec, int v, int *arg)
 * \brief	Fills up vector vec given query variable v. Currently, v does not appear in dependency map.  
 * \param [in]	v: query variable index.
 * \param [out]	vec: vector corresponding to v;
 *				arg:  the dependency of v. 
 * \return 	TRUE if vector could be obtained, FALSE otherwise
 */
 bool QueryEvaluator::getVector(vector<int>* vec, int v, int *arg) {
	
	QueryVariable qv = qVariableList.at(v);

	switch (qv.variableType) {
		
		case DT_ASSIGN: 	
			
			if (!pkb->getAllAssignment(vec))
				return false;
			break;
		
		case DT_WHILE: 		
			
			if (!pkb->getAllWhile(vec))
				return false;
			break;
		
		case DT_IF:			
		
			if (!pkb->getAllIf(vec))
				return false;
			break;

		case DT_CALL: 		

			if (!pkb->getAllCall(vec))
				return false;
			break;
		
		case DT_CONSTANT:	
			
			if (!pkb->getAllConstant(vec))
				return false;
			break;

		// no restriction on the following types, leave vector empty
		case DT_STMT:
		case DT_PROGLINE:
		case DT_VARIABLE: 	
		case DT_PROCEDURE:	
		case DT_UNDERSCORE:		
			break;
			
		// invalid types, should not appear in relations
		case DT_STMTLST: 	
		case DT_BOOLEAN:	
			break;
			
		// Known Constants
		case KT_STMT_NUM:			
		case KT_KNOWN_VARIABLE:		
		case KT_KNOWN_PROCEDURE:	
		case KT_KNOWN_CONSTANT:	
			
			vec->push_back(qv.content);
			break;

	}
	
	if (qv.dependency >= 0) 
		*arg = 1;
	else 
		*arg = 0;

	return true;

}

/**
 * \fn		QueryEvaluator::intersect(vector<int>* vecA, vector<int>* vecB, int a, int b, int arg)
 * \brief	Wrapper for performing intersection with the dependency map.  
 * \param [in]	vecA: vector corresponding to a;
 *				vecB: vector corresponding to b;
 *				a: 1st query variable index;
 *				b: 2nd query variable index;
 *				arg:  the dependency of a and b. 
 * \return 	TRUE if intersection is successful, FALSE otherwise
 */
bool QueryEvaluator::intersect(vector<int>* vecA, vector<int>* vecB, int a, int b, int arg) {
	
	int dep;

	switch (arg) {
		case 3: // store both vecA and vecB

			dep = qVariableList.at(a).dependency; // a and b must be of the same dependency
			
			if (dependencymap.count(dep) == 1) { 
			// dependency map already exists
				
				if (!intersectDependencyMapPair(dep, a, vecA, b, vecB))
					return false; // intersection is empty set

			} else { 
			// dependency map is empty
				
				// store both vectors
				dependencymap[dep][a] = *vecA;
				dependencymap[dep][b] = *vecB;
			
			}
			
			break;

		case 2: // store only vecA
		
			dep = qVariableList.at(a).dependency;
			
			if (dependencymap.count(dep) == 1) { 
			// dependency map already exists
				
				if (!intersectDependencyMap(dep, a, vecA))
					return false; // intersection is empty set

			} else { 
			// dependency map is empty
				
				// store vecA
				dependencymap[dep][a] = *vecA;

			}
			
			break;

		case 1: // store only vecB
			
			dep = qVariableList.at(b).dependency;
			
			if (dependencymap.count(dep) == 1) { 
			// dependency map already exists
				
				if (!intersectDependencyMap(dep, b, vecB))
					return false; // intersection is empty set

			} else { 
			// dependency map is empty
				
				// store vecB
				dependencymap[dep][b] = *vecB;

			}
			
			break;
			
		case 0: // do not need to store
			break;
	}
	
	return true;
}

/**
 * \fn		QueryEvaluator::intersectDependencyMap(int dep, int v, vector<int>* vec)
 * \brief	Perform intersection with the dependency map for single variable. Map is not empty. Incoming variable either already exists or is new. 
 * \param [in]	dep: dependency of v;
 *				v: query variable index;
 *				vec: vector corresponding to v. 
 * \return 	TRUE if intersection is successful, FALSE otherwise
 */
bool QueryEvaluator::intersectDependencyMap(int dep, int v, vector<int>* vec) {
	
	int size, index;
	
	if (dependencymap[dep].count(v) == 1) {
	// variable already exists, remove rows from map
	
		size = dependencymap[dep][v].size();
		index = 0;
		
		for (int i = 0; i < size; i++) {
		
			bool match = false;
			int x = dependencymap[dep][v][index];
		
			for (int j = 0; j < vec->size(); j++) {
				if (x == vec->at(j)) {
				// intersection success
					match = true;
					index++;
					break;
				}
			}

			if (!match) {
			// found no match, remove elements
				for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
					dependencymap[dep][(*it).first].erase(dependencymap[dep][(*it).first].begin() + index);
				}
			}
		}

		if (dependencymap[dep][v].empty()) // intersection is null
			return false;

	} else {
	// variable is new, do cartesian product
		
		size = dependencymap[dep].begin()->second.size();
			
		for (int j = 0; j < vec->size(); j++) {
			
			int x = vec->at(j); 

			for (int i = 0; i < size; i++) {
				
				dependencymap[dep][v].push_back(x);
				
				if (j != 0) { // do not push for first add
					
					for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
						if ((*it).first != v) // skip variable v
							dependencymap[dep][(*it).first].push_back(dependencymap[dep][(*it).first][i]);
					}
		
				}
			}
		}
	}
	
	return true;

}

/**
 * \fn		QueryEvaluator::intersectDependencyMapPair(int dep, int a, vector<int>* vecA, int b, vector<int>* vecB)
 * \brief	Perform intersection with the dependency map for variable pair. Map is not empty. Incoming variables either already exist or are new. 
 * \param [in]	dep: dependency of a and b;
 *				a: 1st query variable index;
 *				vecA: vector corresponding to a;
 *				b: 2nd query variable index;
 *				vecB: vector corresponding to b. 
 * \return 	TRUE if intersection is successful, FALSE otherwise
 */
bool QueryEvaluator::intersectDependencyMapPair(int dep, int a, vector<int>* vecA, int b, vector<int>* vecB) {
	
	int size, index;
	
	if (dependencymap[dep].count(a) == 1 && dependencymap[dep].count(b) == 1) {
	// both variable already exists, remove rows from map
	
		size = dependencymap[dep][a].size();
		index = 0;
		
		for (int i = 0; i < size; i++) {
		
			bool match = false;
			int x = dependencymap[dep][a][index];
			int y = dependencymap[dep][b][index];
		
			for (int j = 0; j < vecA->size(); j++) {
				if (x == vecA->at(j) && y == vecB->at(j)) {
				// intersection success
					match = true;
					index++;
					break;
				}
			}

			if (!match) {
			// found no match, remove elements
				for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
					dependencymap[dep][(*it).first].erase(dependencymap[dep][(*it).first].begin() + index);
				}
			}
		}

		if (dependencymap[dep][a].empty()) // intersection is null
			return false;

	} else if (dependencymap[dep].count(a) == 1) {
	// variable a exists in map
		
		size = dependencymap[dep][a].size();
		
		for (int i = 0; i < size; i++) {
		
			bool match = false;
			int x = dependencymap[dep][a][0];
			vector<int> y;
		
			for (int j = 0; j < vecA->size(); j++) {
				if (x == vecA->at(j)) {
				// intersection success
					match = true;
					y.push_back(vecB->at(j));			
				}
			}

			if (match) {
			// match, duplicate first elements y.size times
				
				for (int k = 0; k < y.size(); k++) {
					
					for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
						if ((*it).first != b) // skip variable b
							dependencymap[dep][(*it).first].push_back(dependencymap[dep][(*it).first][0]); //always push first element
					}

					dependencymap[dep][b].push_back(y[k]);
				}
			} 

			// always remove first element at the end of each iteration
			for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
				if ((*it).first != b) // skip variable b
					dependencymap[dep][(*it).first].erase(dependencymap[dep][(*it).first].begin());
			}
		}

		if (dependencymap[dep][a].empty()) // intersection is null
			return false;

	} else if (dependencymap[dep].count(b) == 1) {
	// variable b exists in map

		size = dependencymap[dep][b].size();
		
		for (int i = 0; i < size; i++) {
		
			bool match = false;
			int x = dependencymap[dep][b][0];
			vector<int> y;
		
			for (int j = 0; j < vecB->size(); j++) {
				if (x == vecB->at(j)) {
				// intersection success
					match = true;
					y.push_back(vecA->at(j));			
				}
			}

			if (match) {
			// match, duplicate first elements y.size times
				
				for (int k = 0; k < y.size(); k++) {
					
					for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
						if ((*it).first != a) // skip variable a
							dependencymap[dep][(*it).first].push_back(dependencymap[dep][(*it).first][0]); //always push first element
					}

					dependencymap[dep][a].push_back(y[k]);
				}
			} 

			// always remove first element at the end of each iteration
			for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
				if ((*it).first != a) // skip variable a
					dependencymap[dep][(*it).first].erase(dependencymap[dep][(*it).first].begin());
			}
		}

		if (dependencymap[dep][b].empty()) // intersection is null
			return false;
	
	} else {
	// both variables are new, do cartesian product
		
		size = dependencymap[dep].begin()->second.size();
			
		for (int j = 0; j < vecA->size(); j++) {
			
			int x = vecA->at(j); 
			int y = vecB->at(j); 

			for (int i = 0; i < size; i++) {
				
				dependencymap[dep][a].push_back(x);
				dependencymap[dep][b].push_back(y);
				
				if (j != 0) { // do not push for first add
					
					for (map<int, vector<int>>::iterator it = dependencymap[dep].begin() ; it != dependencymap[dep].end(); it++) {
						if ((*it).first != a && (*it).first != b) // skip variable a and b
							dependencymap[dep][(*it).first].push_back(dependencymap[dep][(*it).first][i]);
					}
		
				}
			}
		}
	}
	
	return true;

}

/**
 * \fn		QueryEvaluator::getResult(list<string>& result)
 * \brief	Gets the result.  
 * \param [out]	result: results are stored in this list, duplicates allowed. 
 * \return 	TRUE if target list contain exactly 1 element, result obtained is not empty, FALSE otherwise
 */
 bool QueryEvaluator::getResult(list<string>& result) {

	vector<pair<int, int>> QTLDepVarList; // first int is dependency and second int is varIndex
	map<int, int> repetitionMap;
	int resultSize = 1;

	// populate the target map + QTLDepVar + repetitionMap + resultSize
	for (int i = 0; i < qTargetList.size(); i++) {
		
		QueryTarget qt = qTargetList.at(i);
		int qvIndex = qt.varIndex;
		int dep = qVariableList[qvIndex].dependency;
		int targetSize;

		if (!getTarget(qt))
			return false; // return false if target has no values
		
		// sets the size of the target
		targetSize = targetmap[dep][qvIndex].size(); 

		// push the target's dependency and varIndex into the list
		QTLDepVarList.push_back(make_pair(dep, qvIndex)); 

		// updates new dep value if dep = -1
		if (dep == -1) 
			dep *= qvIndex; 

		// populate repetitionMap
		if (repetitionMap.count(dep) == 0) { 
		// repetitionMap does not contain dep yet
			
			for (map<int, int>::iterator it = repetitionMap.begin(); it != repetitionMap.end(); ++it) {
				(*it).second *= targetSize;
			}

			resultSize *= targetSize;
			repetitionMap[dep] = 1;

		} // do nothing if repetitionMap already contain dep (i.e. same dep as earlier targets)
		
	}
	
	// make use of repetitionMap to populate results
	for (int i = 1; i <= resultSize; i++) {
		
		string r = "";

		for (int j = 0; j < QTLDepVarList.size(); j++) {
			
			int dep = QTLDepVarList[j].first;
			int qvIndex = QTLDepVarList[j].second;
			int rep = repetitionMap[dep];
			vector<string> tvalues = targetmap[dep][qvIndex];
			
			int index = (i%(rep*tvalues.size()))/rep;

			r += tvalues[index] + " ";
		
		}

		result.push_back(r);

	}

	return true;

 }	
		

 /**
 * \fn		QueryEvaluator::getTarget(QueryTarget qt)
 * \brief	Given a query target, populates the targetmap with the appropriate results.
 * \param [in]	qt: the query target given. 
 * \return 	TRUE if targetmap is populated, FALSE otherwise
 */
 bool QueryEvaluator::getTarget(QueryTarget qt) {
		
	QueryVariable qv;
	vector<int> vecI;
	vector<int> vecTemp;
	vector<string> vecS;
	int v, dep;

	v = qt.varIndex;
	qv = qVariableList.at(v);
	dep = qv.dependency;
		
	if (dep >= 0 && dependencymap[dep].count(v) == 1) {
	// variable is in dependency map (dependent)

		switch (qv.variableType) {
				
			case DT_VARIABLE: 	
			// target is of type variable
					
				vecS = pkb->getAllVarName(dependencymap[dep][v]); 
				for (int i = 0; i < vecS.size(); i++) {
					targetmap[dep][v].push_back(vecS[i]);
				}
				break;

			case DT_PROCEDURE:	
			// target is of type procedure
					
				vecS = pkb->getAllProcName(dependencymap[dep][v]); 
				for (int i = 0; i < vecS.size(); i++) {
					targetmap[dep][v].push_back(vecS[i]);
				}
				break;

			case DT_CALL:	
			// target is of type call
					
				if (qt.hasAttribute == true && qt.attributeType == AT_CALL_PROC_NAME) {
				// target attribute type is procName
						
					for (int i = 0; i < dependencymap[dep][v].size(); i++) {
					// gets the procTable index of the call stmt 
						vecTemp.push_back(pkb->getCallee(dependencymap[dep][v][i])); 
					}
						
					vecS = pkb->getAllProcName(vecTemp); 
					for (int i = 0; i < vecS.size(); i++) {
						targetmap[dep][v].push_back(vecS[i]);
					}
					break; // break only if it is call.procName
					
				}

			default:
			// target is not of type variable or procedure
					
				for (vector<int>::iterator it = dependencymap[dep][v].begin(); it != dependencymap[dep][v].end(); ++it) {
					targetmap[dep][v].push_back(to_string(static_cast<long long>(*it)));
				}
				break;
					
		}

	} else {
	// variable is not in dependency map (independent)
		
		switch (qv.variableType) {
				
			case DT_VARIABLE: 	
			// target is of type variable
					
				vecS = pkb->getAllVarName();
				for (int i = 0; i < vecS.size(); i++) {
					targetmap[dep][v].push_back(vecS[i]);
				}
				break;

			case DT_PROCEDURE:	
			// target is of type procedure
					
				vecS = pkb->getAllProcName();
				for (int i = 0; i < vecS.size(); i++) {
					targetmap[dep][v].push_back(vecS[i]);
				}
				break;

			case DT_BOOLEAN:	
					
				targetmap[dep][v].push_back("TRUE");
				break;
				
			case DT_ASSIGN: 	
			
				if (!pkb->getAllAssignment(&vecI))
					return false;
				break;
		
			case DT_WHILE: 		
			
				if (!pkb->getAllWhile(&vecI))
					return false;
				break;
		
			case DT_IF:			
		
				if (!pkb->getAllIf(&vecI))
					return false;
				break;

			case DT_CALL: 		

				if (!pkb->getAllCall(&vecI))
					return false;
				break;
		
			case DT_CONSTANT:	
			
				if (!pkb->getAllConstant(&vecI))
					return false;
				break;

			case DT_STMT: 		
			case DT_PROGLINE:
				
				if (!pkb->getAllStmt(&vecI))
					return false;
				break;
			
			// Known Variables
			case KT_STMT_NUM:			
					
				if (qt.hasAttribute == true && qt.attributeType == AT_CALL_PROC_NAME) {
				// target is of type call.procName
						
					// gets the call stmt procTable index, then its procName
					targetmap[dep][v].push_back(pkb->getProcName(pkb->getCallee(qv.content)));					
					break; // break only if it is call.procName
					
				}

			case KT_KNOWN_CONSTANT:
				
				targetmap[dep][v].push_back(to_string(static_cast<long long>(qv.content)));
				break;
					
			case KT_KNOWN_VARIABLE:	
					
				targetmap[dep][v].push_back(pkb->getVarName(qv.content));					
				break;
					
			case KT_KNOWN_PROCEDURE:	
					
				targetmap[dep][v].push_back(pkb->getProcName(qv.content));					
				break;
		
			// invalid types, should not appear in target
			case DT_UNDERSCORE:		
			case DT_STMTLST: 			

				break;

		}

		switch (qv.variableType) {
				
			case DT_CALL: 
					
				if (qt.hasAttribute == true && qt.attributeType == AT_CALL_PROC_NAME) {
				// target attribute type is procName
						
					for (int i = 0; i < vecI.size(); i++) {
					// gets the procTable index of the call stmt 
						vecTemp.push_back(pkb->getCallee(vecI[i])); 
					}
						
					vecS = pkb->getAllProcName(vecTemp); 
					for (int i = 0; i < vecS.size(); i++) {
						targetmap[dep][v].push_back(vecS[i]);
					}
					break; // break only if it is call.procName
					
				}

			case DT_ASSIGN: 	
			case DT_WHILE: 		
			case DT_IF:			
			case DT_CONSTANT:	
			case DT_STMT: 		
					
				for (int i = 0; i < vecI.size(); i++) 
					targetmap[dep][v].push_back(to_string(static_cast<long long>(vecI[i])));
				break;
		}

	}

	return true;
}

/**
 * \fn		QueryEvaluator::cartesianProduct(vector<int>* vec1, vector<int>* vec2)
 * \brief	Perform cartesian product of 2 vectors. 
 * \param 	vec1: 1st vector; 
 *			vec2: 2nd vector. 
 * \return 	TRUE if both vectors are not empty, FALSE otherwise
 */
 bool QueryEvaluator::cartesianProduct(vector<int>* vec1, vector<int>* vec2) {
	vector<int> v1, v2; 
	
	if (vec1->empty() || vec2->empty()) 
		return false;
	
	for (int i = 0; i < vec1->size(); i++)
		v1.push_back(vec1->at(i));
	for (int i = 0; i < vec2->size(); i++)
		v2.push_back(vec2->at(i));
	
	vec1->clear();
	vec2->clear();
	
	for (int i = 0; i < v1.size(); i++) {
		for (int j = 0; j < v2.size(); j++) {
			vec1->push_back(v1.at(i));
			vec2->push_back(v2.at(j));
		}
	}
	
	return true;
}

/**
 * \fn		QueryEvaluator::removeDuplicates(vector<int> vec)
 * \brief	Removes duplicate elements in vector. 
 * \param [in] vec: vector to remove duplicates.  
 * \return 	Vector that does not contain duplicate.
 */
 vector<int> QueryEvaluator::removeDuplicates(vector<int> vec) {
	set<int> s(vec.begin(), vec.end());
	return vector<int> (s.begin(), s.end());
}

/**
 * \fn		QueryEvaluator::getWithType(int t)
 * \brief	Switch enum (for with) from Query Preprocessor to its PKB equivalent. 
 * \param [in] t: Query Preprocessor enum.  
 * \return 	equivalent PKB enum.
 */
 int QueryEvaluator::getWithType(int t) {

	 switch (t) {
	 
		case AT_STMT_NUM:
			
			return WITH_STMTNUMBER;
		
		case AT_VAR_NAME:	
			
			return WITH_VARNAME;
		
		case AT_PROC_NAME:		
		case AT_PROCTABLEINDEX:
		
			return WITH_PROCNAME;

		case AT_CALL_PROC_NAME:		

			return WITH_CALLPROCNAME;

		case AT_VALUE:

			return WITH_VALUE;
	 
	 }

 }


/*int main()
{
return 0;
}
*/