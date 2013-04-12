/**
 * \file	AffectsBip.cpp
 * \class	AffectsBip
 * \brief	Provide functions for query on AffectsBip relation and AffectsBipStar relation.
 */

#include "stdafx.h"
#include "AffectsBip.h"
#include "Utility.h"
#include "PKBController.h"


AffectsBip::AffectsBip(void)
{
	_pkb = PKBController::createInstance();
}

BOOLEAN AffectsBip::isAffectsBip(STMT stmt1, STMT stmt2)
{
	return true;
}


STMT_LIST AffectsBip::getAffectsBipFirst(STMT stmt2, BOOLEAN exhaustive)
{
	vector<int> re;
	return re;
}

STMT_LIST AffectsBip::getAffectsBipSecond(STMT stmt1, BOOLEAN exhaustive)
{
	vector<int> re;
	return re;
}

/**
 * \fn	BOOLEAN AffectsBip::affectsBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
 *
 * \brief	A function for query on 'AffectsBip' relation.
 * 
 * To handle a query in the form AffectsBip(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if there exists a pair <a,b> such that AffectsBip(a,b) is true, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one pair (a, b) such that AffectsBip(a,b) holds where b equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one pair (a, b) such that AffectsBip(a,b) holds where a equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that AffectsBip(ai, bj) is true, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct bi values where AffectsBip(_, bi) holds.
 *	2b) List A non-empty, List B empty
 *	Fill up list B with all the values b if AffectsBip(a, b) is true where a is any element in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements bi from list B if AffectsBip(_,bi) does not hold.
 *	2d) Both lists are non-empty
 *	For each element bi from list B, if no element aj from list A can satisfy AffectsBip(aj, bi), then remove this bi. Otherwise, keep it.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct ai values where AffectsBip(ai, _) holds.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values a if AffectsBip(a, b) is true where b is any element in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements ai from list A if AffectsBip(ai,_) does not hold.
 *	3d) Both lists are non-empty
 *	For each element ai from list A, if no element bj from list B can satisfy AffectsBip(ai, bj), then remove this ai. Otherwise, keep it.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up both lists with all pairs of (ai, bi) that satisfy AffectsBip(ai, bi).
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values ai such that AffectsBip(ai, bi).
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values bi such that AffectsBip(ai, bi).
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if AffectsBip(ai, bi) is false.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	st1s_ptr	 A pointer to a list of statement numbers. These statements are the first parameter of relation AffectsBip.
 * \param	st2s_ptr	 A pointer to a list of statement numbers. These statements are the second parameter of relation AffectsBip.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN AffectsBip::affectsBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(affectsBip_00(st1s_ptr, st2s_ptr));

	case 1:
		return(affectsBip_01(st1s_ptr, st2s_ptr));

	case 2:
		return(affectsBip_10(st1s_ptr, st2s_ptr));

	case 3:
		return(affectsBip_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}

BOOLEAN AffectsBip::affectsBip_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 1a
		STMT_LIST allAssign;
		_pkb->getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(allAssign.at(i), false); //exhaustive is false, stops once one assign found
			if(affectedBipAssigns.size()>0) //one pair of a1, a2 satisfy AffectsBip(a1,a2)
				return true;
		}
		return false;
	} else if(size1==0 && size2>0) { //case 1b
		for(int i=0; i<size2; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(st2s_p->at(i), false);
			if(affectBipAssigns.size()>0)
				return true;
		}
		return false;
	} else if(size1>0 && size2==0) { //case 1c
		for(int i=0; i<size1; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(st1s_p->at(i), false);
			if(affectedBipAssigns.size()>0)
				return true;
		}
		return false;
	} else { //case 1d
		for (int i = 0; i < size1; i++)
		{
			for (int j = 0; j < size2; j++)
			{	
				if(isAffectsBip(st1s_p->at(i),st2s_p->at(j))) return true;
			}
		}
		return false;
	}
}

BOOLEAN AffectsBip::affectsBip_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 2a
		STMT_LIST allAssign;
		_pkb->getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectBipAssigns.size()>0)
				st2s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(st2s_p->at(index), false);
			if(affectBipAssigns.size()==0) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	} else if(size1>0 && size2==0) { //case 2b
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(st1s_p->at(i), true);
			for (int j = 0; j < affectedBipAssigns.size(); j++) {
				st2s_p->push_back(affectedBipAssigns.at(j));
			}
		}
	} else { //case 2d
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isAffectsBip(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN AffectsBip::affectsBip_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 3a
		STMT_LIST allAssign;
		_pkb->getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectedBipAssigns.size()>0)
				st1s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 3b
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(st2s_p->at(i), true);
			for (int j = 0; j < affectBipAssigns.size(); j++) {
				st1s_p->push_back(affectBipAssigns.at(j));
			}
		}	
	} else if(size1>0 && size2==0) { //case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(st1s_p->at(index), false);
			if(affectedBipAssigns.size()==0) {
				st1s_p->erase(st1s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	} else { //case 3d
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isAffectsBip(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN AffectsBip::affectsBip_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		STMT_LIST allAssigns;
		_pkb->getAllAssignment(&allAssigns);
		for(int i=0; i<allAssigns.size(); i++) {
			int current = allAssigns.at(i);
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(current, true);
			for(int j=0; j<affectedBipAssigns.size(); j++) {
				st1s_p->push_back(current);
				st2s_p->push_back(affectedBipAssigns.at(j));
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> st2s_copy;
		for (int i = 0; i < size2; i++) {
			st2s_copy.push_back(st2s_p->at(i));
		}
		st2s_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleSt2s = st2s_copy.at(i);
			vector<int> temp = getAffectsBipFirst(currentEleSt2s,true);
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
			vector<int> temp = getAffectsBipSecond(currentEleSt1s, true);
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
				if (isAffectsBip(st1s_p->at(index), st2s_p->at(index))) {
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

BOOLEAN AffectsBip::isAffectsBipStar(STMT stmt1, STMT stmt2)
{
	STMT_LIST visited; //the visited stmt list
	BOOLEAN result = false; //result initialised to false

	isAffectsBipStarHelper(stmt1, stmt2, &visited, &result);
	return result;
}


void AffectsBip::isAffectsBipStarHelper(STMT stmt1, STMT stmt2, STMT_LIST* visited_p, BOOLEAN* result_p)
{
	STMT_LIST directAffectedBipLst = getAffectsBipSecond(stmt1, true);

	if (directAffectedBipLst.size() == 0) { // no more successor, base case, stop and return
		return;
	}else {
		int directAffectedBipLstSize = directAffectedBipLst.size();
		for (int i = 0; i < directAffectedBipLstSize; i++) { // loop through each direct successor
			int currentSecond = directAffectedBipLst.at(i);
			if(indexOf((*visited_p), currentSecond) >= 0){
				//current successor has been visited, do not visit it again
			} else {
				visited_p->push_back(currentSecond); //add this successor into visited list
				if(currentSecond == stmt2) { //match found, set result to be true and stop
					(*result_p) = true;
					return;
				}
				isAffectsBipStarHelper(currentSecond,stmt2, visited_p, result_p); // let this successor check all its direct/indirect successors
				if((*result_p) == true)
					return;
			}
		}
	}
}



STMT_LIST AffectsBip::getAffectsBipStarFirst(STMT stmt2, BOOLEAN exhaustive)
{
	STMT_LIST resultLst; //the result list as well as visited stmt list

	if(!exhaustive) { //stop once one result is found
		STMT_LIST directAffectsBipLst = getAffectsBipFirst(stmt2, false);
		int size = directAffectsBipLst.size();
		if(size == 0) { //no direct Affects result, which means no Affects* result
			return resultLst;
		} else { //Affects result found, which also satisfy Affects*, since search is non-exhaustive, add one result and return
			resultLst.push_back(directAffectsBipLst.at(0));
			return resultLst;
		}
	} else { //find all results
		getAffectsBipStarFirstHelper(stmt2, &resultLst);
		return resultLst;
	}
}

void AffectsBip::getAffectsBipStarFirstHelper(STMT stmt2, STMT_LIST* resultLst_p)
{
	STMT_LIST directAffectBipLst = getAffectsBipFirst(stmt2, true);

	if (directAffectBipLst.size() == 0) { // no more predecessor, base case, stop and return
		return;
	}else {
		int directAffectBipLstSize = directAffectBipLst.size();
		for (int i = 0; i < directAffectBipLstSize; i++) { // loop through each direct predecessor
			int currentFirst = directAffectBipLst.at(i);
			if(indexOf((*resultLst_p), currentFirst) >= 0){
				//current predecessor is in the result list, which means it has been visited, do not visit it again
			} else {
				resultLst_p->push_back(currentFirst); //add this predecessor into result list
				getAffectsBipStarFirstHelper(currentFirst, resultLst_p); // let this predecessor find all its direct/indirect predecessors
			}
		}
	}
}


STMT_LIST AffectsBip::getAffectsBipStarSecond(STMT stmt1, BOOLEAN exhaustive)
{
	STMT_LIST resultLst; //the result list as well as visited stmt list

	if(!exhaustive) { //stop once one result is found
		STMT_LIST directAffectedBipLst = getAffectsBipSecond(stmt1, false);
		int size = directAffectedBipLst.size();
		if(size == 0) { //no direct Affects result, which means no Affects* result
			return resultLst;
		} else { //Affects result found, which also satisfy Affects*, since search is non-exhaustive, add one result and return
			resultLst.push_back(directAffectedBipLst.at(0));
			return resultLst;
		}
	} else { //find all results
		getAffectsBipStarSecondHelper(stmt1, &resultLst);
		return resultLst;
	}
}



void AffectsBip::getAffectsBipStarSecondHelper(STMT stmt1, STMT_LIST* resultLst_p)
{
	STMT_LIST directAffectedBipLst = getAffectsBipSecond(stmt1, true);

	if (directAffectedBipLst.size() == 0) { // no more successor, base case, stop and return
		return;
	}else {
		int directAffectedBipLstSize = directAffectedBipLst.size();
		for (int i = 0; i < directAffectedBipLstSize; i++) { // loop through each direct successor
			int currentSecond = directAffectedBipLst.at(i);
			if(indexOf((*resultLst_p), currentSecond) >= 0){
				//current successor is in the result list, which means it has been visited, do not visit it again
			} else {
				resultLst_p->push_back(currentSecond); //add this successor into result list
				getAffectsBipStarSecondHelper(currentSecond, resultLst_p); // let this successor find all its direct/indirect successors
			}
		}
	}
}



/**
 * \fn	BOOLEAN AffectsBip::affectsBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
 *
 * \brief	A function for query on 'AffectsBipStar' relation.
 * 
 * To handle a query in the form AffectsBipStar(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if there exists a pair <a,b> such that AffectsBip*(a,b) is true, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one pair (a, b) such that AffectsBip*(a,b) holds where b equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one pair (a, b) such that AffectsBip*(a,b) holds where a equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that AffectsBip*(ai, bj) is true, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct bi values where AffectsBip*(_, bi) holds.
 *	2b) List A non-empty, List B empty
 *	Fill up list B with all the values b if AffectsBip*(a, b) is true where a is any element in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements bi from list B if AffectsBip*(_,bi) does not hold.
 *	2d) Both lists are non-empty
 *	For each element bi from list B, if no element aj from list A can satisfy AffectsBip*(aj, bi), then remove this bi. Otherwise, keep it.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct ai values where AffectsBip*(ai, _) holds.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values a if AffectsBip*(a, b) is true where b is any element in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements ai from list A if AffectsBip*(ai,_) does not hold.
 *	3d) Both lists are non-empty
 *	For each element ai from list A, if no element bj from list B can satisfy AffectsBip*(ai, bj), then remove this ai. Otherwise, keep it.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up both lists with all pairs of (ai, bi) that satisfy AffectsBip*(ai, bi).
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values ai such that AffectsBip*(ai, bi).
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values bi such that AffectsBip*(ai, bi).
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if AffectsBip*(ai, bi) is false.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	st1s_ptr	 A pointer to a list of statement numbers. These statements are the first parameter of relation AffectsBipStar.
 * \param	st2s_ptr	 A pointer to a list of statement numbers. These statements are the second parameter of relation AffectsBipStar.
 *
 * \return	A Boolean value as specified in the detailed description.
 */

