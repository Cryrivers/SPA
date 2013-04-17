/**
 * \file	AffectsBip.cpp
 * \class	AffectsBip
 * \brief	Provide functions for query on AffectsBip relation and AffectsBipStar relation.
 */

#include "stdafx.h"
#include "AffectsBip.h"
#include "Utility.h"
//#include "PKBController.h"


AffectsBip::AffectsBip(void)
{
	_pkb = PKBController::createInstance();
}

BOOLEAN AffectsBip::isAffectsBip(STMT stmt1, STMT stmt2)
{
	vector<CFGNode*> visitedNodes;
	stack<STMT> callStack;
	int result = -1; //the result of the query, 1 means true, 0 means false, -1 means not decided yet

	STMT_LIST stmt1s; stmt1s.push_back(stmt1);
	VAR_INDEX_LIST modifiedVar;
	_pkb->modifies(&stmt1s, &modifiedVar, 1); //get the variable modified by stmt1

	CFGNode* startNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1, the node must be a CFG_NORMAL_BLOCK
	STMT end = startNode->getEndStatement(); //the end statement of this CFGNode

	//one and only one variable can be modified by an assign statement, so size of modifiedVar must be 1
	//only need to check AffectsBip for this single variable 'var' inside modifiedVar

	for(int k=stmt1+1; k<=end; k++) { //statements 'stmt+1' to 'end' are assign statements
		STMT_LIST temp1; temp1.push_back(k);

		if(k == stmt2) {
			if(_pkb->uses(&temp1, &modifiedVar, 0))
				return true;
			else
				return false;
		} else { //k is not stmt2
			if(_pkb->modifies(&temp1, &modifiedVar, 0))
				return false;
		}
	}

	//should proceed to next CFGNode
	isAffectsBipHelper(startNode, stmt2, &visitedNodes, modifiedVar, &result, callStack);

	if(result == -1) //result is still not decided, so false
		return false;
	else
		return result;
}

