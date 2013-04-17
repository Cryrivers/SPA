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
	
	QVsizemap[DT_PROCEDURE] = pkb->procTableSize();
	QVsizemap[DT_STMTLST] = pkb->stmtListSize();
	QVsizemap[DT_STMT] = pkb->stmtSize();
	QVsizemap[DT_PROGLINE] = pkb->stmtSize();
	QVsizemap[DT_ASSIGN] = pkb->assignSize();
	QVsizemap[DT_CALL] = pkb->callSize();
	QVsizemap[DT_WHILE] = pkb->whileSize();
	QVsizemap[DT_IF] = pkb->ifSize();
	QVsizemap[DT_VARIABLE] = pkb->varTableSize();
	QVsizemap[DT_CONSTANT] = pkb->constSize();
	QVsizemap[DT_BOOLEAN] = 0; // invalid
	QVsizemap[DT_UNDERSCORE] = QVsizemap[DT_STMT]; // special value (dependent on the relation type)
	QVsizemap[DT_PLUS] = QVsizemap[DT_ASSIGN]; // set to assign
	QVsizemap[DT_TIMES] = QVsizemap[DT_ASSIGN]; // set to assign
	QVsizemap[DT_MINUS] = QVsizemap[DT_ASSIGN]; // set to assign
	QVsizemap[KT_STMT_NUM] = 1; // constant
	QVsizemap[KT_KNOWN_VARIABLE] = 1; // constant
	QVsizemap[KT_KNOWN_PROCEDURE] = 1; // constant
	QVsizemap[KT_KNOWN_CONSTANT] = 1; // constant

	QCsizemap[RT_MODIFIESP] = pkb->modifiesPSize();
	QCsizemap[RT_MODIFIESS] = pkb->modifiesSize();
	QCsizemap[RT_USESP] = pkb->usesPSize();
	QCsizemap[RT_USESS] = pkb->usesSize();
	QCsizemap[RT_CALLS] = pkb->callsSize();
	QCsizemap[RT_CALLST] = QCsizemap[RT_CALLS]*50;
	QCsizemap[RT_PARENT] = pkb->parentSize();
	QCsizemap[RT_PARENTT] = QCsizemap[RT_PARENT]*50;
	QCsizemap[RT_FOLLOWS] = pkb->followsSize();
	QCsizemap[RT_FOLLOWST] = QCsizemap[RT_FOLLOWS]*50;
	QCsizemap[RT_NEXT] = pkb->nextSize();
	QCsizemap[RT_NEXTT] = QCsizemap[RT_NEXT]*80;
	QCsizemap[RT_AFFECTS] = 999; //QCsizemap[RT_NEXT]*50;
	QCsizemap[RT_AFFECTST] = 999; //QCsizemap[RT_AFFECTS]*100;
	QCsizemap[RT_CONTAINS] = 999;
	QCsizemap[RT_CONTAINST] = 999;//QCsizemap[RT_CONTAINS]*50;
	QCsizemap[RT_SIBLING] = 999;
	QCsizemap[RT_NEXTBIP] = 999; //QCsizemap[RT_NEXT]*100;
	QCsizemap[RT_NEXTBIPT] = 999; //QCsizemap[RT_NEXTBIP]*100;
	QCsizemap[RT_AFFECTSBIP] = 999; //QCsizemap[RT_AFFECTS]*100;
	QCsizemap[RT_AFFECTSBIPT] = 999; //QCsizemap[RT_AFFECTSBIP]*100;
	QCsizemap[CT_PATTERN] = 999;
	QCsizemap[CT_WITH] = 999;

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
	
	qClauseList2.clear();
	dependencymap.clear();
	targetmap.clear();
	
	optimise();
	
	if (!evaluateQuery()) 
		return false; // at least one query clause failed

	if (!getResult(r))
		return false; // result is empty

	return true;
}

/**
 * \fn		QueryEvaluator::evaluateQuery()
 * \brief	Optimises order of query clauses 
 * \return 	TRUE 
 */
