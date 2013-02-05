/**
 * \file	Next.cpp
 * \class	Next
 * \brief	Store all 'Next' relations in the form (stmt1, stmt2) where both 'stmt1' and 'stmt2' are statement numbers. Provide functions for query on Next relation and NextStar relation. The storage is referred as NextTable.
 */

#include "stdafx.h"
#include "Next.h"
#include "Utility.h"

BOOLEAN Next::isNext(STMT stmt1, STMT stmt2)
{
	int size = stmt1s.size();

	for (int i = 0; i < size; i++) {
		if ((stmt1s.at(i) == stmt1) && (stmt2s.at(i) == stmt2)) {
			return(true);
		}
	}
	return(false);
}

STMT_LIST Next::getNextFirst(STMT stmt2)
{
	vector<int> resultLst;

	int size = stmt2s.size();
	for (int i = 0; i < size; i++) {
		if (stmt2s.at(i) == stmt2) {
			// one match found, add in the result list
			resultLst.push_back(stmt1s.at(i));
		}
	}
	return (resultLst);
}


STMT_LIST Next::getNextSecond(STMT stmt1)
{
	vector<int> resultLst;

	int size = stmt1s.size();
	for (int i = 0; i < size; i++) {
		if (stmt1s.at(i) == stmt1) {
			// one match found, add in the result list
			resultLst.push_back(stmt2s.at(i));
		}
	}
	return (resultLst);
}


/**
 * \fn	void Next::addNext(STMT stmt1, STMT stmt2);
 *
 * \brief	Add a new 'Next' relation in the form (stmt1, stmt2) both 'stmt1' and 'stmt2' are statement numbers. If the relation already exists, do nothing.
 * 
 *
 * \param	stmt1	The statement number of the statement that is before the other in the relation Next.
 * \param	stmt2	The statement number of the statement that is after the other in the relation Next.
 *
 * \return	void.
 */
void Next::addNext(STMT stmt1, STMT stmt2)
{
	if(isNext(stmt1, stmt2)) {
	// Next(stmt1, stmt2) already in the table, do nothing
	} else {
	// Next(stmt1, stmt2) not in the table, add in
	stmt1s.push_back(stmt1);
	stmt2s.push_back(stmt2);
	}
}