void AffectsBip::isAffectsBipHelper(CFGNode* node1, STMT stmt2, vector<CFGNode*>* visitedNodes,
	VAR_INDEX_LIST modifiedVar, int* result, stack<STMT> callStack)
{
	if(node1->getBipType() == CFG_BIP_IN) { //call node
		callStack.push(node1->getStartStatement()); //push to call stack
		CFGNode* next_node = node1->getNextEdges().at(0); //there will be exactly one next node
		if(indexOf((*visitedNodes), next_node) >= 0) {
			//current successor has been visited, do not visit it again
		} else {
			visitedNodes->push_back(next_node); //mark current successor as visited to avoid re-visit
			int start1 = next_node->getStartStatement();
			int end1 = next_node->getEndStatement();
			for(int k=start1; k<=end1; k++) {
				STMT_LIST temp1; temp1.push_back(k);

				if(k == stmt2) {
					if(_pkb->uses(&temp1, &modifiedVar, 0)) {
						*result = 1; //true
						return;
					}
					else {
						*result = 0;
						return;
					}
				} else { //k is not stmt2
					if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //k modifies the variable, this path is broken
						return;
					}
				}
			}
			isAffectsBipHelper(next_node, stmt2, visitedNodes, modifiedVar, result, callStack);
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

					int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
					for(int k=start1; k<=end1; k++) {
						STMT_LIST temp1; temp1.push_back(k);

						if(k == stmt2) {
							if(_pkb->uses(&temp1, &modifiedVar, 0)) {
								*result = 1; //true
								return;
							}
							else {
								*result = 0;
								return;
							}
						} else { //k is not stmt2
							if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //k modifies the variable, this path is broken
								return;
							}
						}
					}

					isAffectsBipHelper(currentNode, stmt2, visitedNodes, modifiedVar, result, callStack);
					if(*result != -1) //result already decided
						return;
				}
			}
		}
	}
	else { //return node, which is either all-assign node or dummy node
		vector<CFGNode*> next_nodes = node1->getNextEdges();

		//all-assign node OR dummy node, no next node within the same procedure.

		// clear visited nodes that are within the same procedure
		PROC_INDEX currentProc = node1->getProcIndex();
		STMT stmt1 = _pkb->getProcStart(currentProc); STMT stmt2 = _pkb->getProcEnd(currentProc);
		for(int i=stmt1; i<=stmt2; i++) {
			int index = indexOf((*visitedNodes), _pkb->getCFGBip()->getCFGNodeByStmtNumber(i));
			if(index >=0)
				visitedNodes->erase(visitedNodes->begin()+index);
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

						int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
						for(int k=start1; k<=end1; k++) {
							STMT_LIST temp1; temp1.push_back(k);

							if(k == stmt2) {
								if(_pkb->uses(&temp1, &modifiedVar, 0)) {
									*result = 1; //true
									return;
								}
								else {
									*result = 0; //false
									return;
								}
							} else { //k is not stmt2
								if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //k modifies the variable, this path is broken
									return;
								}
							}
						}

						isAffectsBipHelper(currentNode, stmt2, visitedNodes, modifiedVar, result, callStack);
						if(*result != -1)
							return;
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
							int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
							for(int k=start1; k<=end1; k++) {
								STMT_LIST temp1; temp1.push_back(k);

								if(k == stmt2) {
									if(_pkb->uses(&temp1, &modifiedVar, 0)) {
										*result = 1; //true
										return;
									}
									else {
										*result = 0; //false
										return;
									}
								} else { //k is not stmt2
									if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //k modifies the variable, this path is broken
										return;
									}
								}
							}
						
							isAffectsBipHelper(currentNode, stmt2, visitedNodes, modifiedVar, result, callStack);
							if(*result != -1)
								return;
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
							int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
							for(int k=start1; k<=end1; k++) {
								STMT_LIST temp1; temp1.push_back(k);

								if(k == stmt2) {
									if(_pkb->uses(&temp1, &modifiedVar, 0)) {
										*result = 1; //true
										return;
									}
									else {
										*result = 0; //false
										return;
									}
								} else { //k is not stmt2
									if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //k modifies the variable, this path is broken
										return;
									}
								}
							}
						
							isAffectsBipHelper(currentNode, stmt2, visitedNodes, modifiedVar, result, callStack);
							if(*result != -1)
								return;
						}
					}
				}	
			}
		}
	}
}


STMT_LIST AffectsBip::getAffectsBipFirst(STMT stmt2, BOOLEAN exhaustive)
{
	//if exhaustive is true, the function finds all assignments that affect stmt2.
	//if exhaustive is false, the function stops once one assignment is found.
	vector<CFGNode*> visitedNodes;
	stack<STMT> callStack;
	STMT_LIST results;

	STMT_LIST stmt2s; stmt2s.push_back(stmt2);
	VAR_INDEX_LIST usedVars;
	_pkb->uses(&stmt2s, &usedVars, 1); //get variables used by stmt2

	CFGNode* endNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt2); //get the CFGNode that contains stmt2. The node must be all-assign node
	STMT start = endNode->getStartStatement();

	for(int i=0; i<usedVars.size(); i++) { //for each variable used by stmt2, find all statements that AffectsBip this var
		VAR_INDEX var = usedVars.at(i); //choose one variable used by stmt2
		VAR_INDEX_LIST vars; vars.push_back(var); //need a list to query Modifies relation
		bool carryOn = true; //indicate whether to proceed to previous CFGNode
		for(int k=stmt2-1; k>=start; k--) { //statements 'start' to 'stmt2-1' are assigns
			STMT_LIST temp1; temp1.push_back(k);
			if(_pkb->modifies(&temp1, &vars, 0)) { //means statement k modifies variable var
				results.push_back(k);
				if(!exhaustive)
					return results;

				carryOn = false;
				break;
			}
		}

		if(carryOn) { //should proceed to previous CFGNode
			getAffectsBipFirstHelper(endNode, visitedNodes, vars, exhaustive, &results, callStack);
			if(!exhaustive &&results.size()>0)
				return results;
			visitedNodes.clear();
		}
	}

	return results;
}