bool QueryEvaluator::optimise()
{
	/*map<int, vector<long long>> QCweights; // key: dep; keeps the weight of query clauses 
	long long value;

	// calculate the weights of the clauses
	for (map<int, vector<QueryClause>>::iterator mit = qClauseList.begin(); mit != qClauseList.end(); ++mit) {
	// each loop is a different dependency set

		if (mit->first >= 0) { // calculate weights only for dependent sets
			
			for (vector<QueryClause>::iterator vit = (*mit).second.begin(); vit != (*mit).second.end(); ++vit) {
				
				value = QVsizemap[qVariableList[vit->variable1].variableType]*QVsizemap[qVariableList[vit->variable2].variableType]*QCsizemap[vit->relationType];
				QCweights[mit->first].push_back(value);
			
			}
		}
	}
	
	// populate qClauseList2
	for (map<int, vector<QueryClause>>::iterator mit = qClauseList.begin(); mit != qClauseList.end(); ++mit) {
	// each loop is a different dependency set

		if (mit->first < 0) { // independent set
			
			for (vector<QueryClause>::iterator vit = (*mit).second.begin(); vit != (*mit).second.end(); ++vit)
				qClauseList2.push_back(*vit);
		
		} else { // dependent set
			
			list<int> sIndexList = sortIndex(QCweights[mit->first]);
			set<int> visitedVariableSet;

			QueryClause qc = (*mit).second[sIndexList.front()]; // get the clause with lowest weight (front of list)
			qClauseList2.push_back(qc); // push it into qClauseList2 
			sIndexList.pop_front(); // pop the qcindex

			// add the variables into the visitedVariableSet if they are dependent 
			if (qVariableList[qc.variable1].dependency >= 0)
				visitedVariableSet.insert(qc.variable1);
			if (qVariableList[qc.variable2].dependency >= 0)
				visitedVariableSet.insert(qc.variable2);

			while (!sIndexList.empty()) { // list is not empty
				
				for (list<int>::iterator it = sIndexList.begin(); it != sIndexList.end(); ++it) {
			
					if (visitedVariableSet.count((*mit).second[*it].variable1) == 1 ||
							visitedVariableSet.count((*mit).second[*it].variable2) == 1) {
					// query clause can be selected
						
						qClauseList2.push_back((*mit).second[*it]); // push qc into qClauseList2
						
						// add the variables into the visitedVariableSet if they are dependent 
						if (qVariableList[(*mit).second[*it].variable1].dependency >= 0)
							visitedVariableSet.insert(qc.variable1);
						if (qVariableList[(*mit).second[*it].variable2].dependency >= 0)
							visitedVariableSet.insert(qc.variable2);
						
						sIndexList.erase(it); // remove the qcIndex from the list
						break;

					}
				}	
			}
		}
	}	
	*/
	for (map<int, vector<QueryClause>>::iterator mit = qClauseList.begin(); mit != qClauseList.end(); ++mit) {
		for (vector<QueryClause>::iterator vit = (*mit).second.begin(); vit != (*mit).second.end(); ++vit)
				qClauseList2.push_back(*vit);
	}		
	return true;
}