BOOLEAN AffectsBip::affectsBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(affectsBipStar_00(st1s_ptr, st2s_ptr));

	case 1:
		return(affectsBipStar_01(st1s_ptr, st2s_ptr));

	case 2:
		return(affectsBipStar_10(st1s_ptr, st2s_ptr));

	case 3:
		return(affectsBipStar_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}



BOOLEAN AffectsBip::affectsBipStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 1a
		STMT_LIST allAssign;
		_pkb->getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(allAssign.at(i), false); //exhaustive is false, stops once one assign found
			if(affectedBipAssigns.size()>0) //one pair of a1, a2 satisfy Affects(a1,a2)
				return true;
		}
		return false;
	} else if(size1==0 && size2>0) { //case 1b
		for(int i=0; i<size2; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(st2s_p->at(i), false);
			if(affectBipAssigns.size()>0)
				return true;
		}
		return false;
	} else if(size1>0 && size2==0) { //case 1c
		for(int i=0; i<size1; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(st1s_p->at(i), false);
			if(affectedBipAssigns.size()>0)
				return true;
		}
		return false;
	} else { //case 1d
		for (int i = 0; i < size1; i++)
		{
			for (int j = 0; j < size2; j++)
			{	
				if(isAffectsBipStar(st1s_p->at(i),st2s_p->at(j))) return true;
			}
		}
		return false;
	}
}