void AffectsBip::getAffectsBipFirstHelper(CFGNode* node2, vector<CFGNode*> visitedNodes,
	VAR_INDEX_LIST usedVar, BOOLEAN exhaustive, STMT_LIST* results_p, stack<STMT> callStack)
{
	vector<CFGNode*> prev_nodes = node2->getPrevEdges();
	if(prev_nodes.size() == 0) //no more predecessor, base case, stop and return
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
				bool carryOn = true; //indicate whether to proceed to previous CFGNode
				if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
					//this node is if or while or call or dummy, go to previous CFGNode directly
				} else {
					int end1 = currentNode->getEndStatement(); int start1 = currentNode->getStartStatement();
					for(int k=end1; k>=start1; k--) {
						STMT_LIST temp1; temp1.push_back(k);
						if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
							results_p->push_back(k);
							if(!exhaustive)
								return;

							carryOn = false;
							break;
						}
					}
				}
				
				if(carryOn) { //should proceed to previous CFGNode
					getAffectsBipFirstHelper(currentNode, visitedNodes, usedVar, exhaustive, results_p, callStack);
					if(!exhaustive &&results_p->size()>0)
						return;
				}
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
				bool carryOn = true; //indicate whether to proceed to previous CFGNode
				if(nodeInSameProc->getCFGType() != CFG_NORMAL_BLOCK) {
					//this node is if or while or call or dummy, go to previous CFGNode directly
				} else {
					int end1 = nodeInSameProc->getEndStatement(); int start1 = nodeInSameProc->getStartStatement();
					for(int k=end1; k>=start1; k--) {
						STMT_LIST temp1; temp1.push_back(k);
						if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
							results_p->push_back(k);
							if(!exhaustive)
								return;

							carryOn = false;
							break;
						}
					}
				}
				
				if(carryOn) { //should proceed to previous CFGNode
					getAffectsBipFirstHelper(nodeInSameProc, visitedNodes, usedVar, exhaustive, results_p, callStack);
					if(!exhaustive &&results_p->size()>0)
						return;
				}
			}
		}
		

		//jump back to caller or back to return node, no prev nodes within same proc
		for(int i=0; i<prev_nodes.size(); i++) {
			CFGNode* currentNode = prev_nodes.at(i);
			if(currentNode->getBipType() == CFG_BIP_OUT) { //jump to return nodes
				STMT_LIST prevStmts = _pkb->getNextFirst(node2->getStartStatement());
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

							bool carryOn = true; //indicate whether to proceed to previous CFGNode
							if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
								//this node is if or while or call or dummy, go to previous CFGNode directly
							} else {
								int end1 = currentNode->getEndStatement(); int start1 = currentNode->getStartStatement();
								for(int k=end1; k>=start1; k--) {
									STMT_LIST temp1; temp1.push_back(k);
									if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
										results_p->push_back(k);
										if(!exhaustive)
											return;

										carryOn = false;
										break;
									}
								}
							}
				
							if(carryOn) { //should proceed to previous CFGNode
								getAffectsBipFirstHelper(currentNode, visitedNodes, usedVar, exhaustive, results_p, callStackCopy);
								if(!exhaustive &&results_p->size()>0)
									return;
							}
						}			
					}
				}			
			} else { //jump back to caller
				assert(currentNode->getBipType() == CFG_BIP_IN);
				// clear visited nodes that are within the same procedure before jump back
				vector<CFGNode*> visitedNodesCopy = visitedNodes;
				PROC_INDEX currentProc = node2->getProcIndex();
				STMT stmt1 = _pkb->getProcStart(currentProc); STMT stmt2 = _pkb->getProcEnd(currentProc);
				for(int i=stmt1; i<=stmt2; i++) {
					int index = indexOf((visitedNodesCopy), _pkb->getCFGBip()->getCFGNodeByStmtNumber(i));
					if(index >=0)
						visitedNodesCopy.erase(visitedNodesCopy.begin()+index);
				}

				// pop call stack
				if(callStack.empty()) { // do not come from a label, need to jump back via all labels	
					if(indexOf((visitedNodesCopy), currentNode) >= 0){
						//current successor has been visited, do not visit it again
					} else {
						visitedNodesCopy.push_back(currentNode); //mark current successor as visited to avoid re-visit
						bool carryOn = true; //indicate whether to proceed to previous CFGNode
						if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
							//this node is if or while or call or dummy, go to previous CFGNode directly
						} else {
							int end1 = currentNode->getEndStatement(); int start1 = currentNode->getStartStatement();
							for(int k=end1; k>=start1; k--) {
								STMT_LIST temp1; temp1.push_back(k);
								if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
									results_p->push_back(k);
									if(!exhaustive)
										return;

									carryOn = false;
									break;
								}
							}
						}
				
						if(carryOn) { //should proceed to previous CFGNode
							getAffectsBipFirstHelper(currentNode, visitedNodesCopy, usedVar, exhaustive, results_p, callStack);
							if(!exhaustive &&results_p->size()>0)
								return;
						}
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
						bool carryOn = true; //indicate whether to proceed to previous CFGNode
						if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
							//this node is if or while or call or dummy, go to previous CFGNode directly
						} else {
							int end1 = currentNode->getEndStatement(); int start1 = currentNode->getStartStatement();
							for(int k=end1; k>=start1; k--) {
								STMT_LIST temp1; temp1.push_back(k);
								if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
									results_p->push_back(k);
									if(!exhaustive)
										return;

									carryOn = false;
									break;
								}
							}
						}
				
						if(carryOn) { //should proceed to previous CFGNode
							getAffectsBipFirstHelper(currentNode, visitedNodesCopy, usedVar, exhaustive, results_p, callStackCopy);
							if(!exhaustive &&results_p->size()>0)
								return;
						}
					}
				}
			}
		}
	}
}