/**
 * \fn		QueryEvaluator::evaluateQuery()
 * \brief	Evaluates query by querying clauses one at a time  
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluateQuery() 
{
	
	for (vector<QueryClause>::iterator vit = qClauseList2.begin(); vit != qClauseList2.end(); ++vit) {
			
		if (vit->variable1 == vit->variable2 && qVariableList[vit->variable1].variableType != DT_UNDERSCORE) { // reflexive clause
				
			if (!evaluateReflexiveClause(*vit)) 
				return false; // query clause evaluated to false
				
		} else { // non reflexive clause
				
			if (!evaluateClause(*vit)) 
				return false; // query clause evaluated to false
				
		}
	}
	
	return true;
}

/**
 * \fn		QueryEvaluator::evaluateReflexiveClause(QueryClause qc)
 * \brief	Evaluates a given reflexive query clause, supports relation type NextT, Affects, AffextsT, NextBIPT, AffectsBIP, AffectsBIPT  
 * \param [in]	qc: query clause. 
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluateReflexiveClause(QueryClause qc) {

	vector<int> vectorA;
	int a, dep, inDM, arg;

	a = qc.variable1;
	dep = qVariableList.at(a).dependency; 
	
	if (dep >= 0) { // variable is dependent

		inDM = 0;
		
		// checks if a exists in dependency map
		if (dependencymap.count(dep) == 1)
			inDM = dependencymap[dep].count(a);

		if (inDM) { // a exists in dependency map

			vectorA = removeDuplicates(dependencymap[dep][a]); //get vectorA

		} else { // a does not exist in dependency map
			
			// get vectorA
			switch (qVariableList.at(a).variableType) {
				
				case DT_ASSIGN: 	
					if (!pkb->getAllAssignment(&vectorA))
						return false;
					break;
				
				case DT_WHILE: 		
					if (!pkb->getAllWhile(&vectorA))
						return false;
					break;
				
				case DT_IF:			
					if (!pkb->getAllIf(&vectorA))
						return false;
					break;
				
				case DT_CALL: 		
					if (!pkb->getAllCall(&vectorA))
						return false;
					break;
				
				case DT_STMT: 		
				case DT_PROGLINE:
					if (!pkb->getAllStmt(&vectorA))
						return false;
					break;
				
				default:
					return false;

			}
		}

		vector<int> vectorB (vectorA); //make copy of vectorA
		arg = 3; // update arg

		//perform evaluation
		switch (qc.relationType) {
			
			case RT_NEXTT:
				if (!pkb->nextStar(&vectorA, &vectorB, arg))
					return false; // can't find relation
				break;
			
			case RT_AFFECTS:
				if (!pkb->affects(&vectorA, &vectorB, arg))
					return false; // can't find relation
				break;

			case RT_AFFECTST:
				if (!pkb->affectsStar(&vectorA, &vectorB, arg))
					return false; // can't find relation
				break;

			case RT_NEXTBIPT:
				if (!pkb->nextBipStar(&vectorA, &vectorB, arg))
					return false; // can't find relation
				break;

			case RT_AFFECTSBIP:
				if (!pkb->affectsBip(&vectorA, &vectorB, arg))
					return false; // can't find relation
				break;

			case RT_AFFECTSBIPT:
				if (!pkb->affectsBipStar(&vectorA, &vectorB, arg))
					return false; // can't find relation
				break;

		}

		if (dependencymap.count(dep)) { // dependency map is not empty

			if (!intersectDependencyMap(dep, a, &vectorA))
				return false; // intersection is empty set

		} else { // dependency map is empty
		
			dependencymap[dep][a] = vectorA; // push vectorA into it

		}

	} else { // variable is independent
	
		// get vectorA
		switch (qVariableList.at(a).variableType) {
				
			case DT_ASSIGN: 	
				if (!pkb->getAllAssignment(&vectorA))
					return false;
				break;
				
			case DT_WHILE: 		
				if (!pkb->getAllWhile(&vectorA))
					return false;
				break;
				
			case DT_IF:			
				if (!pkb->getAllIf(&vectorA))
					return false;
				break;
				
			case DT_CALL: 		
				if (!pkb->getAllCall(&vectorA))
					return false;
				break;
				
			case DT_STMT: 		
			case DT_PROGLINE:
				if (!pkb->getAllStmt(&vectorA))
					return false;
				break;
				
			case DT_UNDERSCORE:
				switch (qc.relationType) {
						
					case RT_NEXTT:
					case RT_NEXTBIPT:
						if (!pkb->getAllStmt(&vectorA))
							return false;
						break;
						
					case RT_AFFECTS:
					case RT_AFFECTST:
					case RT_AFFECTSBIP:
					case RT_AFFECTSBIPT:
						if (!pkb->getAllAssignment(&vectorA))
							return false;
						break;
				} 
				break;

			// Known Variables
			case KT_STMT_NUM:				
				vectorA.push_back(qVariableList.at(a).content);	
				break;

			default:
				return false;

		}

		// perform evaluation
		for (int i = 0; i < vectorA.size(); i++) {
			
			if (evaluateBoolean(vectorA[i],vectorA[i],qc.relationType))
				return true; // only need one to be true 
		
		}

		return false; // return false if all return false
		
	}

	return true;
}

/**
 * \fn		QueryEvaluator::evaluateClause(QueryClause qc)
 * \brief	Evaluates a given query clause, supports relation type Parent, ParentT, Follows, FollowsT, 
 *			ModifiesS, ModifiesP, UsesS, UsesP, Calls, CallsT, Next, NextT, Affects, AffextsT, 
 *			NextBIP, NextBIPT, AffectsBIP, AffectsBIPT, Contains, ContainsT, Sibling, Pattern, With.  
 * \param [in]	qc: query clause. 
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluateClause(QueryClause qc) {

	vector<int> vectorA, vectorB;
	int arg;

	if (!getVectors(&vectorA, &vectorB, qc, &arg))
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
			if (!pkb->affects(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_AFFECTST:
			if (!pkb->affectsStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
			
		case RT_NEXTBIP:
			if (!pkb->nextBip(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_NEXTBIPT:
			if (!pkb->nextBipStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_AFFECTSBIP:
			if (!pkb->affectsBip(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;

		case RT_AFFECTSBIPT:
			if (!pkb->affectsBipStar(&vectorA, &vectorB, arg))
				return false; // can't find relation
			break;
		
		case RT_CONTAINS:
			if (!pkb->contains(&vectorA, &vectorB, getNodeType(qc.variable1), getNodeType(qc.variable2), arg))
				return false; // can't find relation
			break;

		case RT_CONTAINST:
			if (!pkb->containsStar(&vectorA, &vectorB, getNodeType(qc.variable1), getNodeType(qc.variable2), arg))
				return false; // can't find relation
			break;

		case RT_SIBLING:	 
			if (!pkb->sibling(&vectorA, &vectorB, getNodeType(qc.variable1), getNodeType(qc.variable2), arg))
				return false; // can't find relation
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
 * \fn		QueryEvaluator::evaluateBoolean(int a, int b, Type relationType)
 * \brief	Evaluates realation(a,b), supports relation type Parent, ParentT, Follows, FollowsT, 
 *			ModifiesS, ModifiesP, UsesS, UsesP, Calls, CallsT, Next, NextT, Affects, AffextsT, 
 *			NextBIP, NextBIPT, AffectsBIP, AffectsBIPT, but not Contains, ContainsT, Sibling, Pattern, With.  
 * \param [in]	a: first parameter;
 *				b: second parameter;
 *				relationType: relation type.. 
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 */
bool QueryEvaluator::evaluateBoolean(int a, int b, Type relationType) {

	switch (relationType) {

		case RT_PARENT:
			return pkb->isParent(a, b);
			
		case RT_PARENTT:
			return pkb->isParentStar(a, b);
			
		case RT_FOLLOWS:
			return pkb->isFollows(a, b);
			
		case RT_FOLLOWST:
			return pkb->isFollowsStar(a, b);

		case RT_MODIFIESS:
			return pkb->isModifies(a, b);

		case RT_MODIFIESP:
			return pkb->isModifiesP(a, b);

		case RT_USESS:
			return pkb->isUses(a, b);

		case RT_USESP:
			return pkb->isUsesP(a, b);
		
		case RT_CALLS:
			return pkb->isCalls(a, b);

		case RT_CALLST:
			return pkb->isCallsStar(a, b);

		case RT_NEXT:
			return pkb->isNext(a, b);
			
		case RT_NEXTT:
			return pkb->isNextStar(a, b);
			
		case RT_AFFECTS:
			return pkb->isAffects(a, b);
			
		case RT_AFFECTST:
			return pkb->isAffectsStar(a, b);
			
		case RT_NEXTBIP:
			return pkb->isNextBip(a, b);

		case RT_NEXTBIPT:
			return pkb->isNextBipStar(a, b);
			
		case RT_AFFECTSBIP:
			return pkb->isAffectsBip(a, b);
			
		case RT_AFFECTSBIPT:
			return pkb->isAffectsBipStar(a, b);
		/*	
		case RT_CONTAINS:
			if (!pkb->contains(&vectorA, &vectorB, getNodeType(qc.variable1), getNodeType(qc.variable2), arg))
				return false; // can't find relation
			break;

		case RT_CONTAINST:
			if (!pkb->containsStar(&vectorA, &vectorB, getNodeType(qc.variable1), getNodeType(qc.variable2), arg))
				return false; // can't find relation
			break;

		case RT_SIBLING:	 
			if (!pkb->sibling(&vectorA, &vectorB, getNodeType(qc.variable1), getNodeType(qc.variable2), arg))
				return false; // can't find relation
			break;

		case CT_PATTERN:
			if (!pkb->pattern(&vectorA, &vectorB, qc.variable3, qc.patternType, arg))
				return false; // can't find relation
			break;
		
		case CT_WITH:
			if (!pkb->with(&vectorA, &vectorB, getWithType(qc.attribute1), getWithType(qc.attribute2), arg))
				return false;
			break;
		*/
	}
	
}