/**
 * \fn	BOOLEAN Next::next(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
 *
 * \brief	A function for query on 'Next' relations.
 * 
 * To handle a query in the form next(List A, List B, arg).
 * Case 1: arg = 00 = 0
 *  1a) Both lists are empty
 *	Return TRUE if NextTable is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (stmt1, stmt2) in the NextTable where stmt2 equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (stmt1, stmt2) in the NextTable where stmt1 equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that (ai, bj) is an entry in NextTable, FASLE otherwise.
 *
 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct values in the second column of NextTable.
 *	2b) List A nont-empty, List B empty
 *	Fill up list B with all the values in the second column of NextTable where the corresponding first column equals to the elements in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements from list B that are not found in the second column of NextTable.
 *	2d) Both lists are non-empty
 *	Find all values (say V2) in the second column of NextTable where the corresponding first column value equals to the elements in list A and remove elements from list B that are not in V2.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.
 *
 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct values in the first column of NextTable.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values in the first column of NextTable where the corresponding second column equals to the elements in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements from list A that are not found in the first column of NextTable.
 *	3d) Both lists are non-empty
 *	Find all values (say V1) in the first column of NextTable where the corresponding second column value equals to the elements in list B and remove elements from list A that are not in V1.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.
 *
 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up the two lists with all entries of NextTable.
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values in the first column of NextTable where the second column equals to bi.
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values in the second column of NextTable where the first column equals to ai.
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if (ai, bi) is not an entry in NextTable.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 * Case: arg = other values
 *	Exception
 *
 * \param	callers_ptr	A pointer to a list of indices of procedures that call another procedure.
 * \param	callees_ptr	A pointer to list of indices of procedures that is called by another procedure.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN Next::next(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(next_00(st1s_ptr, st2s_ptr));

	case 1:
		return(next_01(st1s_ptr, st2s_ptr));

	case 2:
		return(next_10(st1s_ptr, st2s_ptr));

	case 3:
		return(next_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Next::next_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		if (stmt1s.size() == 0) {
			return(false);
		}else {
			return(true);
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < stmt2s.size(); i++) {
			for (int j = 0; j < size2; j++) {
				if (stmt2s.at(i) == st2s_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < stmt1s.size(); i++) {
			for (int j = 0; j < size1; j++) {
				if (stmt1s.at(i) == st1s_p->at(j)) {
					return(true);
				}
			}
		}
		return(false);
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < size1; i++) {
			for (int j = 0; j < size2; j++) {
				if (isNext(st1s_p->at(i), st2s_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN Next::next_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < stmt2s.size(); i++) {
			int current = stmt2s.at(i);
			if (indexOf(*st2s_p, current) == -1) {
				st1s_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			if (indexOf(stmt2s, st2s_p->at(index)) == -1) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b
		for (int i = 0; i < size1; i++) {
			vector<int> val = getNextSecond(st1s_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*st2s_p, current) == -1) {
					st2s_p->push_back(current);
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
				if (isNext(st1s_p->at(j), st2s_p->at(index))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (st2s_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

BOOLEAN Next::next_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < stmt1s.size(); i++) {
			int current = stmt1s.at(i);
			if (indexOf(*st1s_p, current) == -1) {
				st1s_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			vector<int> val = getNextFirst(st2s_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*st1s_p, current) == -1) {
					st1s_p->push_back(current);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			if (indexOf(stmt1s, st1s_p->at(index)) == -1) {
				st1s_p->erase(st1s_p->begin() + index);
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
				if (isNext(st1s_p->at(index), st2s_p->at(j))) {
					noMatch = false;
					break;
				}
			}
			if (noMatch) {
				st1s_p->erase(st1s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}

	if (st1s_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}


BOOLEAN Next::next_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < stmt1s.size(); i++) {
			st1s_p->push_back(stmt1s.at(i));
			st2s_p->push_back(stmt2s.at(i));
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> st2s_copy;
		for (int i = 0; i < size2; i++) {
			st2s_copy.push_back(st2s_p->at(i));
		}
		st2s_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleSt2s = st2s_copy.at(i);
			vector<int> temp = getNextFirst(currentEleSt2s);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					st1s_p->push_back(temp.at(j));
					st2s_p->push_back(currentEleSt2s);
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		vector<int> st1s_copy;
		for (int i = 0; i < size1; i++) {
			st1s_copy.push_back(st1s_p->at(i));
		}
		st1s_p->clear();

		for (int i = 0; i < size1; i++) {
			int currentEleSt1s = st1s_copy.at(i);
			vector<int> temp = getNextSecond(currentEleSt1s);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					st1s_p->push_back(currentEleSt1s);
					st2s_p->push_back(temp.at(j));
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isNext(st1s_p->at(index), st2s_p->at(index))) {
					index++;
				}else {
					st1s_p->erase(st1s_p->begin() + index);
					st2s_p->erase(st2s_p->begin() + index);
				}
			}
		}else {  //exception
			throw "arg is 11, but sizes of both vectors are not the same.";
		}
	}
	if (st1s_p->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}


/**
 * \fn	STMT_LIST Next::getAllNextFirst()
 *
 * \brief	Return all the statement numbers stored in first column of NextTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	STMT_LIST.
 */
STMT_LIST Next::getAllNextFirst()
{
	return(stmt1s);
}

/**
 * \fn	STMT_LIST Next::getAllNextSecond()
 *
 * \brief	Return all the statement numbers stored in second column of NextTable as a list.
 * 
 *
 * 
 * 
 *
 * \return	STMT_LIST.
 */
STMT_LIST Next::getAllNextSecond()
{
	return(stmt2s);
}