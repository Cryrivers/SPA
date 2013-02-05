/**
 * \file	Calls.cpp
 * \class	Calls
 * \brief	Store all 'Calls' relations in the form (stmt, caller, callee) where 'stmt' is a statement number and both 'caller' and 'callee' are procedure indices. Provide functions for query on this relation. The storage is referred as CallTable. The first column of CallTable means the procedures that call another procedure (callers), the second column of CallTable means the procedures that is called by another procedure (callees).
 */

#include "stdafx.h"
#include "Calls.h"
#include "Utility.h"

BOOLEAN Calls::isCalls(PROC_INDEX caller, PROC_INDEX callee)
{
	int size = callers.size();

	for (int i = 0; i < size; i++) {
		if ((callers.at(i) == caller) && (callees.at(i) == callee)) {
			return(true);
		}
	}
	return(false);
}


PROC_INDEX_LIST Calls::getCallsFirst(PROC_INDEX callee)
{
	vector<int> resultLst;

	int size = callees.size();
	for (int i = 0; i < size; i++) {
		if (callees.at(i) == callee) {
			// one match found, add in the result list
			resultLst.push_back(callers.at(i));
		}
	}
	return (resultLst);
}


PROC_INDEX_LIST Calls::getCallsSecond(PROC_INDEX caller)
{
	vector<int> resultLst;

	int size = callers.size();
	for (int i = 0; i < size; i++) {
		if (callers.at(i) == caller) {
			// one match found, add in the result list
			resultLst.push_back(callees.at(i));
		}
	}
	return (resultLst);
}

/**
 * \fn	void addCalls(STMT stmt, PROC_INDEX caller, PROC_INDEX callee);
 *
 * \brief	Add a new 'Calls' relation in the form (stmt, caller, callee) where 'stmt' is a statement number and both 'caller' and 'callee' are procedure indices. If the relation already exists, do nothing.
 * 
 *
 * \param	stmt	The statement number of this call statement.
 * \param	caller	The index of the procedure that is calling in the procedure table.
 * \param	callee	The index of the procedure that is being called in the procedure table.
 *
 * \return	void.
 */
void Calls::addCalls(STMT stmt, PROC_INDEX caller, PROC_INDEX callee)
{
	int index = indexOf(stmts,stmt);

	if (index >=0) {
		// Calls(caller, callee) already in the table, do nothing
	}else {
		// Calls(stmt, proc) not in the table, add in
		stmts.push_back(stmt);
		callers.push_back(caller);
		callees.push_back(callee);
	}
}