STMT_LIST AffectsBip::getAffectsBipSecond(STMT stmt1, BOOLEAN exhaustive)
{
	vector<CFGNode*> visitedNodes;
	stack<STMT> callStack;
	STMT_LIST results;

	STMT_LIST stmt1s; stmt1s.push_back(stmt1);
	VAR_INDEX_LIST modifiedVar;
	_pkb->modifies(&stmt1s, &modifiedVar, 1); //get the variable modified by stmt1

	CFGNode* startNode = _pkb->getCFGBip()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1, the node must be a CFG_NORMAL_BLOCK
	STMT end = startNode->getEndStatement(); //the end statement of this CFGNode

	//one and only one variable can be modified by an assign statement, so size of modifiedVar must be 1
	//only need to check AffectsBip for this single variable 'var' inside modifiedVar

	bool carryOn = true; //indicate whether to proceed to next CFGNode

	for(int k=stmt1+1; k<=end; k++) { //statements 'stmt+1' to 'end' are assign statements
		STMT_LIST temp1; temp1.push_back(k);

		if(_pkb->uses(&temp1, &modifiedVar, 0)) {
			results.push_back(k);
			if(!exhaustive)
				return results;
		}
		if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken
			carryOn = false;
			break;
		}
	}

	if(carryOn) { //should proceed to next CFGNode
		getAffectsBipSecondHelper(startNode, &visitedNodes, modifiedVar, exhaustive, &results, callStack);
		//if(!exhaustive && results.size() >0)
			//return results;
	}

	return results;
}

