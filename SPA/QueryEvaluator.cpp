#include "stdafx.h"
#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(void)
{
	pkb = PKBController::createInstance();
}


QueryEvaluator::~QueryEvaluator(void)
{
}

bool QueryEvaluator::evaluate(vector<Query_clause> qcl, vector<Query_variable> qvl, int tv, list<string>& r)
{
	// prepare new query, initialise data members 
	qClauseList = qcl;
	qVariableList = qvl;
	dependencymap.clear();

	//optimise();
	
	if (!evaluateQuery()) 
		return false; // at least one query clause failed

	if (!getResult(r, tv))
		return false; // result is empty

	return true;
}

bool QueryEvaluator::optimise()
{
	return true;
}	

bool QueryEvaluator::evaluateQuery() 
{
	for (vector<Query_clause>::iterator it = qClauseList.begin(); it != qClauseList.end(); ++it) {
		if (!evaluateClause(*it)) {
			return false; // query clause evaluated to false
		}
	}
	return true;
}

bool QueryEvaluator::evaluateClause(Query_clause qc) {

	vector<int> vectorA, vectorB;
	int arg;

	if (!getVectors(&vectorA, &vectorB, qc.variable1, qc.variable2, &arg))
		return false; // unable to get vectors

	switch (qc.relation) {

		case Query_clause::QC_PARENT:
			
			if (!pkb->parent(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case Query_clause::QC_FOLLOWS:
			
			if (!pkb->follows(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case Query_clause::QC_MODIFIES:
			
			if (!pkb->modifies(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case Query_clause::QC_USES:
			
			if (!pkb->uses(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case Query_clause::QC_PARENTT:

			if (!pkb->parentStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case Query_clause::QC_FOLLOWST:

			if (!pkb->followsStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case Query_clause::QC_PATTERN:
			
			if (!pkb->pattern(&vectorA, &vectorB, qc.variable3, arg))
				return false; // can't find relation
			break;
	}
	
	if (!intersect(&vectorA, &vectorB, qc.variable1, qc.variable2, arg))
		return false; // intersection is empty

	return true;
}

bool QueryEvaluator::getVectors(vector<int>* vecA, vector<int>* vecB, int a, int b, int *arg) {
	
	int argA, argB;
	
	argA = dependencymap.count(a);
	argB = dependencymap.count(b);
	*arg = 2*argA + argB;

	switch (*arg) {

		case 3: // both a and b are found in dependency map
			
			*vecA = dependencymap[a]; // get vector a
			*vecB = dependencymap[b]; // get vector b
			return true;
		
		case 2: // a is found in dependency map

			*vecA = removeDuplicates(dependencymap[a]); // get vector a
			if (!getOtherVector(vecB, b, &argB, vecA)) // get vector b, cartesian product performed if required 
				return false;
			*arg += argB;
			return true;

		case 1: // b is found in dependency map
			
			*vecB = removeDuplicates(dependencymap[b]); // get vector b
			if (!getOtherVector(vecA, a, &argA, vecB)) // get vector a, cartesian product performed if required
				return false;
			*arg += 2*argA;
			return true;

		case 0: // both a and b are not found in dependency map
			
			if (!getVector(vecA, a, &argA)) // get vector a
				return false;
			if (!getVector(vecB, b, &argB)) // get vector b
				return false;
			*arg = 2*argA + argB;
			// if arg = 3 and vecA and vecB are both not empty, do cartesian product 
			if (*arg == 3 && !vecA->empty() && !vecB->empty())
				cartesianProduct(vecA, vecB);
			return true;
	
	}

}

// fills up vector vec given query variable v.
// v does not appear in dependency map
// if v is dependent and not empty, perform cartesian product with vec2
bool QueryEvaluator::getOtherVector(vector<int>* vec, int v, int *arg, vector<int>* vec2) {
	
	Query_variable qv = qVariableList.at(v);

	if (qv.dependency >= 0) {
	// variable is dependent

		switch (qv.type) {
		
			case Query_variable::QV_ASSIGN:
				
				if (!pkb->getAllAssignment(vec))
					return false;
				else if (!cartesianProduct(vec, vec2))
					return false;
				break;

			case Query_variable::QV_WHILE:
				
				if (!pkb->getAllWhile(vec))
					return false;
				else if (!cartesianProduct(vec, vec2))
					return false;
				break;

			case Query_variable::QV_STMT:
			case Query_variable::QV_VARIABLE:
				break;

			// invalid cases??
			case Query_variable::QV_STMT_NUMBER:
			case Query_variable::QV_CONSTANTVAR:
			case Query_variable::QV_UNDERSCORE:
			case Query_variable::QV_CONSTANT:
				break;
		}

		*arg = 1;
	
	} else {
	// variable is independent

		switch (qv.type) {
		
			case Query_variable::QV_ASSIGN:
				
				if (!pkb->getAllAssignment(vec))
					return false;
				break;

			case Query_variable::QV_WHILE:
				
				if (!pkb->getAllWhile(vec))
					return false;
				break;

			// vector is empty for the next 3 cases
			case Query_variable::QV_STMT: 
			case Query_variable::QV_VARIABLE:
			case Query_variable::QV_UNDERSCORE: 
				break;
			
			// adds the constant element to the vector
			case Query_variable::QV_STMT_NUMBER:
			case Query_variable::QV_CONSTANTVAR:
				vec->push_back(stoi(qv.content));
				break;

			// invalid case??
			case Query_variable::QV_CONSTANT:
				break;
		}
		
		*arg = 0;
	
	}

	return true;

}

// fills up vector vec given query variable v.
// v does not appear in dependency map
bool QueryEvaluator::getVector(vector<int>* vec, int v, int* arg) {
	
	Query_variable qv = qVariableList.at(v);

	if (qv.dependency >= 0) {
	// variable is dependent

		switch (qv.type) {
		
			case Query_variable::QV_ASSIGN:
				
				if (!pkb->getAllAssignment(vec))
					return false;
				break;

			case Query_variable::QV_WHILE:
				
				if (!pkb->getAllWhile(vec))
					return false;
				break;

			case Query_variable::QV_STMT:
			case Query_variable::QV_VARIABLE:
				break;

			// invalid cases??
			case Query_variable::QV_STMT_NUMBER:
			case Query_variable::QV_CONSTANTVAR:
			case Query_variable::QV_UNDERSCORE:
			case Query_variable::QV_CONSTANT:
				break;
		}

		*arg = 1;
	
	} else {
	// variable is independent

		switch (qv.type) {
		
			case Query_variable::QV_ASSIGN:
				
				if (!pkb->getAllAssignment(vec))
					return false;
				
				break;

			case Query_variable::QV_WHILE:
				
				if (!pkb->getAllWhile(vec))
					return false;

				break;

			// vector is empty for the next 3 cases
			case Query_variable::QV_STMT: 
			case Query_variable::QV_VARIABLE:
			case Query_variable::QV_UNDERSCORE: 
				break;
			
			// adds the constant element to the vector
			case Query_variable::QV_STMT_NUMBER:
			case Query_variable::QV_CONSTANTVAR:
				vec->push_back(stoi(qv.content));
				break;

			// invalid case??
			case Query_variable::QV_CONSTANT:
				break;
		}
		
		*arg = 0;
	
	}

	return true;

}

bool QueryEvaluator::intersect(vector<int>* vecA, vector<int>* vecB, int a, int b, int arg) {
// for only 2 clauses, at most 2 dependent variables, if size of map = 2, always correspondence

	switch (arg) {
		case 3: // store both vecA and vecB

			// Since at most 2 dependent variables, do not need to do any intersection
			dependencymap[a] = *vecA;
			dependencymap[b] = *vecB;

		case 2: // store only vecA
		
			if (dependencymap.count(a) == 0) {
			// no vecA in dependency map
				
				if (dependencymap.empty()) {
				// current dependency map is empty
					dependencymap[a] = *vecA; // store vecA
				} else {
				// dependency map is not empty
				/***************************************************************
				* IMPOSSIBLE CASE FOR 2 CLAUSES + SINGLE TARGET VARIABLE
				* * * not empty => this is second clause, and first clause contains
				* * * dependent x only (at most 2 dependent variables). 
				* * * if x not in second clause, x is target variable 
				* * * => a only appears once => a not dependent.
				***************************************************************/
				}

			} else {
			// vecA already exists in dependency map

				if (dependencymap.size() == 1) {
				// dependency map only contains key a
					dependencymap[a] = *vecA; // replace it with new vecA
				} else {
				// dependency map contains other keys
					// intersect vecA with dependencymap 
					if (!intersectDependencyMap(vecA, a))
						return false; // intersection is empty set			
				}
			}

		case 1: // store only vecB
			
			if (dependencymap.count(b) == 0) {
			// no vecB in dependency map
				
				if (dependencymap.empty()) {
				// current dependency map is empty
					dependencymap[b] = *vecB; // store vecB
				} else {
				// dependency map is not empty
				/***************************************************************
				* IMPOSSIBLE CASE FOR 2 CLAUSES + SINGLE TARGET VARIABLE
				* * * not empty => this is second clause, and first clause contains
				* * * dependent x only (at most 2 dependent variables). 
				* * * if x not in second clause, x is target variable 
				* * * => b only appears once => b not dependent.
				***************************************************************/
				}

			} else {
			// vecB already exists in dependency map

				if (dependencymap.size() == 1) {
				// dependency map only contains key b
					dependencymap[b] = *vecB; // replace it with new vecB
				} else {
				// dependency map contains other keys
					// intersect vecB with dependencymap 
					if (!intersectDependencyMap(vecB, b))
						return false; // intersection is empty set			
				}
			}

			break;
		case 0: // do not need to store
			break;
	}
	return true;
}

// map contains 2 keys, i, j (both with correspondence)
// intersection of an existing variable in map, with new vector
bool QueryEvaluator::intersectDependencyMap(vector<int>* vec, int v) {
	
	int v2, size, index;

	for (map<int, vector<int>>::iterator it = dependencymap.begin() ; it != dependencymap.end(); it++) {
		if ((*it).first != v)
			v2 = (*it).first;
	}

	size = dependencymap[v].size();
	index = 0;
		
	for (int i = 0; i < size; i++) {
		
		bool match = false;
		int x = dependencymap[v][index];
		
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
			dependencymap[v].erase(dependencymap[v].begin() + index);
			dependencymap[v2].erase(dependencymap[v2].begin() + index);
		}
	
	}
	
	if (dependencymap[v].empty()) // intersection is null
		return false;
	else
		return true;

}

// perform cartesian product of 2 vectors
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

// autotester allows duplicate
bool QueryEvaluator::getResult(list<string>& result, int v) {
	
	Query_variable qv;
	vector<int> vecI;
	vector<string> vecS;
	
	qv = qVariableList.at(v);

	if (dependencymap.count(v) == 1) {
	// variable is in dependency map (dependent)

		if (qv.type == Query_variable::QV_VARIABLE) { 
		// target is of type variable
			vecS = pkb->getAllVarName(dependencymap[v]); 
			for (int i = 0; i < vecS.size(); i++) {
				result.push_back(vecS[i]);
			}
		}
		else {
		// target is not of type variable
			for (vector<int>::iterator it = dependencymap[v].begin(); it != dependencymap[v].end(); ++it) {
				result.push_back(to_string(static_cast<long long>(*it)));
			}
		}

	} else {
	// variable is not in dependency map (independent)
		
		if (qv.type == Query_variable::QV_VARIABLE) { 
		// target is of type variable
			vecS = pkb->getAllVarName();
			for (int i = 0; i < vecS.size(); i++) {
				result.push_back(vecS[i]);
			}
		}
		else {
		// target is not of type variable	
			switch (qv.type) {
		
				case Query_variable::QV_ASSIGN:
				
					if (!pkb->getAllAssignment(&vecI))
						return false;
					break;

				case Query_variable::QV_WHILE:
				
					if (!pkb->getAllWhile(&vecI))
						return false;
					break;

				case Query_variable::QV_STMT:
					
					if (!pkb->getAllStmt(&vecI))
						return false;
					break;
				
				case Query_variable::QV_CONSTANT:

					if (!pkb->getAllConstant(&vecI))
						return false;
					break;

				// invalid cases??
				case Query_variable::QV_VARIABLE:
				case Query_variable::QV_STMT_NUMBER:
				case Query_variable::QV_CONSTANTVAR:
				case Query_variable::QV_UNDERSCORE:
					break;
			}

			for (int i = 0; i < vecI.size(); i++) {
				result.push_back(to_string(static_cast<long long>(vecI[i])));
			}
		}
	}
	return true;
}

vector<int> QueryEvaluator::removeDuplicates(vector<int> vec) {
	set<int> s(vec.begin(), vec.end());
	return vector<int> (s.begin(), s.end());
}

/*int main()
{
return 0;
}
*/