/**
 * \fn	BOOLEAN Calls::calls(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg)
 *
 * \brief	A function for query on 'Calls' relations.
 * 
 * To handle a query in the form calls(List A, List B, arg).
 * Case 1: arg = 00 = 0
 *  1a) Both lists are empty
 *	Return TRUE if CallTable is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (caller, callee) in the CallTable where callee equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (caller, callee) in the CallTable where caller equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that (ai, bj) is an entry in CallTable, FASLE otherwise.
 *
 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct values in the second column of CallTable.
 *	2b) List A nont-empty, List B empty
 *	Fill up list B with all the values in the second column of CallTable where the corresponding first column equals to the elements in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements from list B that are not found in the second column of CallTable.
 *	2d) Both lists are non-empty
 *	Find all values (say V2) in the second column of CallTable where the corresponding first column value equals to the elements in list A and remove elements from list B that are not in V2.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.
 *
 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct values in the first column of CallTable.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values in the first column of CallTable where the corresponding second column equals to the elements in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements from list A that are not found in the first column of CallTable.
 *	3d) Both lists are non-empty
 *	Find all values (say V1) in the first column of CallTable where the corresponding second column value equals to the elements in list B and remove elements from list A that are not in V1.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.
 *
 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up the two lists with all entries of CallTable.
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values in the first column of CallTable where the second column equals to bi.
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values in the second column of CallTable where the first column equals to ai.
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if (ai, bi) is not an entry in CallTable.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 * Case: arg = other values
 *	Exception
 *
 * \param	callers_ptr	A pointer to a list of indices of procedures that call another procedure.
 * \param	callees_ptr	A pointer to list of indices of procedures that is called by another procedure.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN Calls::calls(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(calls_00(callers_ptr, callees_ptr));

	case 1:
		return(calls_01(callers_ptr, callees_ptr));

	case 2:
		return(calls_10(callers_ptr, callees_ptr));

	case 3:
		return(calls_11(callers_ptr, callees_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Calls::calls_00(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p)
{
	int size1 = callers_p->size();
	int size2 = callees_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		if (callers.size() == 0) {
			return(false);
		}else {
			return(true);
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < callees.size(); i++) {
			for (int j = 0; j < size2; j++) {
				if (callees.at(i) == callees_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < callers.size(); i++) {
			for (int j = 0; j < size1; j++) {
				if (callers.at(i) == callers_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < size1; i++) {
			for (int j = 0; j < size2; j++) {
				if (isCalls(callers_p->at(i), callees_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN Calls::calls_01(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p)
{
	int size1 = callers_p->size();
	int size2 = callees_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < callees.size(); i++) {
			int current = callees.at(i);
			if (indexOf(*callees_p, current) == -1) {
				callees_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			if (indexOf(callees, callees_p->at(index)) == -1) {
				callees_p->erase(callees_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b
		for (int i = 0; i < size1; i++) {
			vector<int> val = getCallsSecond(callers_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*callees_p, current) == -1) {
					callees_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
		 /*
		  * VectorA (a1, a2, бн, am) VectorB (b1, b2, бн, bn)
		  * for bi from b1 to bn {
		  * nomatch = true
		  * for aj from a1 to am {
		  * if isRelation(aj, bi) is true
		  *      nomatch = false
		  *      break
		  * }
		  * if (nomatch)
		  * remove bi
		  * }
		  */
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isCalls(callers_p->at(j), callees_p->at(index))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				callees_p->erase(callees_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (callees_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

BOOLEAN Calls::calls_10(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p)
{
	int size1 = callers_p->size();
	int size2 = callees_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < callers.size(); i++) {
			int current = callers.at(i);
			if (indexOf(*callers_p, current) == -1) {
				callers_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			vector<int> val = getCallsFirst(callees_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*callers_p, current) == -1) {
					callers_p->push_back(current);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			if (indexOf(callers, callers_p->at(index)) == -1) {
				callers_p->erase(callers_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
		 /*
		  * VectorA (a1, a2, бн, am) VectorB (b1, b2, бн, bn)
		  * for ai from a1 to am {
		  * nomatch = true
		  * for bj from b1 to bn {
		  * if isRelation(ai, bj) is true
		  *      nomatch = false
		  *      break
		  * }
		  * if (nomatch)
		  * remove ai
		  * }
		  */
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isCalls(callers_p->at(index), callees_p->at(j))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				callers_p->erase(callers_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (callers_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}


BOOLEAN Calls::calls_11(PROC_INDEX_LIST* callers_p, PROC_INDEX_LIST* callees_p)
{
	int size1 = callers_p->size();
	int size2 = callees_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < callers.size(); i++) {
			callers_p->push_back(callers.at(i));
			callees_p->push_back(callees.at(i));
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> callees_copy;
		for (int i = 0; i < size2; i++) {
			callees_copy.push_back(callees_p->at(i));
		}
		callees_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleCallees = callees_copy.at(i);
			vector<int> temp = getCallsFirst(currentEleCallees);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					callers_p->push_back(temp.at(j));
					callees_p->push_back(currentEleCallees);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		vector<int> callers_copy;
		for (int i = 0; i < size1; i++) {
			callers_copy.push_back(callers_p->at(i));
		}
		callers_p->clear();

		for (int i = 0; i < size1; i++) {
			int currentEleCallers = callers_copy.at(i);
			vector<int> temp = getCallsSecond(currentEleCallers);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					callers_p->push_back(currentEleCallers);
					callees_p->push_back(temp.at(j));
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isCalls(callers_p->at(index), callees_p->at(index))) {
					index++;
				}else {
					callers_p->erase(callers_p->begin() + index);
					callees_p->erase(callees_p->begin() + index);
				}
			}
		}else {  //exception
			throw "arg is 11, but sizes of both vectors are not the same.";
		}
	}
	if (callers_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

/**
 * \fn	PROC_INDEX_LIST Calls::getAllCallers()
 *
 * \brief	Return all the procedure indices stored in first column of CallTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	PROC_INDEX_LIST.
 */
PROC_INDEX_LIST Calls::getAllCallers()
{
	return(callers);
}

/**
 * \fn	PROC_INDEX_LIST Calls::getAllCallees()
 *
 * \brief	Return all the procedure indices stored in second column of CallTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	PROC_INDEX_LIST.
 */
PROC_INDEX_LIST Calls::getAllCallees()
{
	return(callees);
}

/**
 * \fn	PROC_INDEX Calls::getCallee(STMT stmt)
 *
 * \brief	Given a statement number (must be a call statement), find the procedure that is called in this call statement.
 * 
 *
 * 
 * 
 *
 * \return	The procedure index of the found procedure. If not found, return -1.
 */
PROC_INDEX Calls::getCallee(STMT stmt)
{
	int size = stmts.size();
	for (int i = 0; i < size; i++) {
		if (stmts.at(i) == stmt) {
			// match found, return
			return (callees.at(i));
		}
	}
	return(-1);
}