void AffectsBip::getAffectsBipSecondHelper(CFGNode* node1, vector<CFGNode*>* visitedNodes,
	VAR_INDEX_LIST modifiedVar, BOOLEAN exhaustive, STMT_LIST* results_p, stack<STMT> callStack)
{
	if(node1->getBipType() == CFG_BIP_IN) { //call node
		callStack.push(node1->getStartStatement()); //push to call stack
		CFGNode* next_node = node1->getNextEdges().at(0); //there will be exactly one next node
		if(indexOf((*visitedNodes), next_node) >= 0) {
			//current successor has been visited, do not visit it again
		} else {
			visitedNodes->push_back(next_node); //mark current successor as visited to avoid re-visit
			bool carryOn = true;

			if(next_node->getCFGType() != CFG_NORMAL_BLOCK) {
				//this node is if or while or call or dummy, go to next CFGNode directly
			} else { //this node is normal block, which contains all assign statements
				int start1 = next_node->getStartStatement(); int end1 = next_node->getEndStatement();
				for(int k=start1; k<=end1; k++) {
					STMT_LIST temp1; temp1.push_back(k);
					if(_pkb->uses(&temp1, &modifiedVar, 0)) {
						results_p->push_back(k);
						if(!exhaustive)
							return;
					}
					if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken
						carryOn = false;
						break;
					}
				}
			}

			if(carryOn) { //should proceed to next CFGNode
				getAffectsBipSecondHelper(next_node, visitedNodes, modifiedVar, exhaustive, results_p, callStack);
				if(!exhaustive && results_p->size() >0)
					return;
			}			
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
					bool carryOn = true;

					if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
						//this node is if or while or call or dummy, go to next CFGNode directly
					} else { //this node is normal block, which contains all assign statements
						int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
						for(int k=start1; k<=end1; k++) {
							STMT_LIST temp1; temp1.push_back(k);
							if(_pkb->uses(&temp1, &modifiedVar, 0)) {
								results_p->push_back(k);
								if(!exhaustive)
									return;
							}
							if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken
								carryOn = false;
								break;
							}
						}
					}

					if(carryOn) { //should proceed to next CFGNode
						getAffectsBipSecondHelper(currentNode, visitedNodes, modifiedVar, exhaustive, results_p, callStack);
						if(!exhaustive && results_p->size() >0)
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
		STMT stmt1 = _pkb->getProcStart(currentProc); STMT stmt2 = _pkb->getProcEnd(currentProc);
		for(int i=stmt1; i<=stmt2; i++) {
			int index = indexOf((*visitedNodes), _pkb->getCFGBip()->getCFGNodeByStmtNumber(i));
			if(index >=0)
				visitedNodes->erase(visitedNodes->begin()+index);
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
						bool carryOn = true;

						if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
							//this node is if or while or call or dummy, go to next CFGNode directly
						} else { //this node is normal block, which contains all assign statements
							int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
							for(int k=start1; k<=end1; k++) {
								STMT_LIST temp1; temp1.push_back(k);
								if(_pkb->uses(&temp1, &modifiedVar, 0)) {
									results_p->push_back(k);
									if(!exhaustive)
										return;
								}
								if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken
									carryOn = false;
									break;
								}
							}
						}

						if(carryOn) { //should proceed to next CFGNode
							getAffectsBipSecondHelper(currentNode, visitedNodes, modifiedVar, exhaustive, results_p, callStack);
							if(!exhaustive && results_p->size() >0)
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
							bool carryOn = true;

							if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
								//this node is if or while or call or dummy, go to next CFGNode directly
							} else { //this node is normal block, which contains all assign statements
								int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
								for(int k=start1; k<=end1; k++) {
									STMT_LIST temp1; temp1.push_back(k);
									if(_pkb->uses(&temp1, &modifiedVar, 0)) {
										results_p->push_back(k);
										if(!exhaustive)
											return;
									}
									if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken
										carryOn = false;
										break;
									}
								}
							}

							if(carryOn) { //should proceed to next CFGNode
								getAffectsBipSecondHelper(currentNode, visitedNodes, modifiedVar, exhaustive, results_p, callStack);
								if(!exhaustive && results_p->size() >0)
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
							bool carryOn = true;

							if(currentNode->getCFGType() != CFG_NORMAL_BLOCK) {
								//this node is if or while or call or dummy, go to next CFGNode directly
							} else { //this node is normal block, which contains all assign statements
								int start1 = currentNode->getStartStatement(); int end1 = currentNode->getEndStatement();
								for(int k=start1; k<=end1; k++) {
									STMT_LIST temp1; temp1.push_back(k);
									if(_pkb->uses(&temp1, &modifiedVar, 0)) {
										results_p->push_back(k);
										if(!exhaustive)
											return;
									}
									if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken
										carryOn = false;
										break;
									}
								}
							}

							if(carryOn) { //should proceed to next CFGNode
								getAffectsBipSecondHelper(currentNode, visitedNodes, modifiedVar, exhaustive, results_p, callStack);
								if(!exhaustive && results_p->size() >0)
									return;
							}
						}
					}
				}
			}
		}
	}
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