/**
 * \fn		QueryEvaluator::getVectors(vector<int>* vecA, vector<int>* vecB,  QueryClause qc, int *arg)
 * \brief	Retrieves 2 vectors and dependency corresponding to the 2 variables a, b in the querying clause.  
 * \param [in]	qc: query clause;
 *		  [out]	vecA: vector corresponding to a;
 *		  [out]	vecB: vector corresponding to b;
 *		  [out]	arg:  the dependency of a and b. 
 * \return 	TRUE if vectors could be obtained, FALSE otherwise
 */
bool QueryEvaluator::getVectors(vector<int>* vecA, vector<int>* vecB, QueryClause qc, int *arg) {
	
	int a, b, argA, argB, depA, depB;
	
	a = qc.variable1;
	b = qc.variable2;
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
			
			removeDuplicatesPair(vecA, vecB);
			
			// no need for cartesian product as both must belong to the same dependency
			return true;
		
		case 2: // only a is found in dependency map
			*vecA = removeDuplicates(dependencymap[depA][a]); // get vector a
			if (!getVector(vecB, b, &argB, qc.relationType)) // get vector b 
				return false;
			
			if (argB == 1 && !vecB->empty() && !cartesianProduct(vecA, vecB)) // perform cartesian product if vecB is dependent and not empty
				return false;
			*arg += argB;
			
			return true;

		case 1: // only b is found in dependency map
			*vecB = removeDuplicates(dependencymap[depB][b]); // get vector b
			if (!getVector(vecA, a, &argA, qc.relationType)) // get vector a
				return false;
			
			if (argA == 1 && !vecA->empty() && !cartesianProduct(vecA, vecB)) // perform cartesian product if vecA is dependent and not empty
				return false;
			*arg += 2*argA;
			
			return true;

		case 0: // both a and b are not found in dependency map
			if (!getVector(vecA, a, &argA, qc.relationType)) // get vector a
				return false;
			if (!getVector(vecB, b, &argB, qc.relationType)) // get vector b
				return false;
			
			*arg = 2*argA + argB;
			// if arg = 3 (both dependent) and vecA and vecB are both not empty, do cartesian product 
			if (*arg == 3 && !vecA->empty() && !vecB->empty() && !cartesianProduct(vecA, vecB))
				return false;
			
			return true;
	
	}

}
 
