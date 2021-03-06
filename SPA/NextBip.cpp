/**
 * \file	NextBip.cpp
 * \class	NextBip
 * \brief	Provide functions for query on NextBip relation and NextBipStar relation.
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
	} else { //node1 is IF, WHILE, CALL, OR stmt1 is the last stmt of node1, need to go to the next nodes
		vector<CFGNode*> nextEdges = node1->getNextEdges(); //go to next nodes

		for(int i=0; i<nextEdges.size(); i++) { //loop through each next edge
			CFGNode* currentNext = nextEdges.at(i);
			if(currentNext->getCFGType() == CFG_DUMMY) { //dummy node, special handling
				queue<CFGNode*> dummyLinks;
				dummyLinks.push(currentNext);
				while(!dummyLinks.empty()) { //there are more dummy links
					CFGNode* currentDummy = dummyLinks.front(); dummyLinks.pop(); //retrieve one dummy node and remove it from queue
					vector<CFGNode*> temp = currentDummy->getNextEdges();
					for(int i=0; i<temp.size(); i++) {
						CFGNode* tempNext = temp.at(i);
						if(tempNext->getCFGType()==CFG_DUMMY) //the next node is still a dummy node
							dummyLinks.push(tempNext);
						else if(tempNext->getStartStatement() == stmt2)
							return true;
					}
				}
			}
			else if(currentNext->getStartStatement() == stmt2)
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
		//node2 contains all assign statements and stmt2 is not the first stmt of node2, there is stmt before stmt2
		resultLst.push_back(stmt2-1);
	} else { //node2 is IF, WHILE, CALL, OR stmt2 is the first stmt of node2, need to go to the prev nodes
		vector<CFGNode*> prevEdges = node2->getPrevEdges();

		for(int i=0; i<prevEdges.size(); i++) { //loop through each previous edge
			CFGNode* currentPrev = prevEdges.at(i);

			if(currentPrev->getCFGType() == CFG_DUMMY) { //dummy node, special handling
				queue<CFGNode*> dummyLinks;
				dummyLinks.push(currentPrev);
				while(!dummyLinks.empty()) { //there are more dummy links
					CFGNode* currentDummy = dummyLinks.front(); dummyLinks.pop(); //retrieve one dummy node and remove it from queue
					vector<CFGNode*> temp = currentDummy->getPrevEdges();
					for(int i=0; i<temp.size(); i++) {
						CFGNode* tempPrev = temp.at(i);
						if(tempPrev->getCFGType()==CFG_DUMMY) //the prev node is still a dummy node
							dummyLinks.push(tempPrev);
						else
							resultLst.push_back(tempPrev->getEndStatement());
					}
				}
			}
			else
				resultLst.push_back(currentPrev->getEndStatement());	
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
	} else { //node1 is IF, WHILE, or CALL, OR stmt1 is the last stmt of node1, need to go to the next nodes
		vector<CFGNode*> nextEdges = node1->getNextEdges();
		
		for(int i=0; i<nextEdges.size(); i++) { //loop through each next edge
			CFGNode* currentNext = nextEdges.at(i);

			if(currentNext->getCFGType() == CFG_DUMMY) { //dummy node, special handling
				queue<CFGNode*> dummyLinks;
				dummyLinks.push(currentNext);
				while(!dummyLinks.empty()) { //there are more dummy links
					CFGNode* currentDummy = dummyLinks.front(); dummyLinks.pop(); //retrieve one dummy node and remove it from queue
					vector<CFGNode*> temp = currentDummy->getNextEdges();
					for(int i=0; i<temp.size(); i++) {
						CFGNode* tempNext = temp.at(i);
						if(tempNext->getCFGType()==CFG_DUMMY) //the next node is still a dummy node
							dummyLinks.push(tempNext);
						else
							resultLst.push_back(tempNext->getStartStatement());
					}
				}
			}
			else
				resultLst.push_back(currentNext->getStartStatement());	
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

BOOLEAN NextBip::isNextBipStar(STMT stmt1, STMT stmt2)
{
	vector<CFGNode*> visitedNodes;
	stack<STMT> callStack;
	BOOLEAN result = false;

	CFGNode* startNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1

	if(startNode->getCFGType() == CFG_NORMAL_BLOCK) { //node with all assigns
		if(stmt2 > stmt1 && stmt2 <= startNode->getEndStatement())
			return true;
		else
			isNextBipStarHelper(startNode, stmt2, &visitedNodes, callStack, &result);
	} else {
		visitedNodes.push_back(startNode);
		isNextBipStarHelper(startNode, stmt2, &visitedNodes, callStack, &result);
	}

	return result;
}


void NextBip::isNextBipStarHelper(CFGNode* node1, STMT stmt2, vector<CFGNode*>* visitedNodes, stack<STMT> callStack, BOOLEAN* result)
{
	if(node1->getBipType() == CFG_BIP_IN) { //call node
		callStack.push(node1->getStartStatement()); //push to call stack
		CFGNode* next_node = node1->getNextEdges().at(0); //there will be exactly one next node
		if(indexOf((*visitedNodes), next_node) >= 0) {
			//current successor has been visited, do not visit it again
		} else {
			visitedNodes->push_back(next_node); //mark current successor as visited to avoid re-visit
			if(stmt2>=next_node->getStartStatement() && stmt2<=next_node->getEndStatement()) {
				*result = true;
				return;
			}
			else
				return isNextBipStarHelper(next_node, stmt2, visitedNodes, callStack, result);
		}
	} 
	else if(node1->getBipType() == CFG_BIP_NORMAL) { //not call, not return
		vector<CFGNode*> next_nodes = node1->getNextEdges();
		if (next_nodes.size() == 0) { // no more successor, base case, stop and return
			return;
		}else {
			for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
				CFGNode* currentNode = next_nodes.at(i);
				if(indexOf((*visitedNodes), currentNode) >= 0){
					//current successor has been visited, do not visit it again
				} else {
					visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
					if(stmt2>=currentNode->getStartStatement() && stmt2<=currentNode->getEndStatement()) {
						*result = true;
						return;
					} else {
						isNextBipStarHelper(currentNode, stmt2, visitedNodes, callStack, result);
						if(*result == true)
							return;
					}
				}
			}
		}
	}
	else { //return node, which is either all-assign node or dummy node
		vector<CFGNode*> next_nodes = node1->getNextEdges();

		//all-assign node OR dummy node, no next node within the same procedure.

		// clear visited nodes that are within the same procedure
		PROC_INDEX currentProc = node1->getProcIndex();
		//STMT stmt1a = _pkb->getProcStart(currentProc); STMT stmt2a = _pkb->getProcEnd(currentProc);
		//for(int i=stmt1a; i<=stmt2a; i++) {
			//int index = indexOf((*visitedNodes), _pkb->getCFGBip()->getCFGNodeByStmtNumber(i));
			//if(index >=0)
				//visitedNodes->erase(visitedNodes->begin()+index);
		//}
		int visitedSize = visitedNodes->size();
		int indexOn = 0;
		for(int i=0; i<visitedSize; i++) {
			if(visitedNodes->at(indexOn)->getProcIndex() == currentProc)
				visitedNodes->erase(visitedNodes->begin()+indexOn);
			else
				indexOn++;
		}

		// pop call stack
		if(callStack.empty()) { // do not come from a label, need to jump via all labels
			if (next_nodes.size() == 0) { // no more successor, base case, stop and return
				return;
			}else {
				for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
					CFGNode* currentNode = next_nodes.at(i);
					if(indexOf((*visitedNodes), currentNode) >= 0){
						//current successor has been visited, do not visit it again
					} else {
						visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
						if(stmt2>=currentNode->getStartStatement() && stmt2<=currentNode->getEndStatement()) {
							*result = true;
							return;
						} else {
							isNextBipStarHelper(currentNode, stmt2, visitedNodes, callStack, result);
							if(*result == true)
								return;
						}
					}
				}
			}
		} else { //come from a specific label, need to jump via that label
			int label = callStack.top(); callStack.pop();
			//int jumpTo = label + 1;
			STMT_LIST nextStmts = _pkb->getNextSecond(label);

			if (next_nodes.size() == 0) { // no more successor, base case, stop and return
				return;
			}else {
				if(nextStmts.size() == 0) {
					for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
						CFGNode* currentNode = next_nodes.at(i);
						if(currentNode->getStartStatement() != -1 || currentNode->getProcIndex() != _pkb->getCFGBip()->getCFGNodeByStmtNumber(label)->getProcIndex())
							continue;
						if(indexOf((*visitedNodes), currentNode) >= 0){
							//current successor has been visited, do not visit it again
						} else {
							visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
							if(stmt2>=currentNode->getStartStatement() && stmt2<=currentNode->getEndStatement()) {
								*result = true;
								return;
							} else {
								isNextBipStarHelper(currentNode, stmt2, visitedNodes, callStack, result);
								if(*result == true)
									return;
							}
						}
					}
				} else {
					assert(nextStmts.size() == 1);
					int jumpTo = nextStmts.at(0);

					for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
						CFGNode* currentNode = next_nodes.at(i);
						if(currentNode->getStartStatement() != jumpTo)
							continue;
						if(indexOf((*visitedNodes), currentNode) >= 0){
							//current successor has been visited, do not visit it again
						} else {
							visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
							if(stmt2>=currentNode->getStartStatement() && stmt2<=currentNode->getEndStatement()) {
								*result = true;
								return;
							} else {
								isNextBipStarHelper(currentNode, stmt2, visitedNodes, callStack, result);
								if(*result == true)
									return;
							}
						}
					}
				}	
			}
		}
	}
}



STMT_LIST NextBip::getNextBipStarFirst(STMT stmt2)
{
	vector<CFGNode*> visitedNodes;
	stack<STMT> callStack;
	STMT_LIST result;

	CFGNode* endNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt2); //get the CFGNode that contains stmt2

	if(endNode->getCFGType() == CFG_NORMAL_BLOCK) { //node with all assigns
		for(int i=stmt2-1; i>=endNode->getStartStatement(); i--) {
			result.push_back(i);
		}
		getNextBipStarFirstHelper(endNode, visitedNodes, callStack, &result);
	} else { //if, while, call
		visitedNodes.push_back(endNode);
		getNextBipStarFirstHelper(endNode, visitedNodes, callStack, &result);
	}

	return result;
}

void NextBip::getNextBipStarFirstHelper(CFGNode* node2,vector<CFGNode*> visitedNodes, stack<STMT> callStack, STMT_LIST* result)
{
	vector<CFGNode*> prev_nodes = node2->getPrevEdges();
	if(prev_nodes.size() == 0)
		return;

	BOOLEAN jumpToCaller = false;
	BOOLEAN jumpToReturnNode = false;
	for(int i=0; i<prev_nodes.size(); i++) {
		if(prev_nodes.at(i)->getBipType() == CFG_BIP_IN) { //jump back to caller
			jumpToCaller = true;
			//break;
		}
		if(prev_nodes.at(i)->getBipType() == CFG_BIP_OUT) { //jump to return node
			jumpToReturnNode = true;
			//break;
		}
	}

	if(!jumpToCaller && !jumpToReturnNode) { //jump within the same proc
		for (int i = 0; i < prev_nodes.size(); i++) { // loop through each prev CFGNode
			CFGNode* currentNode = prev_nodes.at(i);
			if(indexOf((visitedNodes), currentNode) >= 0){
				//current predecessor has been visited, do not visit it again
			} else {
				visitedNodes.push_back(currentNode); //mark current predecessor as visited to avoid re-visit
				for(int i=currentNode->getEndStatement(); i>=currentNode->getStartStatement(); i--) {
					result->push_back(i);
				}
				getNextBipStarFirstHelper(currentNode, visitedNodes, callStack, result);
			}
		}
	} else {
		//jump back to caller or back to a return node, need to traverse node within the same proc first
		CFGNode* nodeInSameProc = NULL;
		for (int i = 0; i < prev_nodes.size(); i++) { // loop through each prev CFGNode to find node within same procedure
			CFGNode* currentNode = prev_nodes.at(i);
			if(node2->getProcIndex() == currentNode->getProcIndex()) {
				nodeInSameProc = currentNode;
				prev_nodes.erase(prev_nodes.begin()+i);
				break;
			}
		}
		// deal with the node within same procedure first, if such node exists
		if(nodeInSameProc != NULL) {
			if(indexOf((visitedNodes), nodeInSameProc) >= 0){
				//current successor has been visited, do not visit it again
			} else {
				visitedNodes.push_back(nodeInSameProc); //mark current successor as visited to avoid re-visit
				for(int i=nodeInSameProc->getEndStatement(); i>=nodeInSameProc->getStartStatement(); i--) {
					result->push_back(i);
				}
				getNextBipStarFirstHelper(nodeInSameProc, visitedNodes, callStack, result);
			}
		}

		//jump back to caller or back to return node, no prev nodes within same proc
		for(int i=0; i<prev_nodes.size(); i++) {
			CFGNode* currentNode = prev_nodes.at(i);
			if(currentNode->getBipType() == CFG_BIP_OUT) { //jump to return nodes
				STMT_LIST prevStmts = _pkb->getNextFirst(node2->getStartStatement());
				if(prevStmts.size() == 0) { //a dummy node
					vector<CFGNode*> prevStmtsNodes = _pkb->getCFGBip()->getCFGNodeByStmtNumber(_pkb->getProcStart(currentNode->getProcIndex()))->getPrevEdges();
					for(int j=0; j<prevStmtsNodes.size(); j++) {
						CFGNode* temp = prevStmtsNodes.at(j);
						if(temp->getCFGType() != CFG_CALL_STATEMENT)
							continue;

						stack<STMT> callStackCopy = callStack;
						callStackCopy.push(temp->getEndStatement()); //push to call stack
						if(indexOf((visitedNodes), currentNode) >= 0) {
							//current predecessor has been visited, do not visit it again
						} else {
							visitedNodes.push_back(currentNode); //mark current predecessor as visited to avoid re-visit
							for(int i=currentNode->getEndStatement(); i>=currentNode->getStartStatement(); i--) {
								if(i != -1)
									result->push_back(i);
							}
							getNextBipStarFirstHelper(currentNode, visitedNodes, callStackCopy, result);
						}
					}
				}


				for(int j=0; j<prevStmts.size();j++) {
					if(_pkb->getCFGBip()->getCFGNodeByStmtNumber(prevStmts.at(j))->getCFGType() != CFG_CALL_STATEMENT) {
						//not a call statement, do not consider
					} else if(_pkb->getCallee(prevStmts.at(j)) != currentNode->getProcIndex()) {
						//a call statement, but the node to jump is not inside the called procedure, still do not consider 
					} else { //the node to jump is inside the called procedure, ready to jump
						stack<STMT> callStackCopy = callStack;
						callStackCopy.push(prevStmts.at(j)); //push to call stack
						if(indexOf((visitedNodes), currentNode) >= 0) {
							//current predecessor has been visited, do not visit it again
						} else {
							visitedNodes.push_back(currentNode); //mark current predecessor as visited to avoid re-visit
							for(int i=currentNode->getEndStatement(); i>=currentNode->getStartStatement(); i--) {
								if(i != -1)
									result->push_back(i);
							}
							getNextBipStarFirstHelper(currentNode, visitedNodes, callStackCopy, result);
						}
					}
				}			
			} else { //jump back to caller
				assert(currentNode->getBipType() == CFG_BIP_IN);
				// clear visited nodes that are within the same procedure before jump back
				vector<CFGNode*> visitedNodesCopy = visitedNodes;
				PROC_INDEX currentProc = node2->getProcIndex();
				//STMT stmt1a = _pkb->getProcStart(currentProc); STMT stmt2a = _pkb->getProcEnd(currentProc);
				//for(int i=stmt1a; i<=stmt2a; i++) {
					//int index = indexOf((visitedNodesCopy), _pkb->getCFGBip()->getCFGNodeByStmtNumber(i));
					//if(index >=0)
						//visitedNodesCopy.erase(visitedNodesCopy.begin()+index);
				//}
				int visitedSize = visitedNodesCopy.size();
				int indexOn = 0;
				for(int i=0; i<visitedSize; i++) {
					if(visitedNodesCopy.at(indexOn)->getProcIndex() == currentProc)
						visitedNodesCopy.erase(visitedNodesCopy.begin()+indexOn);
					else
						indexOn++;
				}

				// pop call stack
				if(callStack.empty()) { // do not come from a label, need to jump back via all labels
					if(indexOf((visitedNodesCopy), currentNode) >= 0){
						//current successor has been visited, do not visit it again
					} else {
						visitedNodesCopy.push_back(currentNode); //mark current successor as visited to avoid re-visit
						for(int i=currentNode->getStartStatement(); i<=currentNode->getEndStatement(); i++) {
							result->push_back(i);
						}
						getNextBipStarFirstHelper(currentNode, visitedNodesCopy, callStack, result);
					}	
				} else { //come from a specific label, need to jump via that label
					stack<STMT> callStackCopy = callStack;
					int label = callStackCopy.top(); callStackCopy.pop();
					int jumpTo = label;
					
					if(currentNode->getEndStatement() != jumpTo)
						continue;

					if(indexOf((visitedNodesCopy), currentNode) >= 0){
						//current successor has been visited, do not visit it again
					} else {
						visitedNodesCopy.push_back(currentNode); //mark current successor as visited to avoid re-visit
						for(int i=currentNode->getStartStatement(); i<=currentNode->getEndStatement(); i++) {
							result->push_back(i);
						}
						getNextBipStarFirstHelper(currentNode, visitedNodesCopy, callStackCopy, result);
					}				
				}
			}
		}
	}
}

STMT_LIST NextBip::getNextBipStarSecond(STMT stmt1)
{
	vector<CFGNode*> visitedNodes;
	stack<STMT> callStack;
	STMT_LIST result;

	CFGNode* startNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1

	if(startNode->getCFGType() == CFG_NORMAL_BLOCK) { //node with all assigns
		for(int i=stmt1+1; i<=startNode->getEndStatement(); i++) {
			result.push_back(i);
		}
		getNextBipStarSecondHelper(startNode, &visitedNodes, callStack, &result);
	} else {
		visitedNodes.push_back(startNode);
		getNextBipStarSecondHelper(startNode, &visitedNodes, callStack, &result);
	}

	return result;
}



void NextBip::getNextBipStarSecondHelper(CFGNode* node1,vector<CFGNode*>* visitedNodes, stack<STMT> callStack, STMT_LIST* result)
{
	if(node1->getBipType() == CFG_BIP_IN) { //call node
		callStack.push(node1->getStartStatement()); //push to call stack
		CFGNode* next_node = node1->getNextEdges().at(0); //there will be exactly one next node
		if(indexOf((*visitedNodes), next_node) >= 0) {
			//current successor has been visited, do not visit it again
		} else {
			visitedNodes->push_back(next_node); //mark current successor as visited to avoid re-visit
			for(int i=next_node->getStartStatement(); i<=next_node->getEndStatement(); i++) {
				result->push_back(i);
			}
			getNextBipStarSecondHelper(next_node, visitedNodes, callStack, result);
		}
	} 
	else if(node1->getBipType() == CFG_BIP_NORMAL) { //not call, not return
		vector<CFGNode*> next_nodes = node1->getNextEdges();
		if (next_nodes.size() == 0) { // no more successor, base case, stop and return
			return;
		}else {
			for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
				CFGNode* currentNode = next_nodes.at(i);
				if(indexOf((*visitedNodes), currentNode) >= 0){
					//current successor has been visited, do not visit it again
				} else {
					visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
					for(int i=currentNode->getStartStatement(); i<=currentNode->getEndStatement(); i++) {
						if(i != -1) //avoid adding dummy nodes
							result->push_back(i);
					}
					getNextBipStarSecondHelper(currentNode, visitedNodes, callStack, result);
				}
			}
		}
	}
	else { //return node, which is either all-assign node or dummy node
		vector<CFGNode*> next_nodes = node1->getNextEdges();

		//all-assign node OR dummy node, no next node within the same procedure.

		// clear visited nodes that are within the same procedure
		PROC_INDEX currentProc = node1->getProcIndex();
		//STMT stmt1a = _pkb->getProcStart(currentProc); STMT stmt2a = _pkb->getProcEnd(currentProc);
		//for(int i=stmt1a; i<=stmt2a; i++) {
			//int index = indexOf((*visitedNodes), _pkb->getCFGBip()->getCFGNodeByStmtNumber(i));
			//if(index >=0)
				//visitedNodes->erase(visitedNodes->begin()+index);
		//}
		int visitedSize = visitedNodes->size();
		int indexOn = 0;
		for(int i=0; i<visitedSize; i++) {
			if(visitedNodes->at(indexOn)->getProcIndex() == currentProc)
				visitedNodes->erase(visitedNodes->begin()+indexOn);
			else
				indexOn++;
		}

		// pop call stack
		if(callStack.empty()) { // do not come from a label, need to jump via all labels
			if (next_nodes.size() == 0) { // no more successor, base case, stop and return
				return;
			}else {
				for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
					CFGNode* currentNode = next_nodes.at(i);
					if(indexOf((*visitedNodes), currentNode) >= 0){
						//current successor has been visited, do not visit it again
					} else {
						visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
						for(int i=currentNode->getStartStatement(); i<=currentNode->getEndStatement(); i++) {
							if(i != -1)
								result->push_back(i);
						}
						getNextBipStarSecondHelper(currentNode, visitedNodes, callStack, result);
					}
				}
			}
		} else { //come from a specific label, need to jump via that label
			int label = callStack.top(); callStack.pop();
			//int jumpTo = label + 1;
			STMT_LIST nextStmts = _pkb->getNextSecond(label);

			if (next_nodes.size() == 0) { // no more successor, base case, stop and return
				return;
			}else {
				if(nextStmts.size() == 0) {
					for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
						CFGNode* currentNode = next_nodes.at(i);
						if(currentNode->getStartStatement() != -1 || currentNode->getProcIndex() != _pkb->getCFGBip()->getCFGNodeByStmtNumber(label)->getProcIndex())
							continue;
						if(indexOf((*visitedNodes), currentNode) >= 0){
							//current successor has been visited, do not visit it again
						} else {
							visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
							for(int i=currentNode->getStartStatement(); i<=currentNode->getEndStatement(); i++) {
								if(i != -1)
									result->push_back(i);
							}
							getNextBipStarSecondHelper(currentNode, visitedNodes, callStack, result);
						}
					}
				} else {
					assert(nextStmts.size() == 1);
					int jumpTo = nextStmts.at(0);

					for (int i = 0; i < next_nodes.size(); i++) { // loop through each next CFGNode
						CFGNode* currentNode = next_nodes.at(i);
						if(currentNode->getStartStatement() != jumpTo)
							continue;
						if(indexOf((*visitedNodes), currentNode) >= 0){
							//current successor has been visited, do not visit it again
						} else {
							visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit
							for(int i=currentNode->getStartStatement(); i<=currentNode->getEndStatement(); i++) {
								if(i != -1)
									result->push_back(i);
							}
							getNextBipStarSecondHelper(currentNode, visitedNodes, callStack, result);
						}
					}
				}	
			}		
		}
	}
}



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



BOOLEAN NextBip::nextBipStar_00(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
				if (isNextBipStar(st1s_p->at(i), st2s_p->at(j))) {
					return(true);
				}
			}
		}
		return(false);
	}
}

BOOLEAN NextBip::nextBipStar_01(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
			STMT_LIST val = getNextBipStarSecond(st1s_p->at(i));
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
				if (isNextBipStar(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN NextBip::nextBipStar_10(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
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
			STMT_LIST val = getNextBipStarFirst(st2s_p->at(i));
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
				if (isNextBipStar(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN NextBip::nextBipStar_11(STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		STMT_LIST allStmts;
		_pkb->getAllStmt(&allStmts);
		for(int i=0; i<allStmts.size(); i++) {
			STMT current = allStmts.at(i);
			STMT_LIST nexts = getNextBipStarSecond(current);
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
			vector<int> temp = getNextBipStarFirst(currentEleSt2s);
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
			vector<int> temp = getNextBipStarSecond(currentEleSt1s);
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
				if (isNextBipStar(st1s_p->at(index), st2s_p->at(index))) {
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