BOOLEAN AffectsBip::affectsBipStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 2a
		STMT_LIST allAssign;
		_pkb->getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectBipAssigns.size()>0)
				st2s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipFirst(st2s_p->at(index), false);
			if(affectBipAssigns.size()==0) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	} else if(size1>0 && size2==0) { //case 2b
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipStarSecond(st1s_p->at(i), true);
			for (int j = 0; j < affectedBipAssigns.size(); j++) {
				st2s_p->push_back(affectedBipAssigns.at(j));
			}
		}
	} else { //case 2d
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isAffectsBipStar(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN AffectsBip::affectsBipStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 3a
		STMT_LIST allAssign;
		_pkb->getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectedBipAssigns.size()>0)
				st1s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 3b
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectBipAssigns = getAffectsBipStarFirst(st2s_p->at(i), true);
			for (int j = 0; j < affectBipAssigns.size(); j++) {
				st1s_p->push_back(affectBipAssigns.at(j));
			}
		}	
	} else if(size1>0 && size2==0) { //case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedBipAssigns = getAffectsBipSecond(st1s_p->at(index), false);
			if(affectedBipAssigns.size()==0) {
				st1s_p->erase(st1s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	} else { //case 3d
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isAffectsBipStar(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN AffectsBip::affectsBipStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		STMT_LIST allAssigns;
		_pkb->getAllAssignment(&allAssigns);
		for(int i=0; i<allAssigns.size(); i++) {
			int current = allAssigns.at(i);
			STMT_LIST affectedBipAssigns = getAffectsBipStarSecond(current, true);
			for(int j=0; j<affectedBipAssigns.size(); j++) {
				st1s_p->push_back(current);
				st2s_p->push_back(affectedBipAssigns.at(j));
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		vector<int> st2s_copy;
		for (int i = 0; i < size2; i++) {
			st2s_copy.push_back(st2s_p->at(i));
		}
		st2s_p->clear();

		for (int i = 0; i < size2; i++) {
			int currentEleSt2s = st2s_copy.at(i);
			vector<int> temp = getAffectsBipStarFirst(currentEleSt2s,true);
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
			vector<int> temp = getAffectsBipStarSecond(currentEleSt1s, true);
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
				if (isAffectsBipStar(st1s_p->at(index), st2s_p->at(index))) {
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