/**
 * \fn		QueryEvaluator::getVector(vector<int>* vec, int v, int *arg, int rel)
 * \brief	Fills up vector vec given query variable v and its relation. Currently, v does not appear in dependency map.  
 * \param [in]	v: query variable index.
 *		  [in]	rel: the relation we are evaluating	
 *		  [out]	vec: vector corresponding to v;
 *		  [out]	arg:  the dependency of v. 
 * \return 	TRUE if vector could be obtained, FALSE otherwise
 */
 bool QueryEvaluator::getVector(vector<int>* vec, int v, int *arg, int rel) {
	
	QueryVariable qv = qVariableList.at(v);

	switch (qv.variableType) {
		
		case DT_ASSIGN: 	
			if (rel != RT_AFFECTS && rel != RT_AFFECTST && rel != RT_AFFECTSBIP && rel != RT_AFFECTSBIPT) {
			// do not get all assignments if relation is of affects type
				
				if (!pkb->getAllAssignment(vec))
					return false;		
			}
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
		case DT_STMTLST:
		case DT_VARIABLE: 	
		case DT_PROCEDURE:	
		case DT_UNDERSCORE:
			break;
			
		// invalid types, should not appear in relations
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
			removeDuplicatesPair(vecA, vecB);
			dep = qVariableList.at(a).dependency; // a and b must be of the same dependency
			
			if (dependencymap.count(dep) == 1) { // dependency map already exists
				
				if (!intersectDependencyMapPair(dep, a, vecA, b, vecB))
					return false; // intersection is empty set

			} else { // dependency map is empty
				
				// store both vectors
				dependencymap[dep][a] = *vecA;
				dependencymap[dep][b] = *vecB;
			
			}
			
			break;

		case 2: // store only vecA
			*vecA = removeDuplicates(*vecA);
			dep = qVariableList.at(a).dependency;
			
			if (dependencymap.count(dep) == 1) { // dependency map already exists
				
				if (!intersectDependencyMap(dep, a, vecA))
					return false; // intersection is empty set

			} else { // dependency map is empty
				
				// store vecA
				dependencymap[dep][a] = *vecA;

			}
			
			break;

		case 1: // store only vecB
			*vecB = removeDuplicates(*vecB);
			dep = qVariableList.at(b).dependency;
			
			if (dependencymap.count(dep) == 1) { // dependency map already exists
				
				if (!intersectDependencyMap(dep, b, vecB))
					return false; // intersection is empty set

			} else { // dependency map is empty
				
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
 * \brief	Perform intersection with the dependency map for single variable. Map is not empty. 
 *			Incoming variable either already exists or is new. 
 * \param [in]	dep: dependency of v;
 *				v: query variable index;
 *				vec: vector corresponding to v. 
 * \return 	TRUE if intersection is successful, FALSE otherwise
 */
bool QueryEvaluator::intersectDependencyMap(int dep, int v, vector<int>* vec) {
	
	int size, index;
	
	if (dependencymap[dep].count(v) == 1) {
	// variable already exists, remove rows from map
	
	/****************************************************************************************
	NEW EVALUATION:
		Convert vec to set and when checking use count
	****************************************************************************************/
		
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
			} // do nothing if matched

		}

		if (dependencymap[dep][v].empty()) // intersection is null
			return false;

	} else {
	// variable is new, do cartesian product
	
	/****************************************************************************************
	NEW EVALUATION:
		This case will NEVER happen
	****************************************************************************************/
		
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
	
	/****************************************************************************************
	NEW EVALUATION:
		Iterate through the list and check Boolean, if false, remove from list.
		At end, if map becomes empty -> return false
	****************************************************************************************/
		
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
			} // do nothing if matched

		}

		if (dependencymap[dep][a].empty()) // intersection is null
			return false;

	} else if (dependencymap[dep].count(a) == 1) {
	// variable a exists in map
	
	/****************************************************************************************
	NEW EVALUATION:
		Use a map to keep values of a (key) pointing to list of b (value)
		a found in key -> duplicate b in it
		a not found in key -> remove row from dependency map
	****************************************************************************************/

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
	
	/****************************************************************************************
	NEW EVALUATION:
		Use a map to keep values of b (key) pointing to list of a (value)
		b found in key -> duplicate a in it
		b not found in key -> remove row from dependency map
	****************************************************************************************/
		
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
	
	/****************************************************************************************
	NEW EVALUATION:
		This case will NEVER happen
	****************************************************************************************/

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
 * \return 	TRUE if result list contain at least 1 element, result obtained is not empty, FALSE otherwise
 */
 bool QueryEvaluator::getResult(list<string>& result) {

	QueryTarget qt;
	int qvIndex;
	int dep;

	if (qTargetList.size() == 1) {
		
		qt = qTargetList.at(0);
		qvIndex = qt.varIndex;
		dep = qVariableList[qvIndex].dependency;
		
		if (!getTarget(qt))
			return false; // return false if target has no values

		for (int i = 0; i < targetmap[dep][qvIndex].size(); i++)
			result.push_back(targetmap[dep][qvIndex][i]);

	} else {
		
		vector<pair<int, int>> QTLDepVarList; // first int is dependency and second int is varIndex
		map<int, int> repetitionMap; // key is dep, value the number of times to repeat
		int resultSize = 1;
		int targetSize;

		// populate the target map + QTLDepVar + repetitionMap + resultSize
		for (int i = 0; i < qTargetList.size(); i++) {
		
			qt = qTargetList.at(i);
			qvIndex = qt.varIndex;
			dep = qVariableList[qvIndex].dependency;

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
		for (int i = 0; i < resultSize; i++) {
		
			string r = "";

			for (int j = 0; j < QTLDepVarList.size(); j++) {
			
				int dep = QTLDepVarList[j].first;
				int qvIndex = QTLDepVarList[j].second;
				int rep = repetitionMap[(dep == -1 ? dep*qvIndex : dep)];
				int index = (i%(rep*targetmap[dep][qvIndex].size()))/rep;

				r += (j == 0 ? "" : " ") + targetmap[dep][qvIndex][index];
		
			}

			result.push_back(r);

		}
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
				
			case DT_VARIABLE: // target is of type variable	
				targetmap[dep][v] = pkb->getAllVarName(dependencymap[dep][v]); 
				break;

			case DT_PROCEDURE: // target is of type procedure
				targetmap[dep][v] = pkb->getAllProcName(dependencymap[dep][v]); 
				break;

			case DT_CALL: // target is of type call	
				if (qt.hasAttribute == true && qt.attributeType == AT_CALL_PROC_NAME) {
				// target attribute type is procName
						
					for (int i = 0; i < dependencymap[dep][v].size(); i++) {
					// gets the procTable index of the call stmt 
						vecTemp.push_back(pkb->getCallee(dependencymap[dep][v][i])); 
					}
						
					targetmap[dep][v] = pkb->getAllProcName(vecTemp); 
					break; // break only if it is call.procName
					
				} // else drops into default case

			default: // target is not of type variable or procedure
				for (vector<int>::iterator it = dependencymap[dep][v].begin(); it != dependencymap[dep][v].end(); ++it) {
					targetmap[dep][v].push_back(to_string(static_cast<long long>(*it)));
				}
				break;
					
		}

	} else { // variable is not in dependency map (independent)
		
		switch (qv.variableType) {
				
			case DT_VARIABLE: // target is of type variable
				targetmap[dep][v] = pkb->getAllVarName();
				break;

			case DT_PROCEDURE: // target is of type procedure	
				targetmap[dep][v] = pkb->getAllProcName();
				break;

			case DT_BOOLEAN:	
				targetmap[dep][v].push_back("true");
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
			
			case DT_STMTLST: 
				if (!pkb->getAllStmtList(&vecI))
					return false;
				break;
				
			// Known Variables
			case KT_STMT_NUM:			
				if (qt.hasAttribute == true && qt.attributeType == AT_CALL_PROC_NAME) {
				// target is of type call.procName
						
					// gets the call stmt procTable index, then its procName
					targetmap[dep][v].push_back(pkb->getProcName(pkb->getCallee(qv.content)));					
					break; // break only if it is call.procName
					
				} // else drops to known constant case 
			
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
			case KT_CONSTANT_INTEGER:
			case KT_CONSTANT_STRING:
				return false;

		}

		switch (qv.variableType) {
				
			case DT_CALL: 
				if (qt.hasAttribute == true && qt.attributeType == AT_CALL_PROC_NAME) {
				// target attribute type is procName
						
					for (int i = 0; i < vecI.size(); i++) {
					// gets the procTable index of the call stmt 
						vecTemp.push_back(pkb->getCallee(vecI[i])); 
					}
						
					targetmap[dep][v] = pkb->getAllProcName(vecTemp); 
					break; // break only if it is call.procName
					
				} // else drops into cases below

			case DT_ASSIGN: 	
			case DT_WHILE: 		
			case DT_IF:			
			case DT_CONSTANT:	
			case DT_STMT:
			case DT_PROGLINE:
			case DT_STMTLST: 	
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
 * \fn		QueryEvaluator::sortedIndex(vector<int> vec)
 * \brief	Perform cartesian product of 2 vectors. 
 * \param 	vec: vector; 
 * \return 	returns a list containing the index of the vector sorted by its element value non increasing
 */
 list<int> QueryEvaluator::sortIndex(vector<long long> vec) {
	
	list<int> l; 

	if (vec.empty())
		return l;
	else
		l.push_back(0);

	for (int i = 1; i < vec.size(); i++) {
		
		bool inserted = false;

		for (list<int>::iterator it = l.begin(); it != l.end(); ++it) {
			
			if (vec[i] < vec[*it]) {
				l.insert(it,i);
				inserted = true;
				break;
			}
		
		}	

		if (!inserted)
			l.push_back(i);

	}

	return l;
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
 * \fn		QueryEvaluator::removeDuplicatesPair(vector<int>* vecA, vector<int>* vecB)
 * \brief	Removes duplicate pair elements in the vectors. 
 * \param [in] vecA: first vector
 *		  [in] vecB: second vector
 * \return 	true.
 */
 bool QueryEvaluator::removeDuplicatesPair(vector<int>* vecA, vector<int>* vecB) {
	
	set<pair<int, int>> s;
	
	for (int i = 0; i < vecA->size(); i++) {
		s.insert(make_pair(vecA->at(i), vecB->at(i)));
	}

	vecA->clear();
	vecB->clear();

	for (set<pair<int, int>>::iterator it = s.begin(); it != s.end(); ++it) {
		vecA->push_back((*it).first);
		vecB->push_back((*it).second);
	}
	
	return true;
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

/**
 * \fn		QueryEvaluator::getNodeType(int t)
 * \brief	Switch enum (for contains/containsT/sibling) from Query Preprocessor to its PKB equivalent. 
 * \param [in] t: Query Preprocessor enum.  
 * \return 	equivalent PKB enum.
 */
 ASTNodeType QueryEvaluator::getNodeType(int t) {

	 switch (qVariableList[t].variableType) {
	 
		case DT_PROCEDURE:
		case KT_KNOWN_PROCEDURE:
			return AST_PROCEDURE;

		case DT_STMTLST:
			return AST_STATEMENT_LIST;

		case DT_STMT:
		case KT_STMT_NUM:
		case DT_PROGLINE:
			return AST_STMT;

		case DT_ASSIGN:
			return AST_ASSIGNMENT;

		case DT_CALL:
			return AST_CALL;

		case DT_WHILE:
			return AST_WHILE_LOOP;

		case DT_IF:
			return AST_IF_BRANCH;

		case DT_PLUS:
			return AST_PLUS;

		case DT_MINUS:
			return AST_MINUS;

		case DT_TIMES:
			return AST_MULTIPLY;

		case DT_VARIABLE:
		case KT_KNOWN_VARIABLE:
			return AST_VARIABLE;

		case DT_CONSTANT:
		case KT_KNOWN_CONSTANT:
			return AST_CONSTANT;

	 }

 }

 /**
 * \fn		QueryEvaluator::evaluateQuery()
 * \brief	Evaluates query by querying clauses one at a time  
 * \return 	TRUE if evaluation is successful, FALSE otherwise
 
bool QueryEvaluator::evaluateQuery() 
{
	
	for (map<int, vector<QueryClause>>::iterator mit = qClauseList.begin(); mit != qClauseList.end(); ++mit) {
		for (vector<QueryClause>::iterator vit = (*mit).second.begin(); vit != (*mit).second.end(); ++vit) {
			
			if (vit->variable1 == vit->variable2) { // reflexive clause
				
				if (!evaluateReflexiveClause(*vit)) 
					return false; // query clause evaluated to false
				
			} else { // non reflexive clause
				
				if (!evaluateClause(*vit)) 
					return false; // query clause evaluated to false
				
			}
		}
	}
	return true;
}
**/