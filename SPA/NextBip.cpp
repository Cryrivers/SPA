/**
 * \file	Next.cpp
 * \class	Next
 * \brief	Store all 'Next' relations in the form (stmt1, stmt2) where both 'stmt1' and 'stmt2' are statement numbers. Provide functions for query on Next relation and NextStar relation. The storage is referred as NextTable.
 */

#include "stdafx.h"
#include "NextBip.h"
#include "Utility.h"
#include "PKBController.h"


NextBip::NextBip(void)
{
	_pkb = PKBController::createInstance();
}

BOOLEAN NextBip::isNextBip(STMT stmt1, STMT stmt2)
{
	CFGNode* node1 = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1
	if(node1->getCFGType() == CFG_NORMAL_BLOCK && node1->getEndStatement() != stmt1) {
		//node1 contains all assign statements and stmt1 is not the last stmt of node1, there is stmt after stmt1
		if(stmt2 == stmt1+1)
			return true;
		else	
			return false;
	} else { //node1 is IF, WHILE, or CALL, OR stmt1 is the last stmt of node1 //
		vector<CFGNode*> nextEdges = node1->getNextEdges();
		for(int i=0; i<nextEdges.size(); i++) { //loop through each next edge
			if(nextEdges.at(i)->getStartStatement() == stmt2)
				return true;	
		}
		return false;	
	}
}

STMT_LIST NextBip::getNextBipFirst(STMT stmt2)
{
	STMT_LIST resultLst;

	CFGNode* node2 = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt2); //get the CFGNode that contains stmt2
	if(node2->getCFGType() == CFG_NORMAL_BLOCK && node2->getStartStatement() != stmt2) {
		//node1 contains all assign statements and stmt1 is not the last stmt of node1, there is stmt after stmt1
		resultLst.push_back(stmt2-1);
	} else { //node1 is IF, WHILE, or CALL, OR stmt1 is the last stmt of node1 //
		vector<CFGNode*> prevEdges = node2->getPrevEdges();
		for(int i=0; i<prevEdges.size(); i++) { //loop through each previous edge
			resultLst.push_back(prevEdges.at(i)->getEndStatement());	
		}
	}

	return resultLst;
}

STMT_LIST NextBip::getNextBipSecond(STMT stmt1)
{
	STMT_LIST resultLst;

	CFGNode* node1 = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1
	if(node1->getCFGType() == CFG_NORMAL_BLOCK && node1->getEndStatement() != stmt1) {
		//node1 contains all assign statements and stmt1 is not the last stmt of node1, there is stmt after stmt1
		resultLst.push_back(stmt1+1);
	} else { //node1 is IF, WHILE, or CALL, OR stmt1 is the last stmt of node1 //
		vector<CFGNode*> nextEdges = node1->getNextEdges();
		for(int i=0; i<nextEdges.size(); i++) { //loop through each next edge
			resultLst.push_back(nextEdges.at(i)->getStartStatement());	
		}
	}

	return resultLst;
}

