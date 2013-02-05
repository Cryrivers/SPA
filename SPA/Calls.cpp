/**
 * \file	Calls.cpp
 * \class	Calls
 * \brief	Store all 'Calls' relations in the form (stmt, proc) where 'stmt' is a statement number and 'proc' is a procedure index. Provide funtions for query on this relation. The storage is referred as CallTable.
 */

#include "stdafx.h"
#include "Calls.h"

BOOLEAN Calls::isCalls(STMT stmt, PROC_INDEX proc)
{
	int size = stmts.size();

	for (int i = 0; i < size; i++) {
		if ((stmts.at(i) == stmt) && (procs.at(i) == proc)) {
			return(true);
		}
	}
	return(false);
}


STMT_LIST Calls::getCallsFirst(PROC_INDEX proc)
{
	vector<int> resultLst;

	int size = procs.size();
	for (int i = 0; i < size; i++) {
		if (procs.at(i) == proc) {
			// one match found, add in the result list
			resultLst.push_back(stmts.at(i));
		}
	}
	return(resultLst);
}


PROC_INDEX Calls::getCallsSecond(STMT stmt)
{
	int size = stmts.size();
	for (int i = 0; i < size; i++) {
		if (stmts.at(i) == stmt) {
			// match found, return
			return (procs.at(i));
		}
	}
	return(-1);
}

/**
 * \fn	void Calls::addCalls(STMT stmt, PROC_INDEX proc)
 *
 * \brief	Add a new 'Calls' relation in the form (stmt, proc) to Calls where 'stmt' is a statement number and 'proc' is a procedure index. If the relation already exists, do nothing.
 * 
 *
 * \param	stmt	The statement number of this call statement.
 * \param	proc	The index of the procedure that is being called in the procedure table.
 *
 * \return	void.
 */
void Calls::addCalls(STMT stmt, PROC_INDEX proc)
{
	if (isCalls(stmt, proc)) {
		// Calls(stmt, proc) already in the table, do nothing
	}else {
		// Calls(stmt, proc) not in the table, add in
		stmts.push_back(stmt);
		procs.push_back(proc);
	}
}

/**
 * \fn	BOOLEAN Calls::calls(STMT_LIST* sts_ptr, PROC_INDEX_LIST* ps_ptr, int arg)
 *
 * \brief	A function for query on 'Calls' relations.
 * 
 * To handle a query in the form calls(List A, List B, arg).
 * In Iteration 1, only part of this function is implemented, i.e. when arg is 0.
 * Case 1: arg = 00 = 0
 *  1a) Both lists are empty
 *	Return TRUE if Calls is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (stmt, proc) in the Calls where proc equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (stmt, proc) in the Calls where stmt equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that (ai, bj) is an entry in Calls, FASLE otherwise.
 * Case: arg = other values
 *	Exception
 *
 * \param	sts_ptr	A pointer to a statement list, which contains a list of statement numbers.
 * \param	ps_ptr	A pointer to a procedure index list.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN Calls::calls(STMT_LIST* sts_ptr, PROC_INDEX_LIST* ps_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(calls_00(sts_ptr, ps_ptr));

	//case 1:
	//	return(modifies_01(sts_ptr, vs_ptr));

	//case 2:
	//	return(modifies_10(sts_ptr, vs_ptr));

	//case 3:
	//	return(modifies_11(sts_ptr, vs_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Calls::calls_00(STMT_LIST* sts_p, PROC_INDEX_LIST* ps_p)
{
	int size1 = sts_p->size();
	int size2 = ps_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		if (stmts.size() == 0) {
			return(false);
		}else {
			return(true);
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < procs.size(); i++) {
			for (int j = 0; j < size2; j++) {
				if (procs.at(i) == ps_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < stmts.size(); i++) {
			for (int j = 0; j < size1; j++) {
				if (stmts.at(i) == sts_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < size1; i++) {
			for (int j = 0; j < size2; j++) {
				if (isCalls(sts_p->at(i), ps_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}

/*
BOOLEAN Calls::modifies_01(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < vars.size(); i++) {
			int current = vars.at(i);
			if (indexOf(*vs_p, current) == -1) {
				vs_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			if (indexOf(vars, vs_p->at(index)) == -1) {
				vs_p->erase(vs_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b
		for (int i = 0; i < size1; i++) {
			vector<int> val = getModifiesSecond(sts_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*vs_p, current) == -1) {
					vs_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
	*/

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

/*
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isModifies(sts_p->at(j), vs_p->at(index))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				vs_p->erase(vs_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (vs_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}
*/
/*
BOOLEAN Calls::modifies_10(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < stmts.size(); i++) {
			int current = stmts.at(i);
			if (indexOf(*sts_p, current) == -1) {
				sts_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			vector<int> val = getModifiesFirst(vs_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*sts_p, current) == -1) {
					sts_p->push_back(current);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			if (indexOf(stmts, sts_p->at(index)) == -1) {
				sts_p->erase(sts_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
	*/
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
/*
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isModifies(sts_p->at(index), vs_p->at(j))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				sts_p->erase(sts_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (sts_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}
*/
/*

BOOLEAN Calls::modifies_11(STMT_LIST *sts_p, VAR_INDEX_LIST *vs_p)
{
	int size1 = sts_p->size();
	int size2 = vs_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < stmts.size(); i++) {
			sts_p->push_back(stmts.at(i));
			vs_p->push_back(vars.at(i));
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> vs_copy;
		for (int i = 0; i < size2; i++) {
			vs_copy.push_back(vs_p->at(i));
		}
		vs_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleVs = vs_copy.at(i);
			vector<int> temp = getModifiesFirst(currentEleVs);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					sts_p->push_back(temp.at(j));
					vs_p->push_back(currentEleVs);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		vector<int> sts_copy;
		for (int i = 0; i < size1; i++) {
			sts_copy.push_back(sts_p->at(i));
		}
		sts_p->clear();

		for (int i = 0; i < size1; i++) {
			int currentEleSts = sts_copy.at(i);
			vector<int> temp = getModifiesSecond(currentEleSts);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					sts_p->push_back(currentEleSts);
					vs_p->push_back(temp.at(j));
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isModifies(sts_p->at(index), vs_p->at(index))) {
					index++;
				}else {
					sts_p->erase(sts_p->begin() + index);
					vs_p->erase(vs_p->begin() + index);
				}
			}
		}else {  //exception
			throw "arg is 11, but sizes of both vectors are not the same.";
		}
	}
	if (sts_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}
*/

/**
 * \fn	STMT_LIST Calls::getAllCallsFirst()
 *
 * \brief	Return all the statement numbers stored in Calls as a list.
 * 
 *
 * 
 * 
 *
 * \return	STMT_LIST.
 */
STMT_LIST Calls::getAllCallsFirst()
{
	return(stmts);
}

/**
 * \fn	PROC_INDEX_LIST Calls::getAllCallsSecond()
 *
 * \brief	Return all the procedure indices stored in Calls as a list.
 * 
 *
 * 
 * 
 *
 * \return	PROC_INDEX_LIST.
 */
PROC_INDEX_LIST Calls::getAllCallsSecond()
{
	return(procs);
}
