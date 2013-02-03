/**
 * \file	Follows.cpp
 * \class	Follows
 * \brief	Store all 'Follows' relations in the form (stmt1, stmt2) where 'stmt1' and 'stmt2' are statement numbers. Provide funtions for query on 'Follows' and 'FollowsStar' relations. The storage is referred to as FollowsTable.
 */

#include "stdafx.h"
#include "Follows.h"

int Follows::getFollowsFirst(int stmt)
{
	int size = stmt2s.size();

	for (int i = 0; i < size; i++) {
		if (stmt2s.at(i) == stmt) {
			// found stmt2, return corresponding stmt1
			return(stmt1s.at(i));
		}
	}
	return(-1);
}


int Follows::getFollowsSecond(int stmt)
{
	int size = stmt1s.size();

	for (int i = 0; i < size; i++) {
		if (stmt1s.at(i) == stmt) {
			// found stmt1, return corresponding stmt2
			return(stmt2s.at(i));
		}
	}
	return(-1);
}


STMT_LIST Follows::getFollowsStarFirst(STMT stmt)
{
	vector<int> resultLst;
	int temp = getFollowsFirst(stmt);
	while (temp != -1) {
		resultLst.push_back(temp);
		temp = getFollowsFirst(temp);
	}
	return(resultLst);
}


STMT_LIST Follows::getFollowsStarSecond(STMT stmt)
{
	vector<int> resultLst;
	int temp = getFollowsSecond(stmt);
	while (temp != -1) {
		resultLst.push_back(temp);
		temp = getFollowsSecond(temp);
	}
	return(resultLst);
}


BOOLEAN Follows::isFollows(STMT stmt1, STMT stmt2)
{
	int size = stmt1s.size();

	for (int i = 0; i < size; i++) {
		if (stmt1s.at(i) == stmt1) {
			if (stmt2s.at(i) == stmt2) {
				return(true);
			}else {
				return(false);
			}
		}else if (stmt2s.at(i) == stmt2) {  //stmt1s.at(i)!=stmt1 && stmt2s.at(i)==stmt2
			return(false);
		}
	}
	return(false);
}


BOOLEAN Follows::isFollowsStar(STMT stmt1, STMT stmt2)
{
	int temp = getFollowsSecond(stmt1);

	if (temp == -1) {
		return(false);
	}else if (temp == stmt2) {
		return(true);
	}else {
		return(isFollowsStar(temp, stmt2));
	}
}

/**
 * \fn	void Follows::addFollows(STMT stmt1, STMT stmt2)
 *
 * \brief	Add a 'Follows' relation in the form (stmt1, stmt2) into FollowsTable where 'stmt1' and 'stmt2' are statement numbers.
 * 
 *
 * \param	stmt1	The statement number of the first statement in the relation Follows.
 * \param	stmt2	The statement number of the second statement in the relation Follows.
 *
 * \return	void.
 */
void Follows::addFollows(STMT stmt1, STMT stmt2)
{
	//if(isFollows(stmt1, stmt2)) {
	// Follows(stmt1, stmt2) already in the table, do nothing
	//} else {
	// Follows(stmt1, stmt2) not in the table, add in
	stmt1s.push_back(stmt1);
	stmt2s.push_back(stmt2);
	//}
}