/**
 * \fn	BOOLEAN NextBip::nextBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
 *
 * \brief	A function for query on 'NextBip' relation.
 * 
 * To handle a query in the form NextBip(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if there exists a pair <a,b> such that NextBip(a,b) is true, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one pair (a, b) such that NextBip(a,b) holds where b equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one pair (a, b) such that NextBip(a,b) holds where a equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that NextBip(ai, bj) is true, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct bi values where NextBip(_, bi) holds.
 *	2b) List A non-empty, List B empty
 *	Fill up list B with all the values b if NextBip(a, b) is true where a is any element in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements bi from list B if NextBip(_,bi) does not hold.
 *	2d) Both lists are non-empty
 *	For each element bi from list B, if no element aj from list A can satisfy NextBip(aj, bi), then remove this bi. Otherwise, keep it.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct ai values where NextBip(ai, _) holds.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values a if NextBip(a, b) is true where b is any element in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements ai from list A if NextBip(ai,_) does not hold.
 *	3d) Both lists are non-empty
 *	For each element ai from list A, if no element bj from list B can satisfy NextBip(ai, bj), then remove this ai. Otherwise, keep it.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up both lists with all pairs of (ai, bi) that satisfy NextBip(ai, bi).
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values ai such that NextBip(ai, bi).
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values bi such that NextBip(ai, bi).
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if NextBip(ai, bi) is false.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	st1s_ptr	 A pointer to a list of statement numbers. These statements are the first parameter of relation NextBip.
 * \param	st2s_ptr	 A pointer to a list of statement numbers. These statements are the second parameter of relation NextBip.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
BOOLEAN NextBip::nextBip(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(nextBip_00(st1s_ptr, st2s_ptr));

	case 1:
		return(nextBip_01(st1s_ptr, st2s_ptr));

	case 2:
		return(nextBip_10(st1s_ptr, st2s_ptr));

	case 3:
		return(nextBip_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}

BOOLEAN NextBip::nextBip_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 1a
		CFGNode* firstNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(1);
		if(firstNode == NULL)
			return false;
		if(firstNode->getCFGType() == CFG_NORMAL_BLOCK) {
			if(firstNode->getStartStatement() != firstNode->getEndStatement())
				return true;
			else {
				if(firstNode->getNextEdges().size() != 0)
					return true;
				else
					return false;
			}
		} else {
			return true;
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < size2; i++) {
			if (getNextBipFirst(st2s_p->at(i)).size() != 0) {
				return(true);
			}
		}
		return(false);
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < size1; i++) {
			if (getNextBipSecond(st1s_p->at(i)).size() != 0) {
				return(true);
			}
		}
		return(false);
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < size1; i++) {
			for (int j = 0; j < size2; j++) {
				if (isNextBip(st1s_p->at(i), st2s_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}

BOOLEAN NextBip::nextBip_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		STMT_LIST allStmts;
		_pkb->getAllStmt(&allStmts);
		for (int i = 0; i < allStmts.size(); i++) {
			STMT current = allStmts.at(i);
			if (getNextBipFirst(current).size() != 0) {
				st2s_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			if (getNextBipFirst(st2s_p->at(index)).size() == 0) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b
		for (int i = 0; i < size1; i++) {
			STMT_LIST val = getNextBipSecond(st1s_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*st2s_p, current) == -1) {
					st2s_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isNextBip(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN NextBip::nextBip_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		STMT_LIST allStmts;
		_pkb->getAllStmt(&allStmts);
		for (int i = 0; i < allStmts.size(); i++) {
			STMT current = allStmts.at(i);
			if (getNextBipSecond(current).size() != 0) {
				st1s_p->push_back(current);
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b
		for (int i = 0; i < size2; i++) {
			STMT_LIST val = getNextBipFirst(st2s_p->at(i));
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
			if (getNextBipSecond(st1s_p->at(index)).size() == 0) {
				st1s_p->erase(st1s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isNextBip(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN NextBip::nextBip_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		STMT_LIST allStmts;
		_pkb->getAllStmt(&allStmts);
		for(int i=0; i<allStmts.size(); i++) {
			STMT current = allStmts.at(i);
			STMT_LIST nexts = getNextBipSecond(current);
			for(int j=0; j<nexts.size(); j++) {
				st1s_p->push_back(current);
				st2s_p->push_back(nexts.at(j));
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
			vector<int> temp = getNextBipFirst(currentEleSt2s);
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
			vector<int> temp = getNextBipSecond(currentEleSt1s);
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
				if (isNextBip(st1s_p->at(index), st2s_p->at(index))) {
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
BOOLEAN NextBip::isNextStar(STMT stmt1, STMT stmt2)
{
	if(isNext(stmt1, stmt2)) //direct Next relation found, return true
		return true;
	else {
		STMT_LIST links; // the link between predecessor and successor
		links = getNextSecond(stmt1);
		STMT_LIST* visitedNodes = new STMT_LIST();
		BOOLEAN result = isNextStarHelper(links, stmt2, visitedNodes);
		delete visitedNodes;
		return result;
	}
}
*/

/*
BOOLEAN Next::isNextStarHelper(STMT_LIST links, STMT stmt2, STMT_LIST* visitedNodes)
{
	int linkSize = links.size();
	if(linkSize==0) // no more links between predecessor and successor, return false;
		return false;

	STMT_LIST links2;
	for(int i=0; i<linkSize; i++) {
		STMT current = links.at(i);
		if(indexOf((*visitedNodes), current)>=0) {
			// current node has been visited, do not visit again
		} else {
			visitedNodes->push_back(current); //mark current node as visited to avoid re-visit
			if(isNext(current, stmt2)) // one Next* path found, return true
				return true;
			STMT_LIST temp = getNextSecond(current);
			for(int j=0; j<temp.size(); j++) {
				links2.push_back(temp.at(j));
			}
		}
	}
	return isNextStarHelper(links2, stmt2, visitedNodes);
}
*/