/**
 * \fn	BOOLEAN Follows::follows(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
 *
 * \brief	A function for query on 'Follows' relations.
 * 
 * To handle a query in the form follows(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if FollowsTable is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (stmt1, stmt2) in the FollowsTable where stmt2 equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (stmt1, stmt2) in the FollowsTable where stmt1 equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that (ai, bj) is an entry in FollowsTable, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct values in the second column of FollowsTable.
 *	2b) List A nont-empty, List B empty
 *	Fill up list B with all the values in the second column of FollowsTable where the corresponding first column equals to the elements in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements from list B that are not found in the second column of FollowsTable.
 *	2d) Both lists are non-empty
 *	Find all values (say V2) in the second column of FollowsTable where the corresponding first column value equals to the elements in list A and remove elements from list B that are not in V2.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct values in the first column of FollowsTable.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values in the first column of FollowsTable where the corresponding second column equals to the elements in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements from list A that are not found in the first column of FollowsTable.
 *	3d) Both lists are non-empty
 *	Find all values (say V1) in the first column of FollowsTable where the corresponding second column value equals to the elements in list B and remove elements from list A that are not in V1.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up the two lists with all entries of FollowsTable.
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values in the first column of FollowsTable where the second column equals to bi.
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values in the second column of FollowsTable where the first column equals to ai.
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if (ai, bi) is not an entry in FollowsTable.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	st1s_ptr	A pointer to a statement list, which contains a list of statement numbers. These statements are the first statement of relation Follows.
 * \param	st2s_ptr	A pointer to a statement list, which contains a list of statement numbers. These statements are the second statement of relation Follows.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN Follows::follows(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(follows_00(st1s_ptr, st2s_ptr));

	case 1:
		return(follows_01(st1s_ptr, st2s_ptr));

	case 2:
		return(follows_10(st1s_ptr, st2s_ptr));

	case 3:
		return(follows_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Follows::follows_00(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
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
				if (isFollows(st1s_p->at(i), st2s_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN Follows::follows_01(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < stmt2s.size(); i++) {
			st2s_p->push_back(stmt2s.at(i));
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
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b, st1s_p has no duplicate
		for (int i = 0; i < size1; i++) {
			int val = getFollowsSecond(st1s_p->at(i));
			if (val != -1) {
				st2s_p->push_back(val);
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
		 /*
		  * VectorA (a1, a2, …, am) VectorB (b1, b2, …, bn)
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
				if (isFollows(st1s_p->at(j), st2s_p->at(index))) {
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


BOOLEAN Follows::follows_10(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < stmt1s.size(); i++) {
			st1s_p->push_back(stmt1s.at(i));
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b, st2s_p has no duplicate
		for (int i = 0; i < size2; i++) {
			int val = getFollowsFirst(st2s_p->at(i));
			if (val != -1) {
				st1s_p->push_back(val);
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
		  * VectorA (a1, a2, …, am) VectorB (b1, b2, …, bn)
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
				if (isFollows(st1s_p->at(index), st2s_p->at(j))) {
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


BOOLEAN Follows::follows_11(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
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
		int index = 0;
		for (int i = 0; i < size2; i++) {
			int temp = getFollowsFirst(st2s_p->at(index));
			if (temp == -1) {
				st2s_p->erase(st2s_p->begin() + index);
				// do not increment index
			}else {
				st1s_p->push_back(temp);
				index++;
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			int temp = getFollowsSecond(st1s_p->at(index));
			if (temp == -1) {
				st1s_p->erase(st1s_p->begin() + index);
				// do not increment index
			}else {
				st2s_p->push_back(temp);
				index++;
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isFollows(st1s_p->at(index), st2s_p->at(index))) {
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
 * \fn	BOOLEAN Follows::followsStar(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
 *
 * \brief	A function for query on 'FollowsStar' relations.
 * 
 * To handle a query in the form followsStar(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if FollowsTable is not empty, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one entry (stmt1, stmt2) in the FollowsTable where stmt2 equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one entry (stmt1, stmt2) in the FollowsTable where stmt1 equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that FollowsStar(ai, bj) is true, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct values in the second column of FollowsTable.
 *	2b) List A nont-empty, List B empty
 *	Fill up list B with all the values b if followsStar(a, b) is true where a is any element in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements from list B that are not found in the second column of FollowsTable.
 *	2d) Both lists are non-empty
 *	For each element bi from list B, if no element aj from list A can satisfy FollowsStar(aj, bi), then remove this bi. Otherwise, keep it.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct values in the first column of FollowsTable.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values a if followsStar(a, b) is true where b is any element in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements from list A that are not found in the first column of FollowsTable.
 *	3d) Both lists are non-empty
 *	For each element ai from list A, if no element bj from list B can satisfy FollowsStar(ai, bj), then remove this ai. Otherwise, keep it.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Throw Exception.
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values in the first column of FollowsTable where the second column equals to bi.
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values in the second column of FollowsTable where the first column equals to ai.
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if FollowsStar(ai, bi) is false.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	st1s_ptr	A pointer to a statement list, which contains a list of statement numbers. These statements are the first statement of relation FollowsStar.
 * \param	st2s_ptr	A pointer to a statement list, which contains a list of statement numbers. These statements are the second statement of relation FollowsStar.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN Follows::followsStar(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(followsStar_00(st1s_ptr, st2s_ptr));

	case 1:
		return(followsStar_01(st1s_ptr, st2s_ptr));

	case 2:
		return(followsStar_10(st1s_ptr, st2s_ptr));

	case 3:
		return(followsStar_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}


BOOLEAN Follows::followsStar_00(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
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
				if (isFollowsStar(st1s_p->at(i), st2s_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}


BOOLEAN Follows::followsStar_01(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < stmt2s.size(); i++) {
			st2s_p->push_back(stmt2s.at(i));
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
			vector<int> val = getFollowsStarSecond(st1s_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*st2s_p, current) == -1) {
					st2s_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
		 /*
		  * VectorA (a1, a2, …, am) VectorB (b1, b2, …, bn)
		  * for bi from b1 to bn {
		  * nomatch = true
		  * for aj from a1 to am {
		  * if isRelationStar(aj, bi) is true
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
				if (isFollowsStar(st1s_p->at(j), st2s_p->at(index))) {
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


BOOLEAN Follows::followsStar_10(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < stmt1s.size(); i++) {
			st1s_p->push_back(stmt1s.at(i));
		}

		//for(int i=0; i<stmts.size(); i++) {
		//	int current = stmts.at(i);
		//	if(indexOf(*sts_p, current) == -1)
		//		sts_p->push_back(current);
		//}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			vector<int> val = getFollowsStarFirst(st2s_p->at(i));
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
		  * VectorA (a1, a2, …, am) VectorB (b1, b2, …, bn)
		  * for ai from a1 to am {
		  * nomatch = true
		  * for bj from b1 to bn {
		  * if isRelationStar(ai, bj) is true
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
				if (isFollowsStar(st1s_p->at(index), st2s_p->at(j))) {
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


BOOLEAN Follows::followsStar_11(STMT_LIST *st1s_p, STMT_LIST *st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		throw "arg is 11, but both vectors are empty.";
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> st2s_copy;
		for (int i = 0; i < size2; i++) {
			st2s_copy.push_back(st2s_p->at(i));
		}
		st2s_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleSt2s = st2s_copy.at(i);
			vector<int> temp = getFollowsStarFirst(currentEleSt2s);
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
			vector<int> temp = getFollowsStarSecond(currentEleSt1s);
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
				if (isFollowsStar(st1s_p->at(index), st2s_p->at(index))) {
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


/*
 * BOOLEAN Follows::followsStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr) {
 *      int size1 = st1s_ptr->size();
 *      int size2 = st2s_ptr->size();
 *      if(size1==0 && size2==0) { //case1
 *              if(stmt1s.size()==0)
 *                      return false;
 *              else
 *                      return true;
 *      } else if(size1==0) { //size1==0 && size2!=0, case 2&3
 *              if(size2==1) { //case2
 *                      int firstEleSt2s = st2s_ptr->at(0);
 *                      vector<int> temp = getFollowsStarFirst(firstEleSt2s);
 *                      int tempSize = temp.size();
 *                      if(tempSize==0) {
 *                              st1s_ptr->push_back(-1);
 *                              return false;
 *                      } else {
 *                              st1s_ptr->push_back(temp.at(0));
 *                              for(int i=1; i<tempSize; i++) {
 *                                      st1s_ptr->push_back(temp.at(i));
 *                                      st2s_ptr->push_back(firstEleSt2s);
 *                              }
 *                              return true;
 *                      }
 *              } else { //case3
 *                      bool flag = false; //no relation found yet
 *                      vector<int> st2s_copy;
 *                      for(int i=0; i<size2; i++)
 *                              st2s_copy.push_back(st2s_ptr->at(i));
 *                      st2s_ptr->clear();
 *
 *                      for(int i=0; i<size2; i++) {
 *                              int currentEleSt2s = st2s_copy.at(i);
 *                              vector<int> temp = getFollowsStarFirst(currentEleSt2s);
 *                              int tempSize = temp.size();
 *                              if(tempSize==0) {
 *                                      st1s_ptr->push_back(-1);
 *                                      st2s_ptr->push_back(currentEleSt2s);
 *                              } else {
 *                                      flag=true;
 *                                      for(int j=0; j<tempSize; j++) {
 *                                              st1s_ptr->push_back(temp.at(j));
 *                                              st2s_ptr->push_back(currentEleSt2s);
 *                                      }
 *                              }
 *                      }
 *                      return flag;
 *              } //end of case3
 *
 *      } else if(size2==0) { //size2==0 && size1 !=0, case 4&5
 *              if(size1==1) { //case4
 *                      int firstEleSt1s = st1s_ptr->at(0);
 *                      vector<int> temp = getFollowsStarSecond(firstEleSt1s);
 *                      int tempSize = temp.size();
 *                      if(tempSize==0) {
 *                              st2s_ptr->push_back(-1);
 *                              return false;
 *                      } else {
 *                              st2s_ptr->push_back(temp.at(0));
 *                              for(int i=1; i<tempSize; i++) {
 *                                      st1s_ptr->push_back(firstEleSt1s);
 *                                      st2s_ptr->push_back(temp.at(i));
 *                              }
 *                              return true;
 *                      }
 *              } else { //case5
 *                      bool flag = false; //no relation found yet
 *                      vector<int> st1s_copy;
 *                      for(int i=0; i<size1; i++)
 *                              st1s_copy.push_back(st1s_ptr->at(i));
 *                      st1s_ptr->clear();
 *
 *                      for(int i=0; i<size1; i++) {
 *                              int currentEleSt1s = st1s_copy.at(i);
 *                              vector<int> temp = getFollowsStarSecond(currentEleSt1s);
 *                              int tempSize = temp.size();
 *                              if(tempSize==0) {
 *                                      st1s_ptr->push_back(currentEleSt1s);
 *                                      st2s_ptr->push_back(-1);
 *                              } else {
 *                                      flag=true;
 *                                      for(int j=0; j<tempSize; j++) {
 *                                              st1s_ptr->push_back(currentEleSt1s);
 *                                              st2s_ptr->push_back(temp.at(j));
 *                                      }
 *                              }
 *                      }
 *                      return flag;
 *              } //end of case5
 *      } else { //size1!=0 && size2!=0, case 6,7,8
 *              if(size1==1 && size2==1) { //case 6
 *                      if(isFollowsStar(st1s_ptr->at(0), st2s_ptr->at(0)))
 *                              return true;
 *                      else
 *                              return false;
 *              } else { //case 7&8
 *                      throw "At least one vector has more than one element!";
 *              }
 *      }
 *
 * }
 *
 *
 *
 *
 * void Follows::follows(STMT* stmt1_ptr, STMT* stmt2_ptr) {
 *
 *      if(stmt1_ptr == NULL) {
 *              if(stmt2_ptr == NULL)
 *                      throw exception("Both pointers are NULL when enquiring follows!");
 *              else {
 *                      int stmt1 = getFollowsFirst(*stmt2_ptr);
 *                      if(stmt1 != -1)
 *                              stmt1_ptr = &stmt1;
 *              }
 *      } else {
 *              if(stmt2_ptr != NULL)
 *                      throw exception("Both pointers are non-NULL when enquiring follows!");
 *              else {
 *                      int stmt2 = getFollowsSecond(*stmt1_ptr);
 *                      if(stmt2 != -1)
 *                              stmt2_ptr = &stmt2;
 *              }
 *      }
 * }
 */