/*
STMT_LIST NextBip::getNextStarFirst(STMT stmt2)
{
	STMT_LIST resultLst; //the result list as well as visited stmt list
	getNextStarFirstHelper(stmt2, &resultLst);
	
	return resultLst;
}
*/

/*
void Next::getNextStarFirstHelper(STMT stmt2, STMT_LIST* resultLst)
{
	STMT_LIST directFirstLst = getNextFirst(stmt2);

	if (directFirstLst.size() == 0) { // no more predecessor, base case, stop and return
		return;
	}else {
		int directFirstLstSize = directFirstLst.size();
		for (int i = 0; i < directFirstLstSize; i++) { // loop through each direct predecessor
			int currentFirst = directFirstLst.at(i);
			if(indexOf((*resultLst), currentFirst) >= 0){
				//current predecessor is in the result list, which means it has been visited, do not visit it again
			} else {
				resultLst->push_back(currentFirst); //add this predecessor into result list
				getNextStarFirstHelper(currentFirst, resultLst); // let this predecessor find all its direct/indirect predecessors
			}
		}
	}
}
*/

/*
STMT_LIST NextBip::getNextStarSecond(STMT stmt1)
{
	STMT_LIST resultLst;
	getNextStarSecondHelper(stmt1, &resultLst);
	
	return resultLst;
}
*/

/*
void Next::getNextStarSecondHelper(STMT stmt1, STMT_LIST* resultLst)
{
	STMT_LIST directSecondLst = getNextSecond(stmt1);

	if (directSecondLst.size() == 0) { // no more successor, base case, stop and return
		return;
	}else {
		int directSecondLstSize = directSecondLst.size();
		for (int i = 0; i < directSecondLstSize; i++) { // loop through each direct successor
			int currentSecond = directSecondLst.at(i);
			if(indexOf((*resultLst), currentSecond) >= 0){
				//current successor is in the result list, which means it has been visited, do not visit it again
			} else {
				resultLst->push_back(currentSecond); //add this successor into result list
				getNextStarSecondHelper(currentSecond, resultLst); // let this successor find all its direct/indirect successor
			}
		}
	}
}
*/



/**
 * \fn	BOOLEAN NextBip::nextBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
 *
 * \brief	A function for query on 'NextBipStar' relation.
 * 
 * To handle a query in the form NextBipStar(List A, List B, arg).

 * Case 1: arg = 00 = 0
 *	1a) Both lists are empty
 *	Return TRUE if there exists a pair <a,b> such that NextBip*(a,b) is true, FASLE otherwise.
 *	1b) List A empty, List B non-empty
 *	Return TRUE if there exists one pair (a, b) such that NextBip*(a,b) holds where b equals to at least one of the element in List B, FASLE otherwise.
 *	1c) List A non-empty, List B empty
 *	Return TRUE if there exists one pair (a, b) such that NextBip*(a,b) holds where a equals to at least one of the element in List A, FASLE otherwise.
 *	1d) Both lists are non-empty
 *	Let ai be the i-th element of List A and bj be the j-th element of List B. Return TRUE if there exist at least one <ai, bj> pair such that NextBip*(ai, bj) is true, FASLE otherwise.

 * Case 2: arg = 01 = 1
 *  2a) Both lists are empty
 *	Fill up list B with all the distinct bi values where NextBip*(_, bi) holds.
 *	2b) List A non-empty, List B empty
 *	Fill up list B with all the values b if NextBip*(a, b) is true where a is any element in list A.
 *	2c) List A empty, List B non-empty
 *	Remove elements bi from list B if NextBip*(_,bi) does not hold.
 *	2d) Both lists are non-empty
 *	For each element bi from list B, if no element aj from list A can satisfy NextBip*(aj, bi), then remove this bi. Otherwise, keep it.
 *	For case 2a, 2b, 2c and 2d: Return TRUE if at the end, list B is not empty, FASLE otherwise.

 * Case 3: arg = 10 = 2
 *  3a) Both lists are empty
 *	Fill up list A with all the distinct ai values where NextBip*(ai, _) holds.
 *	3b) List A empty, List B non-empty
 *	Fill up list A with all the values a if NextBip*(a, b) is true where b is any element in list B.
 *	3c) List A non-empty, List B empty
 *	Remove elements ai from list A if NextBip*(ai,_) does not hold.
 *	3d) Both lists are non-empty
 *	For each element ai from list A, if no element bj from list B can satisfy NextBip*(ai, bj), then remove this ai. Otherwise, keep it.
 *	For case 3a, 3b, 3c and 3d: Return TRUE if at the end, list A is not empty, FASLE otherwise.

 * Case 4: arg = 11 = 3
 *  4a) Both lists are empty
 *	Fill up both lists with all pairs of (ai, bi) that satisfy NextBip*(ai, bi).
 *	4b) List A empty, List B non-empty
 *	For each element (bi) in list B, find values ai such that NextBip*(ai, bi).
	(i) If found no values, remove bi from list B.
	(ii) If found 1 single value, add this value to list A.
	(iii) if found n (n>1) values, add these values to list A and duplicate bi n-1 times in list B.
 *	4c) List A non-empty, List B empty
 *	For each element (ai) in list A, find values bi such that NextBip*(ai, bi).
	(i) If found no values, remove ai from list A.
	(ii) If found 1 single value, add this value to list B.
	(iii) if found n (n>1) values, add these values to list B and duplicate ai n-1 times in list A.
 *	4d) Both lists are non-empty
 *	Sizes of the two lists must be the same, otherwise exception. Say the sizes are both n. 
 *	Let ai be the i-th element of list A and b-i be the i-th element of list B. For i from 1 to n, remove ai, bi from list A, list B if NextBip*(ai, bi) is false.
 *	For case 4a, 4b, 4c and 4d: At the end, size of list A = size of list B. Return TRUE if at the end, list A is not empty, FASLE otherwise.
 
 * Case: arg = other values
 *	Exception

 * \param	st1s_ptr	 A pointer to a list of statement numbers. These statements are the first parameter of relation NextBipStar.
 * \param	st2s_ptr	 A pointer to a list of statement numbers. These statements are the second parameter of relation NextBipStar.
 *
 * \return	A Boolean value as specified in the detailed description.
 */
/*
BOOLEAN NextBip::nextBipStar(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg) {
	case 0:
		return(nextBipStar_00(st1s_ptr, st2s_ptr));

	case 1:
		return(nextBipStar_01(st1s_ptr, st2s_ptr));

	case 2:
		return(nextBipStar_10(st1s_ptr, st2s_ptr));

	case 3:
		return(nextBipStar_11(st1s_ptr, st2s_ptr));

	default:
		throw "arg value invalid";
	}
}
*/

/*
BOOLEAN NextBip::nextStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
				if (isNextStar(st1s_p->at(i), st2s_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}

BOOLEAN NextBip::nextStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
			vector<int> val = getNextStarSecond(st1s_p->at(i));
			for (int j = 0; j < val.size(); j++) {
				int current = val.at(j);
				if (indexOf(*st2s_p, current) == -1) {
					st2s_p->push_back(current);
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 2d
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isNextStar(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN NextBip::nextStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
			vector<int> val = getNextStarFirst(st2s_p->at(i));
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
		int index = 0;
		for (int i = 0; i < size1; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size2; j++) {
				if (isNextStar(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN NextBip::nextStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
			int currentEleCallees = st2s_copy.at(i);
			vector<int> temp = getNextStarFirst(currentEleCallees);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					st1s_p->push_back(temp.at(j));
					st2s_p->push_back(currentEleCallees);
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
			int currentEleCallers = st1s_copy.at(i);
			vector<int> temp = getNextStarSecond(currentEleCallers);
			int tempSize = temp.size();
			if (tempSize == 0) {
				//do not fill in vector
			}else {
				for (int j = 0; j < tempSize; j++) {
					st1s_p->push_back(currentEleCallers);
					st2s_p->push_back(temp.at(j));
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		if (size1 == size2) { //case 4d
			int index = 0;
			for (int i = 0; i < size1; i++) {
				if (isNextStar(st1s_p->at(index), st2s_p->at(index))) {
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
*/