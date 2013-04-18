/**
 * \file	DesignExtractor.cpp
 * \class	Design Extractor
 * \brief	Handles Pattern, With and getAll* queries and complete Modifies and Uses table
 */

#include "stdafx.h"
#include "DesignExtractor.h"
#include "SPAType.h"
#include "ASTNode.h"
#include "Utility.h"
#include <queue>
#include <string>
#include <assert.h>

/**
 * \fn	DesignExtractor::DesignExtractor(void)
 *
 * \brief	Construct DesignExtractor object containing AST object and PKBController object
 *
 * \author	Yue Cong
 */

DesignExtractor::DesignExtractor(void)
{
	_pkb = PKBController::createInstance();
	_ast = new AST();
}


DesignExtractor::~DesignExtractor(void)
{
	delete(_ast);
}

/**
 * \fn	void DesignExtractor::addModifies()
 *
 * \brief	Complete modifies table
 *			Combine parent table and previous modifies table(only for assignment)
 *
 * \author	Yue Cong
 */
void DesignExtractor::addModifies()
{
	STMT_LIST       stmts = _pkb->getAllModifiesFirst();
	VAR_INDEX_LIST  vars = _pkb->getAllModifiesSecond();
	PROC_INDEX_LIST proc = _pkb->getAllModifiesPFirst();
	VAR_INDEX_LIST  pvars;
	STMT_LIST       *parents = new STMT_LIST();
	STMT_LIST       *child = new STMT_LIST();
	STMT_LIST       *callNumbers = new STMT_LIST();
	PROC_INDEX_LIST callees = _pkb->getAllCallees();
	PROC_INDEX_LIST callers = _pkb->getAllCallers();
	PROC_INDEX_LIST pmod;

	getAllCall(callNumbers);
	for (int n = 0; n < callNumbers->size(); n++){
		for (int i = 0; i < callNumbers->size();i++)
		{
			pmod.push_back(callees.at(i));
			_pkb->modifiesP(&pmod, &pvars, 1);
			for (int j = 0; j < pvars.size(); j++)
			{
				_pkb->addModifies(callNumbers->at(i), pvars.at(j));
				_pkb->addModifiesP(callers.at(i), pvars.at(j));
			}
			pmod.clear();
			pvars.clear();
		}
	}
	stmts.clear();
	vars.clear();
	stmts = _pkb->getAllModifiesFirst();
	vars = _pkb->getAllModifiesSecond();
	for (int i = 0; i < stmts.size(); i++) {
		child->push_back(stmts.at(i));
		_pkb->parentStar(parents, child, 2);

		while(parents->size() > 0){
			_pkb->addModifies(parents->at(parents->size()-1), vars.at(i));
			parents->pop_back();
		}
		child->pop_back();
	}
}

/**
 * \fn	void DesignExtractor::addUses()
 *
 * \brief	Complete Uses table
 *			Combine parent table and previous uses table(only for assignment)
 *
 * \author	Yue Cong
 */
void DesignExtractor::addUses()
{
	STMT_LIST       stmts = _pkb->getAllUsesFirst();
	VAR_INDEX_LIST  vars = _pkb->getAllUsesSecond();
	PROC_INDEX_LIST proc = _pkb->getAllUsesPFirst();
	VAR_INDEX_LIST  pvars;
	STMT_LIST       *parents = new STMT_LIST();
	STMT_LIST       *child = new STMT_LIST();
	STMT_LIST       *callNumbers = new STMT_LIST();
	PROC_INDEX_LIST callees = _pkb->getAllCallees();
	PROC_INDEX_LIST callers = _pkb->getAllCallers();
	PROC_INDEX_LIST pmod;

	getAllCall(callNumbers);
	for (int n = 0; n < callNumbers->size();n++){
		for (int i = 0; i < callNumbers->size();i++)
		{
			pmod.push_back(callees.at(i));
			_pkb->usesP(&pmod, &pvars, 1);
			for (int j = 0; j < pvars.size(); j++)
			{
				_pkb->addUses(callNumbers->at(i), pvars.at(j));
				_pkb->addUsesP(callers.at(i), pvars.at(j));
			}
			pmod.clear();
			pvars.clear();
		}
	}
	stmts.clear();
	vars.clear();
	stmts = _pkb->getAllUsesFirst();
	vars = _pkb->getAllUsesSecond();
	for (int i = 0; i < stmts.size(); i++) {
		child->push_back(stmts.at(i));
		_pkb->parentStar(parents, child, 2);
		while(parents->size() > 0){
			_pkb->addUses(parents->at(parents->size()-1), vars.at(i));
			parents->pop_back();
		}
		child->pop_back();
	}
}

/**
 * \fn	BOOLEAN DesignExtractor::getAllStmt(STMT_LIST *result)
 *
 * \brief	Add all program line that is a statement to result list
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllStmt(STMT_LIST *result)
{
	for (int i = 0; i < _pkb->getPreprocessedProgram()->size(); i++) {
		if (_pkb->getPreprocessedProgram()->at(i).stmtNumber > 0) {
			result->push_back(_pkb->getPreprocessedProgram()->at(i).stmtNumber);
		}
	}
	if (result->size() == 0) {
		return(false);
	}
	return(true);
}

/**
 * \fn	BOOLEAN DesignExtractor::getAllAssignment(STMT_LIST *result)
 *
 * \brief	Add all program line that is an assignment to result list
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllAssignment(STMT_LIST *result)
{
	for (int i = 0; i < _pkb->getPreprocessedProgram()->size(); i++) {
		if (_pkb->getPreprocessedProgram()->at(i).type == STMT_ASSIGNMENT) {
			result->push_back(_pkb->getPreprocessedProgram()->at(i).stmtNumber);
		}
	}
	if (result->size() == 0) {
		return(false);
	}
	return(true);
}

/**
 * \fn	BOOLEAN DesignExtractor::getAllWhile(STMT_LIST *result)
 *
 * \brief	Add all program line that is a while to result list
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllWhile(STMT_LIST *result)
{
	for (int i = 0; i < _pkb->getPreprocessedProgram()->size(); i++) {
		if (_pkb->getPreprocessedProgram()->at(i).type == STMT_WHILE) {
			result->push_back(_pkb->getPreprocessedProgram()->at(i).stmtNumber);
		}
	}
	if (result->size() == 0) {
		return(false);
	}
	return(true);
}

/**
 * \fn	BOOLEAN DesignExtractor::getAllStmtList(STMT_LIST *result)
 *
 * \brief	Add all program line that is the first statement of a statement list.
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllStmtList(STMT_LIST *result)
{
	for (int i = 0; i < _pkb->getPreprocessedProgram()->size(); i++) {
		if (_pkb->getPreprocessedProgram()->at(i).type == STMT_OPEN_BRACKET) {
			result->push_back(_pkb->getPreprocessedProgram()->at(i+1).stmtNumber);
		}
	}
	if (result->size() == 0) {
		return(false);
	}
	return(true);
}

/**
 * \fn	BOOLEAN DesignExtractor::getAllIf(STMT_LIST *result)
 *
 * \brief	Add all program line that is an if to result list
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllIf(STMT_LIST *result)
{
	for (int i = 0; i < _pkb->getPreprocessedProgram()->size(); i++) {
		if (_pkb->getPreprocessedProgram()->at(i).type == STMT_IF) {
			result->push_back(_pkb->getPreprocessedProgram()->at(i).stmtNumber);
		}
	}
	if (result->size() == 0) {
		return(false);
	}
	return(true);
}

/**
 * \fn	BOOLEAN DesignExtractor::getAllCall(STMT_LIST *result)
 *
 * \brief	Add all program line that is a call statement to result list
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllCall(STMT_LIST *result)
{
	for (int i = 0; i < _pkb->getPreprocessedProgram()->size(); i++) {
		if (_pkb->getPreprocessedProgram()->at(i).type == STMT_CALL) {
			result->push_back(_pkb->getPreprocessedProgram()->at(i).stmtNumber);
		}
	}
	if (result->size() == 0) {
		return(false);
	}
	return(true);
}

//not complete
/**
 * \fn	BOOLEAN DesignExtractor::getAllConstant(vector<int> *result)
 *
 * \brief	Add all constant to result vector
 *
 * \author	Yue Cong
 *
 * \param [out]	True or False	If there is no item in result list, return false. else return true.
 */
BOOLEAN DesignExtractor::getAllConstant(vector<int> *result)
{
	/*
	* regex constant("[-\\+\\*\\s]([0-9]+)[-\\+\\*\\s]");
	* for ( int i = 0; i < _pkb->getPreprocessedProgram()->size();i++){
	*  smatch constants;
	*  if(_pkb->getPreprocessedProgram()->at(i).type == STMT_ASSIGNMENT){
	*      regex_match(_pkb->getPreprocessedProgram()->at(i).stmtLine,constants, constant);
	*          for(int j = 0; j < constants.size(); j++){
	*              result->push_back(constants(j));
	*          }
	*  }
	* }
	* if (result->size() == 0) return false;
	* return true;
	*/
	vector<int> *constTable = _pkb->getConstantTable();
	for (int i = 0; i < constTable->size(); i++) {
		result->push_back(constTable->at(i));
	}
	return(true);
}

/**
 * \fn	BOOLEAN DesignExtractor::pattern(vector<int> *a, vector<int> *b, string expr, int arg)
 *
 * \brief	Handles patterns query

 *----------------------------------------------------------------------------------------------------------------------------
 *			Case 1: arg = 00 = 0 
 *			(Both independent - only need to return TRUE FALSE)
 *
 *			1a) vectorA empty, vectorB empty
 *				Return TRUE if expr exists on the RHS of assignment statements, FASLE otherwise.
 *			1b) vectorA empty, vectorB size = 1 (element b)
 *				Return TRUE if there exists assignment statement a such that pattern a(b, expr) is true, FASLE otherwise.
 *			1c) vectorA size >= 1, vectorB empty
 *				Return TRUE if expr exists on the RHS of assignment statements, FASLE otherwise.
 *			1d) vectorA size >= 1,, vectorB size = 1 (element b)
 *				Return TRUE if there exists assignment statement a such that pattern a(b, expr) is true, FASLE otherwise.
 *
 *				a and c are the same, b and d are the same.
 *
 *----------------------------------------------------------------------------------------------------------------------------
 *
 *			Case 2: arg = 01 = 1 
 *			(A independent, B dependent - Modify vectorB, don¡¯t modify vectorA)
 *
 *			2a) vectorA empty, vectorB empty
 *				Fill up vectorB with all the variable index on LHS such that RHS contains expr.
 *			2b) vectorA size >= 1, vectorB empty
 *				For assignment statements in A, fill up vectorB with all the variable index on LHS where RHS contains expr..
 *			2c) vectorA empty, vectorB size >= 1 
 *				Remove elements from vectorB where RHS does not contain expr. 
 *			2d) vectorA size >= 1, vectorB size >= 1 
 *				For assignment statements in A, remove elements from vectorB where LHS = b and RHS does not contain expr. 
 *
 *			For case 2a -> 2d:
 *			Return TRUE if at the end, vectorB is not empty, FASLE otherwise.
 *
 *----------------------------------------------------------------------------------------------------------------------------
 *
 *			Case 3: arg = 10 = 2 
 *			(A dependent, B independent - Modify vectorA, don¡¯t modify vectorB)
 *
 *			3a) vectorA empty, vectorB empty
 *				Fill up vectorA with all the assignment statements where RHS contains expr.
 *			3b) vectorA empty, vectorB size 1 (element b)
 *				Fill up vectorA with all the assignment statements where LHS = b, and RHS contains expr..
 *			3c) vectorA size >= 1, vectorB empty
 *				Remove elements from vectorA where RHS does not contain expr. 
 *			3d) vectorA size >= 1, vectorB size 1 (element b)
 *				Remove elements from vectorA where LHS != b or RHS does not contain expr. 
 *
 *			For case 3a -> 3d:
 *			Return TRUE if at the end, vectorA is not empty, FASLE otherwise.
 *
 *----------------------------------------------------------------------------------------------------------------------------
 *
 *			Case 4: arg = 11 = 3 
 *			(Both dependent - Modify both vectors)
 *
 *			4a) vectorA empty, vectorB empty
 *				Fill up both vectors with all (ai, bi) where pattern ai(bi, expr) is true.
 *			4b) vectorA empty, vectorB size >= 1
 *				For each element (bi) in vectorB, find assignment statements a such that pattern a(bi, expr) is true.
 *					(i) If found no a, remove bi from vectorB.
 *					(ii) If found 1 a, add a to vectorA.
 *					(iii) if found n>1 a, add them to vectorA, duplicate bi n times in vectorB.
 *			4c) vectorA size >= 1, vectorB empty
 *				For each element (ai) in vectorA, find variables b such that pattern ai(b, expr) is true.
 *				Remove elements from vectorA where RHS does not contain expr. Add fill up vectorB
 *			4d) vectorA size = n, vectorB size = n (where n >= 1)
 *				Let ai be the ith element of vectorA and bi be the ith element of vectorB. For i from 1 to n, remove ai, bi from vectorA, vectorB if pattern ai(bi, expr) is false.
 *
 *			For case 4a -> 4d:
 *			At the end, size of vectorA = size of vectorB.
 *
 * \author	Yue Cong
 *
 * \param [in]	a: list of assignment statement number; 
 *				b: list of variables on left hand side; 
 *				expr: expression to be matched on right hand side; 
 *				arg: 0-3 according to dependency.
 *
 * \param [out]	True or False	If there is no item in a or b, return false. else return true.
 */
BOOLEAN DesignExtractor::pattern(vector<int>* a, vector<int>* b, string expr, PatternQueryType queryType,int arg)
{
	switch(queryType){
	case PATTERN_ASSIGN_NO_UNDERSCORE:
		return assignPattern(a,b,expr, arg, 0);
		break;
	case PATTERN_ASSIGN_UNDERSCORE_ONLY:
	case PATTERN_ASSIGN_WITH_UNDERSCORE:
		return assignPattern(a, b, expr, arg, 1);
		break;
	case PATTERN_IF:
		return ifPattern(a, b, expr, arg);
		break;
	case PATTERN_WHILE:
		return whilePattern(a, b, expr, arg);
		break;
	default:
		break;
	}
	return false;
}

BOOLEAN DesignExtractor::whilePattern(vector<int> *a, vector<int> *b, string expr, int arg){
	vector<ASTNode *> *whileLoop = _pkb->getWhileTable();
	vector<int> *temp = new vector<int>();
	int flag = 0;
	if (arg == 0)
	{
		//1a
		if(b->size() == 0)
		{
			if(whileLoop->size()!=0) return true;
			else return false;
		}
		//1b
		else if (b->size() == 1)
		{
			for(int i = 0; i < whileLoop->size(); i++){
				if(b->at(0) == whileLoop->at(i)->getChildren()->getNodeValue()) return true;
			}
			return false;
		}
	}
	else if (arg == 1)
	{
		//2a
		if (a->size() == 0 && b->size() == 0)
		{
			for(int i = 0; i < whileLoop->size(); i++){
				b->push_back(whileLoop->at(i)->getChildren()->getNodeValue());
			}
			if(b->size() == 0) return false;
			return true;
		}
		//2b
		else if (a->size() > 0 && b->size() == 0)
		{
			for (int j = 0; j < a->size(); j++){
				for(int i = 0; i < whileLoop->size(); i++){
					if(whileLoop->at(i)->getStmtNumber() == a->at(j)){
						b->push_back(whileLoop->at(i)->getChildren()->getNodeValue());
						break;
					}
				}
			}
			if(b->size() == 0) return false;
			return true;
		}
		//2c
		else if (a->size() == 0 && b->size() > 0)
		{
			int i = 0;
			while (i < b->size())
			{
				flag = 0;
				for (int j = 0; j < whileLoop->size(); j++)
				{
					if (b->at(i) == whileLoop->at(j)->getChildren()->getNodeValue())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if(flag == 0) b->erase(b->begin() + i);
			}
			if(b->size() == 0) return false;
			return true;
		}
		//2d
		else if (a->size() > 0 && b->size() > 0)
		{
			int i = 0;
			vector<int> temp = *a;
			while (i < b->size())
			{
				flag = 0;
				for (int j = 0; j < whileLoop->size(); j++)
				{
					if (indexOf(temp, whileLoop->at(j)->getStmtNumber()) >= 0 && b->at(i) == whileLoop->at(j)->getChildren()->getNodeValue())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if(flag == 0) b->erase(b->begin() + i);
			}
			if(b->size() == 0) return false;
			return true;
		}
	}
	else if (arg == 2)
	{
		//3a
		if (a->size() == 0 && b->size() == 0)
		{
			for(int i = 0; i < whileLoop->size(); i++){
				a->push_back(whileLoop->at(i)->getStmtNumber());
			}
			if(a->size() == 0) return false;
			return true;
		}
		//3b
		else if (a->size() == 0 && b->size() ==1)
		{
			for(int i = 0; i < whileLoop->size(); i++){
				if(whileLoop->at(i)->getChildren()->getNodeValue() == b->at(i))
					a->push_back(whileLoop->at(i)->getStmtNumber());
			}
			if(a->size() == 0) return false;
			return true;
		}
		//3c
		else if (a->size() > 0 && b->size() == 0)
		{
			int i = 0;
			while (i < a->size())
			{
				flag = 0;
				for (int j = 0; j < whileLoop->size(); j ++ )
				{
					if (a->at(i) == whileLoop->at(j)->getStmtNumber())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if (flag == 0) a->erase(a->begin()+i);
			}
			if (a->size() == 0) return false;
			return true;
		}
		//3d
		else if (a->size() > 0 && b->size() == 1)
		{
			int i = 0;
			while (i < a->size())
			{
				flag = 0;
				for(int j = 0; j < whileLoop->size(); j++){
					if (whileLoop->at(j)->getStmtNumber() == a->at(i) && whileLoop->at(j)->getChildren()->getNodeValue() == b->at(0))
					{
						i++;
						flag = 1;
						break;
					}
				}
				if(flag == 0) a->erase(a->begin()+i);
			}
			if(a->size() == 0) return false;
			return true;
		}
	}
	else if (arg == 3)
	{
		//4a
		if (a->size() == 0 && b->size() == 0)
		{
			for (int i = 0; i < whileLoop->size(); i++)
			{
				a->push_back(whileLoop->at(i)->getStmtNumber());
				b->push_back(whileLoop->at(i)->getChildren()->getNodeValue());
			}
			if(a->size() == 0) return false;
			return true;
		}
		//4b
		else if (a->size() == 0 && b->size() > 0)
		{
			for (int i = 0; i < b->size(); i++)
			{
				temp->push_back(b->at(i));
			}
			b->clear();
			for (int i = 0; i < whileLoop->size(); i++)
			{
				for (int j = 0; j < temp->size(); j++)
				{
					if(whileLoop->at(i)->getChildren()->getNodeValue() == temp->at(j)){
						a->push_back(whileLoop->at(i)->getStmtNumber());
						b->push_back(temp->at(j));
						break;
					}
				}
			}
			if (a->size() == 0) return false;
			return true;
		}
		//4c
		else if (a->size() > 0 && b->size() == 0)
		{
			int i = 0;
			while (i < a->size()){
				flag = 0;
				for (int j = 0; j < whileLoop->size(); j++)
				{
					if (a->at(i) == whileLoop->at(j)->getStmtNumber())
					{
						b->push_back(whileLoop->at(j)->getChildren()->getStmtNumber());
						i++;
						flag = 1;
						break;
					}
				}
				if (flag == 0) a->erase(a->begin() + i);
			}
			if(a->size() == 0) return false;
			return true;
		}
		//4d
		else if (a->size() > 0 && b->size() > 0)
		{
			int i = 0;
			while (i < a->size())
			{
				flag = 0;
				for (int j = 0; j < whileLoop->size(); j++)
				{
					if (a->at(i) == whileLoop->at(j)->getStmtNumber() && b->at(i) == whileLoop->at(j)->getChildren()->getNodeValue())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					a->erase(a->begin() + i);
					b->erase(b->begin() + i);
				}
			}
		}
	}
	else
	{
		printf("wrong input format");
		exit(1);
	}
	return false;
}

BOOLEAN DesignExtractor::ifPattern(vector<int> *a, vector<int> *b, string expr, int arg){
	vector<ASTNode *> *ifBranch = _pkb->getIfBranchTable();
	vector<int> *temp = new vector<int>();
	int i = 0, flag = 0;
	if (arg == 0)
	{
		//case 1a
		if(b->size() == 0)
		{
			if (ifBranch->size() == 0)
				return  false;
			return true;
		}
		//case 1b
		else if (b->size() == 1)
		{
			temp = _pkb->getAST()->matchIfBranch(b->at(0));
			if (temp->size() == 0)
			{
				delete(temp);
				return false;
			}
			delete(temp);
			return true;
		}
	}
	else if (arg == 1)
	{
		//case 2a
		if (a->size() == 0 && b->size() == 0)
		{
			for(int j = 0; j < ifBranch->size(); j++){
				b->push_back(ifBranch->at(j)->getNodeValue());
			}
			if(b->size() == 0) return false;
			return true;
		}
		//2b
		else if (a->size() > 0 && b->size() == 0)
		{
			for (int j = 0; j < a->size(); j++){
				for(int i = 0; i < ifBranch->size(); i++){
					if(ifBranch->at(i)->getStmtNumber() == a->at(j)){
						b->push_back(ifBranch->at(i)->getChildren()->getNodeValue());
						break;
					}
				}
			}
			if(b->size() == 0) return false;
			return true;
		}
		//2c
		else if (a->size() == 0 && b->size() > 0)
		{
			int i = 0;
			while (i < b->size())
			{
				flag = 0;
				for (int j = 0; j < ifBranch->size(); j++)
				{
					if (b->at(i) == ifBranch->at(j)->getChildren()->getNodeValue())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if(flag == 0) b->erase(b->begin() + i);
			}
			if(b->size() == 0) return false;
			return true;
		}
		//2d
		else if (a->size() > 0 && b->size() > 0)
		{
			int i = 0;
			vector<int> temp = *a;
			while (i < b->size())
			{
				flag = 0;
				for (int j = 0; j < ifBranch->size(); j++)
				{
					if (indexOf(temp, ifBranch->at(j)->getStmtNumber()) >= 0 && b->at(i) == ifBranch->at(j)->getChildren()->getNodeValue())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if(flag == 0) b->erase(b->begin() + i);
			}
			if(b->size() == 0) return false;
			return true;
		}
	}
	else if (arg == 2)
	{
		//3a
		if (a->size() == 0 && b->size() == 0)
		{
			for(int i = 0; i < ifBranch->size(); i++){
				a->push_back(ifBranch->at(i)->getStmtNumber());
			}
			if(a->size() == 0) return false;
			return true;
		}
		//3b
		else if (a->size() == 0 && b->size() ==1)
		{
			for(int i = 0; i < ifBranch->size(); i++){
				if(ifBranch->at(i)->getChildren()->getNodeValue() == b->at(i))
					a->push_back(ifBranch->at(i)->getStmtNumber());
			}
			if(a->size() == 0) return false;
			return true;
		}
		//3c
		else if (a->size() > 0 && b->size() == 0)
		{
			int i = 0;
			while (i < a->size())
			{
				flag = 0;
				for (int j = 0; j < ifBranch->size(); j ++ )
				{
					if (a->at(i) == ifBranch->at(j)->getStmtNumber())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if (flag == 0) a->erase(a->begin()+i);
			}
			if (a->size() == 0) return false;
			return true;
		}
		//3d
		else if (a->size() > 0 && b->size() == 1)
		{
			int i = 0;
			while (i < a->size())
			{
				flag = 0;
				for(int j = 0; j < ifBranch->size(); j++){
					if (ifBranch->at(j)->getStmtNumber() == a->at(i) && ifBranch->at(j)->getChildren()->getNodeValue() == b->at(0))
					{
						i++;
						flag = 1;
						break;
					}
				}
				if(flag == 0) a->erase(a->begin()+i);
			}
			if(a->size() == 0) return false;
			return true;
		}
	}
	else if (arg == 3)
	{
		//4a
		if (a->size() == 0 && b->size() == 0)
		{
			for (int i = 0; i < ifBranch->size(); i++)
			{
				a->push_back(ifBranch->at(i)->getStmtNumber());
				b->push_back(ifBranch->at(i)->getChildren()->getNodeValue());
			}
			if(a->size() == 0) return false;
			return true;
		}
		//4b
		else if (a->size() == 0 && b->size() > 0)
		{
			for (int i = 0; i < b->size(); i++)
			{
				temp->push_back(b->at(i));
			}
			b->clear();
			for (int i = 0; i < ifBranch->size(); i++)
			{
				for (int j = 0; j < temp->size(); j++)
				{
					if(ifBranch->at(i)->getChildren()->getNodeValue() == temp->at(j)){
						a->push_back(ifBranch->at(i)->getStmtNumber());
						b->push_back(temp->at(j));
						break;
					}
				}
			}
			if (a->size() == 0) return false;
			return true;
		}
		//4c
		else if (a->size() > 0 && b->size() == 0)
		{
			int i = 0;
			while (i < a->size()){
				flag = 0;
				for (int j = 0; j < ifBranch->size(); j++)
				{
					if (a->at(i) == ifBranch->at(j)->getStmtNumber())
					{
						b->push_back(ifBranch->at(j)->getChildren()->getStmtNumber());
						i++;
						flag = 1;
						break;
					}
				}
				if (flag == 0) a->erase(a->begin() + i);
			}
			if(a->size() == 0) return false;
			return true;
		}
		//4d
		else if (a->size() > 0 && b->size() > 0)
		{
			int i = 0;
			while (i < a->size())
			{
				flag = 0;
				for (int j = 0; j < ifBranch->size(); j++)
				{
					if (a->at(i) == ifBranch->at(j)->getStmtNumber() && b->at(i) == ifBranch->at(j)->getChildren()->getNodeValue())
					{
						i++;
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					a->erase(a->begin() + i);
					b->erase(b->begin() + i);
				}
			}
		}
	}
	else
	{
		printf("wrong input format");
		exit(1);
	}
	return false;
}

BOOLEAN DesignExtractor::assignPattern(vector<int> *a, vector<int> *b, string expr, int arg, int flag)
{
	ASTNode *tree;
	vector<ASTNode *> *assign = _pkb->getAssignmentTable();

	vector<int> *temp = new vector<int>();
	int i = 0, flag2 = 0;
	/*
	if (expr!="" && expr[0] == '"') {
		expr = expr.substr(1, expr.size()-2);
		flag = 1;
	}
	*/
	if (arg == 0) {
		if ((b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(temp);
			}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
				if (flag == 0) {
					temp = _pkb->getAST()->containTree(tree);
				}else {
					temp = _pkb->getAST()->containSubtree(tree);
				}
			}
			if (temp->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((b->size() == 1)) {
			if(expr == ""){
				for(int j = 0; j < assign->size(); j++){
					if(b->at(0) == assign->at(j)->getChildren()->getNodeValue()){
						temp->push_back(assign->at(j)->getStmtNumber());
					}
				}
			}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(b->at(0), expr);
				if (flag == 0) {
					temp = _pkb->getAST()->containTree(tree);
				}else {
					temp = _pkb->getAST()->containSubtree(tree);
				}
			}
			if (temp->size() == 0) {
				return(false);
			}
			return(true);
		}else {
			printf("wrong input format");
			exit(1);
		}
	}else if (arg == 1){
		if ((a->size() == 0) && (b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(a);
			}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
				if (flag == 0) {
					a = _pkb->getAST()->containTree(tree);
				}else {
					a = _pkb->getAST()->containSubtree(tree);
				}
			}
			_pkb->modifies(a, b, 1);
			if(b->size() == 0){
				return false;
			}
			return true;
		}else if ((a->size() > 0) && (b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(temp);
			}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
				if (flag == 0) {
					temp = _pkb->getAST()->containTree(tree);
				}else {
					temp = _pkb->getAST()->containSubtree(tree);
				}
			}
			i = 0;
			while (i < a->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (a->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					a->erase(a->begin() + i);
				}
			}
			_pkb->modifies(a, b, 1);
			if (b->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() == 0) && (b->size() > 0)) {
			for (int j = 0; j < b->size(); j++) {
				if(expr == ""){
					for(int j = 0; j < assign->size(); j++){
						if(b->at(0) == assign->at(j)->getChildren()->getNodeValue()){
							temp->push_back(assign->at(j)->getStmtNumber());
						}
					}
				}else{
					tree = _pkb->getAST()->buildAssignmentSubtree(b->at(j), expr);
					if (flag == 0) {
						temp = _pkb->getAST()->containTree(tree);
					}else {
						temp = _pkb->getAST()->containSubtree(tree);
					}
				}
				for (int k = 0; k < temp->size(); k++) {
					a->push_back(temp->at(k));
				}
			}
			_pkb->modifies(a, b, 1);
			if (b->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() > 0) && (b->size() > 0)) {
			if(expr == ""){
				getAllAssignment(temp);
			}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
				if (flag == 0) {
					temp = _pkb->getAST()->containTree(tree);
				}else {
					temp = _pkb->getAST()->containSubtree(tree);
				}
			}
			i = 0;
			while (i < a->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (a->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					a->erase(a->begin() + i);
				}
			}
			temp->clear();
			temp = b;
			_pkb->modifies(a, b, 1);
			i = 0;
			while (i < b->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (b->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					b->erase(b->begin() + i);
					a->erase(a->begin() + i);
				}
			}
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else {
			printf("wrong input format");
			exit(1);
		}
	}else if (arg == 2) {
		if ((a->size() == 0) && (b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(a);
			}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
			if (flag == 0) {
				a = _pkb->getAST()->containTree(tree);
			}else {
				a = _pkb->getAST()->containSubtree(tree);
			}
			}
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() == 0) && (b->size() == 1)) {
			if(expr == ""){
				for(int j = 0; j < assign->size(); j++){
					if(b->at(0) == assign->at(j)->getChildren()->getNodeValue()){
						a->push_back(assign->at(j)->getStmtNumber());
					}
				}
			}else{
			tree = _pkb->getAST()->buildAssignmentSubtree(b->at(0), expr);
			if (flag == 0) {
				a = _pkb->getAST()->containTree(tree);
			}else {
				a = _pkb->getAST()->containSubtree(tree);
			}
			}
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() > 0) && (b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(temp);
			}else{
			tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
			if (flag == 0) {
				temp = _pkb->getAST()->containTree(tree);
			}else {
				temp = _pkb->getAST()->containSubtree(tree);
			}
			}
			i = 0;
			while (i < a->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (a->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					a->erase(a->begin() + i);
				}
			}
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() > 0) && (b->size() == 1)) {
			if(expr == ""){
				for(int j = 0; j < assign->size(); j++){
					if(b->at(0) == assign->at(j)->getChildren()->getNodeValue()){
						temp->push_back(assign->at(j)->getStmtNumber());
					}
				}
			}else{
			tree = _pkb->getAST()->buildAssignmentSubtree(b->at(0), expr);
			if (flag == 0) {
				temp = _pkb->getAST()->containTree(tree);
			}else {
				temp = _pkb->getAST()->containSubtree(tree);
			}
			}
			i = 0;
			while (i < a->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (a->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					a->erase(a->begin() + i);
				}
			}
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else {
			printf("wrong input format");
			exit(1);
		}
	}else if (arg == 3) {
		if ((a->size() == 0) && (b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(a);
			}else{
			tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
			if (flag == 0) {
				a = _pkb->getAST()->containTree(tree);
			}else {
				a = _pkb->getAST()->containSubtree(tree);
			}
			}
			_pkb->modifies(a, b, arg);
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() == 0) && (b->size() > 0)) {
			for (int j = 0; j < b->size(); j++) {
				if(expr == ""){
					for(int j = 0; j < assign->size(); j++){
						if(b->at(0) == assign->at(j)->getChildren()->getNodeValue()){
							temp->push_back(assign->at(j)->getStmtNumber());
						}
					}
				}else{
				tree = _pkb->getAST()->buildAssignmentSubtree(b->at(j), expr);
				if (flag == 0) {
					temp = _pkb->getAST()->containTree(tree);
				}else {
					temp = _pkb->getAST()->containSubtree(tree);
				}
				}
				for (int k = 0; k < temp->size(); k++) {
					a->push_back(temp->at(k));
				}
			}
			_pkb->modifies(a, b, arg);
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() > 0) && (b->size() == 0)) {
			if(expr == ""){
				getAllAssignment(temp);
			}else{
			tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
			if (flag == 0) {
				temp = _pkb->getAST()->containTree(tree);
			}else {
				temp = _pkb->getAST()->containSubtree(tree);
			}
			}
			i = 0;
			while (i < a->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (a->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					a->erase(a->begin() + i);
				}
			}
			_pkb->modifies(a, b, arg);
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else if ((a->size() > 0) && (b->size() > 0)) {
			if(expr == ""){
				getAllAssignment(temp);
			}else{
			tree = _pkb->getAST()->buildAssignmentSubtree(-1, expr);
			if (flag == 0) {
				temp = _pkb->getAST()->containTree(tree);
			}else {
				temp = _pkb->getAST()->containSubtree(tree);
			}
			}
			i = 0;
			while (i < a->size()) {
				flag2 = 0;
				for (int j = 0; j < temp->size(); j++) {
					if (a->at(i) == temp->at(j)) {
						flag2 = 1;
						i++;
						break;
					}
				}
				if (flag2 == 0) {
					a->erase(a->begin() + i);
					b->erase(b->begin() + i);
				}
			}
			_pkb->modifies(a, b, arg);
			
			if (a->size() == 0) {
				return(false);
			}
			return(true);
		}else {
			printf("wrong input format");
			exit(1);
		}
	}
}

/**
 * \fn	void DesignExtractor::addNext()
 *
 * \brief	Adds next.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/15
 */
void DesignExtractor::addNext()
{
	//Going thru CFG to add next is easier to implement
	vector<CFGNode*> nodes = _pkb->getCFG()->getAllCFGNodes();
	for(vector<CFGNode*>::iterator it = nodes.begin(); it != nodes.end();++it)
	{
		CFGNode* c = *it;
		vector<CFGNode*> edges = c->getNextEdges();

		for(int i = c->getStartStatement(); i< c->getEndStatement(); i++)
			_pkb->addNext(i,i+1,c->getBipType());

		for(vector<CFGNode*>::iterator n = edges.begin(); n!= edges.end(); ++n)
			_pkb->addNext(c->getEndStatement(),(*n)->getStartStatement(),c->getBipType());
	}
	
}

/**
 * \fn	void DesignExtractor::connectCFG()
 *
 * \brief	Connects the CFG after Parser splits them.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/25
 */

void DesignExtractor::connectCFG(CFG* cfg, bool bipEnabled)
{
	stack<IfPreprocessingPhase> phaseStack;
	stack<statement> scope;
	vector<statement>* preprocProgram  = _pkb->getPreprocessedProgram();
	//Find all paired nodes first.
	// e.g. ELSE CFG Block has a paired node (which is its THEN block)
	// Which means these two paired blocks share the same Next relation.
	// Only applicable for Non BIP

	if(!bipEnabled)
	{
		for(vector<statement>::iterator it =  preprocProgram->begin(); it != preprocProgram->end(); ++it)
		{
			if(it->type == STMT_IF)
			{
				CFGNode* thenNodeEnd = cfg->getCFGNodeByStmtNumber(it->midOfTheScope);
				CFGNode* elseNodeEnd = cfg->getCFGNodeByStmtNumber(it->endOfTheScope);
				elseNodeEnd->setPairedCFGNode(thenNodeEnd);
			}
		}
	}
	
	// Start connecting CFG now
	for(vector<statement>::iterator it =  preprocProgram->begin(); it != preprocProgram->end(); ++it)
	{
		int sm = it->stmtNumber;
		if(it->type ==  STMT_IF)
		{
			scope.push(*it);
			phaseStack.push(PREPROCESS_THEN);

			//Connect If Block
			CFGNode* ifNode = cfg->getCFGNodeByStmtNumber(it->stmtNumber);
			CFGNode* thenNodeStart = cfg->getCFGNodeByStmtNumber(it->stmtNumber+1);
			CFGNode* elseNodeStart = cfg->getCFGNodeByStmtNumber(it->midOfTheScope+1);
			ifNode->connectTo(thenNodeStart);
			ifNode->connectTo(elseNodeStart);
		}
		else if (it->type == STMT_CLOSE_BRACKET_END_OF_WHILE)
		{
			phaseStack.pop();
			scope.pop();
		}
		else if (it->type == STMT_CLOSE_BRACKET_END_OF_THEN)
		{
			phaseStack.top() = PREPROCESS_ELSE;
			if(bipEnabled)
			{
				CFGNode* current = cfg->getCFGNodeByStmtNumber(it->stmtNumber);
				vector<CFGNode*> nexts = cfg->getNextCFGNodeByCurrentStatement(*it, bipEnabled);
				printf("CFGBip: Size: %d\n", nexts.size());
				for(vector<CFGNode*>::iterator it = nexts.begin(); it != nexts.end(); ++it)
				{
					//TODO: AVOID WHILE CONNECTION
					if(current->getCFGType() != CFG_CALL_STATEMENT)
						__smartConnectThisCFGToNext(*it, current);
				}
			}
		}
		else if (it->type == STMT_CLOSE_BRACKET_END_OF_ELSE)
		{
			phaseStack.pop();
			scope.pop();
		}
		else if(it->type == STMT_WHILE)
		{
			//Connect While Block
			CFGNode* whileNode = cfg->getCFGNodeByStmtNumber(it->stmtNumber);
			CFGNode* followingNode = cfg->getFollowingCFGNodeByCurrentStmtNumber(it->stmtNumber);
			if(followingNode != NULL)
				if(followingNode != whileNode && followingNode->getProcIndex() == whileNode->getProcIndex())
					whileNode->connectTo(followingNode);

			CFGNode* whileBlockEnd =  cfg->getCFGNodeByStmtNumber(it->endOfTheScope);
			
			if(!bipEnabled)
			{
				whileBlockEnd->connectTo(whileNode);
			}
			else
			{
				if(whileBlockEnd->getCFGType() != CFG_CALL_STATEMENT)
				{
					whileBlockEnd->connectTo(whileNode);
				}
			}
			
			if(whileBlockEnd->getPairedCFGNode()!=NULL)
			{
				//When adding the relationship to its paired node.
				//it must ensure that this while loop is paired node's parents.
				//there's trick to detect that. using StmtNumber
				if(it->stmtNumber < whileBlockEnd->getPairedCFGNode()->getStartStatement())
					whileBlockEnd->getPairedCFGNode()->connectTo(whileNode);
			}
			
			//Connect while statement to next statement, if any.
			CFGNode* afterWhileBlock = cfg->getFollowingCFGNodeByCurrentStmtNumber(it->endOfTheScope);

			if(afterWhileBlock == NULL && bipEnabled)
			{
				afterWhileBlock = cfg->getBipDummyNodeByProcIndex(it->procIndex);
				__smartConnectThisCFGToNext(afterWhileBlock, whileNode);
			}

			if(afterWhileBlock != NULL)	
			{
				if(afterWhileBlock->getCFGType() == CFG_DUMMY && bipEnabled)
				{
					__smartConnectThisCFGToNext(afterWhileBlock, whileNode);
				}

				if(afterWhileBlock->getCFGType() != CFG_DUMMY)
				{
					if(
						(__getParsingPhase(phaseStack) == PREPROCESS_WHILE && afterWhileBlock->getEndStatement() <= scope.top().endOfTheScope) ||
						(__getParsingPhase(phaseStack) == PREPROCESS_THEN && afterWhileBlock->getEndStatement() <= scope.top().midOfTheScope) ||
						(__getParsingPhase(phaseStack) == PREPROCESS_ELSE) ||
						(__getParsingPhase(phaseStack) == PREPROCESS_NORMAL_BLOCK)
						)
					{	
						__smartConnectThisCFGToNext(afterWhileBlock, whileNode);
					}
					else if(__getParsingPhase(phaseStack) == PREPROCESS_THEN)
					{
						CFGNode* outsideBlock = cfg->getCFGNodeByStmtNumber(scope.top().endOfTheScope + 1);
						whileBlockEnd->disconnect(outsideBlock);
						whileBlockEnd->denyNode(outsideBlock);
						__smartConnectThisCFGToNext(outsideBlock, whileNode);
					}
				}
			}
			phaseStack.push(PREPROCESS_WHILE);
			scope.push(*it);
		}
		else if (it->type == STMT_ASSIGNMENT || it->type == STMT_CALL)
		{
			//Connect this to next
			CFGNode* thisNode = cfg->getCFGNodeByStmtNumber(it->stmtNumber);

			if(!bipEnabled || it->type == STMT_ASSIGNMENT)
			{
				CFGNode* nextNode;
				nextNode = cfg->getCFGNodeByStmtNumber(it->stmtNumber + 1);

				if(nextNode == NULL && !bipEnabled) continue;
				if(nextNode == NULL && bipEnabled)
					nextNode = cfg->getBipDummyNodeByProcIndex(it->procIndex);
				if(bipEnabled && nextNode->getProcIndex() != it->procIndex)
					nextNode = cfg->getBipDummyNodeByProcIndex(it->procIndex);

				__connectAssignmentAndCall(phaseStack, it, scope, nextNode, bipEnabled, thisNode, cfg);
			}
			else
			{
				//if CFG BipEnabled, should get thisNode's literal next statement. If result is empty, then
				//get the dummy node of current proc index.
				vector<CFGNode*> nextNodes;
				nextNodes = cfg->getNextCFGNodeByCurrentStatement(*it, bipEnabled);
				printf("CFGBip: Size: %d\n",nextNodes.size());
				for(vector<CFGNode*>::iterator t = nextNodes.begin(); t != nextNodes.end(); ++t)
				{
					printf("CFGBip: Current: %d, Next: %d.\n", it->stmtNumber, (*t)->getStartStatement());
					__connectAssignmentAndCall(phaseStack, it, scope, *t, bipEnabled, thisNode, cfg);
				}

			}
			
			

		}
	}
}
void DesignExtractor::__smartConnectThisCFGToNext( CFGNode* nextNode, CFGNode* thisNode ) 
{
	if(nextNode != NULL)
	{
		if(nextNode != thisNode && nextNode->getProcIndex() == thisNode->getProcIndex())
		{
			thisNode->connectTo(nextNode);
			if(thisNode->getPairedCFGNode()!=NULL)
				thisNode->getPairedCFGNode()->connectTo(nextNode);
		}
	}
}

IfPreprocessingPhase DesignExtractor::__getParsingPhase( stack<IfPreprocessingPhase> &s )
{
	if(s.size()>0)
		return s.top();
	else
		return PREPROCESS_NORMAL_BLOCK;
}

BOOLEAN DesignExtractor::isAffects(int first, int second)
{
	STMT_LIST* firstStmt = new STMT_LIST;
	STMT_LIST* secondStmt = new STMT_LIST;
	STMT_LIST* temp = new STMT_LIST;
	STMT_LIST* nextStmts = new STMT_LIST;
	VAR_INDEX_LIST* var = new VAR_INDEX_LIST;
	vector<CFGNode*> cfg = _pkb->getCFG()->getAllCFGNodes();
	CFGNode* firstNode = _pkb->getCFG()->getCFGNodeByStmtNumber(first);
	CFGNode* secondNode = _pkb->getCFG()->getCFGNodeByStmtNumber(second);
	CFGNode* current;
	CFGNode* nextNode;
	vector<int> visitedNode;
	queue<int>	rest;
	vector<int> route;
	int nodeIndex, noModifiesInMiddle;

	firstStmt->push_back(first);
	secondStmt->push_back(second);
	if(!_pkb->nextStar(firstStmt, secondStmt, 0)) return false;
	if(!(_pkb->modifies(firstStmt, var, 1)&& _pkb->uses(secondStmt, var, 0))) return false;

	//rest.push(indexOf(cfg, firstNode));
	//visitedNode.push_back(indexOf(cfg, firstNode));

	//if at same node. special case
	if(firstNode == secondNode){
		if(first < second){
			for(int i = first+1; i < second; i++){
				temp->clear();
				temp->push_back(i);
				if (_pkb->modifies(temp, var, 0)&&(getTypeOfStmt(i) == STMT_ASSIGNMENT||getTypeOfStmt(i) == STMT_CALL)) return false;
				
			}
			return true;
		}else{
			for (int i = first+1; i <= firstNode->getEndStatement(); i++)
			{
				temp->clear();
				temp->push_back(i);
				if (_pkb->modifies(temp, var, 0)&&(getTypeOfStmt(i) == STMT_ASSIGNMENT||getTypeOfStmt(i) == STMT_CALL)) return false;
				
			}
			for (int i = firstNode->getStartStatement(); i < second; i++)
			{
				temp->clear();
				temp->push_back(i);
				if (_pkb->modifies(temp, var, 0)&&(getTypeOfStmt(i) == STMT_ASSIGNMENT||getTypeOfStmt(i) == STMT_CALL)) return false;
				
			}
		}
	}

	for (int i = first+1; i <= firstNode->getEndStatement(); i++)
	{
		temp->clear();
		temp->push_back(i);
		if(_pkb->modifies(temp, var, 0)&&(getTypeOfStmt(i) == STMT_ASSIGNMENT||getTypeOfStmt(i) == STMT_CALL)) return false;
	}
	nextStmts->clear();
	temp->clear();
	current = firstNode;
	temp->push_back(current->getEndStatement());
	_pkb->next(temp,nextStmts,1);
	for (int i = 0; i < nextStmts->size(); i++)
	{
		rest.push(indexOf(cfg, _pkb->getCFG()->getCFGNodeByStmtNumber(nextStmts->at(i))));	
	}

	while(rest.size()!=0){
		nodeIndex = rest.front();
		rest.pop();
		visitedNode.push_back(nodeIndex);
		current =cfg.at(nodeIndex);
		if (second > current->getStartStatement() && second < current->getEndStatement())
		{
			noModifiesInMiddle = 1;
			for (int i = current->getStartStatement(); i < second; i++)
			{
				temp->clear();
				temp->push_back(i);
				if (_pkb->modifies(temp, var, 0)&&(getTypeOfStmt(i) == STMT_ASSIGNMENT||getTypeOfStmt(i) == STMT_CALL))
				{
					noModifiesInMiddle = 0;
					break;
				}
			}
			if(noModifiesInMiddle) return true;
		}
		else
		{
			noModifiesInMiddle = 1;
			for (int i = current->getStartStatement(); i <= current->getEndStatement(); i++)
			{
				temp->clear();
				temp->push_back(i);
				if(_pkb->modifies(temp, var,0)&&(getTypeOfStmt(i) == STMT_ASSIGNMENT||getTypeOfStmt(i) == STMT_CALL))
				{
					noModifiesInMiddle = 0;
					break;
				}
			}
			if (noModifiesInMiddle == 1)
			{
				temp->clear();
				nextStmts->clear();
				temp->push_back(current->getEndStatement());
				_pkb->next(temp, nextStmts, 1);
				for (int i = 0; i < nextStmts->size(); i++)
				{
					nextNode = _pkb->getCFG()->getCFGNodeByStmtNumber(nextStmts->at(i));
					if(indexOf(visitedNode,indexOf(cfg,nextNode)) < 0)
						rest.push(indexOf(cfg, nextNode));
				}
			}
		}
	}
	return false;
}

STMT_LIST DesignExtractor::getAffectsFirst(STMT stmt2, BOOLEAN exhaustive)
{ 
	//if exhaustive is true, the function finds all assignments that affect stmt2.
	//if exhaustive is false, the function stops once one assignment is found.

	vector<CFGNode*> visitedNodes;
	STMT_LIST stmt2s;
	stmt2s.push_back(stmt2);
	VAR_INDEX_LIST usedVars;
	_pkb->uses(&stmt2s, &usedVars, 1); //get variables used by stmt2

	CFGNode* endNode = _pkb->getCFG()->getCFGNodeByStmtNumber(stmt2); //get the CFGNode that contains stmt2
	STMT start = endNode->getStartStatement(); //the start statement of this CFGNode

	STMT_LIST results; //global result list

	for(int i=0; i<usedVars.size(); i++) { //for each variable used by stmt2, find all statements that Affects this var
		VAR_INDEX var = usedVars.at(i); //choose one variable used by stmt2
		VAR_INDEX_LIST vars; vars.push_back(var); //need a list to query Modifies relation
		bool carryOn = true; //indicate whether to proceed to previous CFGNode
		
		for(int k=stmt2-1; k>=start; k--) { //statements 'start' to 'stmt2-1' are either assign or call
			STMT_LIST temp1; 
			temp1.push_back(k);
			if(_pkb->modifies(&temp1, &vars, 0)) { //means statement k modifies variable var
				if(getTypeOfStmt(k) == STMT_ASSIGNMENT) {
					results.push_back(k);
					if(!exhaustive) {
						return results;
					}	
				}
				carryOn = false;
				break;
			}
		}

		if(carryOn) { //should proceed to previous CFGNode
			getAffectsFirstHelper(endNode, &visitedNodes, vars, exhaustive, &results);	
			if(!exhaustive && results.size()>0)
				return results;
			visitedNodes.clear();
		}
	}
 
	return results;
}

void DesignExtractor::getAffectsFirstHelper(CFGNode* node2, vector<CFGNode*>* visitedNodes,
	VAR_INDEX_LIST usedVar, BOOLEAN exhaustive, STMT_LIST* resultLst_p)
{
	vector<CFGNode*> prev_nodes;
	prev_nodes = node2->getPrevEdges();
	
	if (prev_nodes.size() == 0) { // no more predecessor, base case, stop and return
		return;
	}else {
		int prev_nodes_size = prev_nodes.size();
		
		for (int i = 0; i < prev_nodes_size; i++) { // loop through each previous CFGNode
			CFGNode* currentNode = prev_nodes.at(i);
			if(indexOf((*visitedNodes), currentNode) >= 0){
				//current predecessor has been visited, do not visit it again
			} else {
				visitedNodes->push_back(currentNode); //mark current predecessor as visited to avoid re-visit

				bool carryOn = true; //indicate whether to proceed to previous CFGNode

				if(currentNode->getCFGType() == CFG_IF_STATEMENT || currentNode->getCFGType() == CFG_WHILE_STATEMENT) {
					//this node is if or while, go to previous CFGNode directly
				} else {
					int end1 = currentNode->getEndStatement();
					int start1 = currentNode->getStartStatement();
					for(int k=end1; k>=start1; k--) {
						STMT_LIST temp1; 
						temp1.push_back(k);
						if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
							if(getTypeOfStmt(k) == STMT_ASSIGNMENT) {
								resultLst_p->push_back(k);
								if(!exhaustive)
									return;
							}
							carryOn = false;
							break;
						}
					}
				}

				if(carryOn) { //should proceed to previous CFGNode
					getAffectsFirstHelper(currentNode, visitedNodes, usedVar, exhaustive, resultLst_p);		
					if(!exhaustive && resultLst_p->size()>0)
						return;
				}
			}
		}
		return;
	}
}

STMT_LIST DesignExtractor::getAffectsSecond(STMT stmt1, BOOLEAN exhaustive)
{
	//if exhaustive is true, the function finds all assignments affected by stmt1.
	//if exhaustive is false, the function stops once one assignment is found.

	vector<CFGNode*> visitedNodes;
	STMT_LIST stmt1s;
	stmt1s.push_back(stmt1);
	VAR_INDEX_LIST modifiedVar;
	_pkb->modifies(&stmt1s, &modifiedVar, 1); //get the variable modified by stmt1

	CFGNode* startNode = _pkb->getCFG()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1
	STMT end = startNode->getEndStatement(); //the end statement of this CFGNode

	STMT_LIST results;

	//one and only one variable can be modified by an assign statement, so size of modifiedVar must be 1
	//only need to check Affects for this single variable 'var' inside modifiedVar

	bool carryOn = true; //indicate whether to proceed to next CFGNode
		
	for(int k=stmt1+1; k<=end; k++) { //statements 'stmt+1' to 'end' are either assign or call
		STMT_LIST temp1; 
		temp1.push_back(k);

		if(getTypeOfStmt(k) == STMT_ASSIGNMENT && _pkb->uses(&temp1, &modifiedVar, 0) ) {
			results.push_back(k);
			if(!exhaustive) {
				return results;
			}	
		}
		if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken, should not move to nextNode
			carryOn = false;
			break;
		}
	}

	if(carryOn) { //should proceed to next CFGNode
		getAffectsSecondHelper(startNode, &visitedNodes, modifiedVar, exhaustive, &results);
		if(!exhaustive && results.size()>0)
			return results;
	}
 
	return results;
}

void DesignExtractor::getAffectsSecondHelper(CFGNode* node1, vector<CFGNode*>* visitedNodes,
	VAR_INDEX_LIST modifiedVar, BOOLEAN exhaustive, STMT_LIST* resultLst_p)
{
	vector<CFGNode*> next_nodes;
	next_nodes = node1->getNextEdges();
	
	if (next_nodes.size() == 0) { // no more successor, base case, stop and return
		return;
	}else {
		int next_nodes_size = next_nodes.size();
		
		for (int i = 0; i < next_nodes_size; i++) { // loop through each next CFGNode
			CFGNode* currentNode = next_nodes.at(i);
			if(indexOf((*visitedNodes), currentNode) >= 0){
				//current successor has been visited, do not visit it again
			} else {
				visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit

				bool carryOn = true; //indicate whether to proceed to next CFGNode

				if(currentNode->getCFGType() == CFG_IF_STATEMENT || currentNode->getCFGType() == CFG_WHILE_STATEMENT) {
					//this node is if or while, go to next CFGNode directly
				} else {
					int start1 = currentNode->getStartStatement();
					int end1 = currentNode->getEndStatement();
					for(int k=start1; k<=end1; k++) {
						STMT_LIST temp1; 
						temp1.push_back(k);

						if(getTypeOfStmt(k) == STMT_ASSIGNMENT && _pkb->uses(&temp1, &modifiedVar, 0) ) {
							resultLst_p->push_back(k);
							if(!exhaustive)
								return;
						}
						if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken, should not move to nextNode
							carryOn = false;
							break;
						}
					}
				}

				if(carryOn) { //should proceed to next CFGNode
					getAffectsSecondHelper(currentNode, visitedNodes, modifiedVar, exhaustive, resultLst_p);
					if(!exhaustive && resultLst_p->size()>0)
						return;
				}
			}
		}
		return;
	}
}

BOOLEAN DesignExtractor::affects00( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 1a
		STMT_LIST allAssign;
		getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(allAssign.at(i), false); //exhaustive is false, stops once one assign found
			if(affectedAssigns.size()>0) //one pair of a1, a2 satisfy Affects(a1,a2)
				return true;
		}
		return false;
	} else if(size1==0 && size2>0) { //case 1b
		for(int i=0; i<size2; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(st2s_p->at(i), false);
			if(affectAssigns.size()>0)
				return true;
		}
		return false;
	} else if(size1>0 && size2==0) { //case 1c
		for(int i=0; i<size1; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(st1s_p->at(i), false);
			if(affectedAssigns.size()>0)
				return true;
		}
		return false;
	} else { //case 1d
		for (int i = 0; i < size1; i++)
		{
			for (int j = 0; j < size2; j++)
			{	
				if(isAffects(st1s_p->at(i),st2s_p->at(j))) return true;
			}
		}
		return false;
	}
}

BOOLEAN DesignExtractor::affects01( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 2a
		STMT_LIST allAssign;
		getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectAssigns.size()>0)
				st2s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(st2s_p->at(index), false);
			if(affectAssigns.size()==0) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	} else if(size1>0 && size2==0) { //case 2b
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(st1s_p->at(i), true);
			for (int j = 0; j < affectedAssigns.size(); j++) {
				st2s_p->push_back(affectedAssigns.at(j));
			}
		}
	} else { //case 2d
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isAffects(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN DesignExtractor::affects10( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 3a
		STMT_LIST allAssign;
		getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectedAssigns.size()>0)
				st1s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 3b
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(st2s_p->at(i), true);
			for (int j = 0; j < affectAssigns.size(); j++) {
				st1s_p->push_back(affectAssigns.at(j));
			}
		}	
	} else if(size1>0 && size2==0) { //case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(st1s_p->at(index), false);
			if(affectedAssigns.size()==0) {
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
				if (isAffects(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN DesignExtractor::affects11( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		STMT_LIST allAssigns;
		getAllAssignment(&allAssigns);
		for(int i=0; i<allAssigns.size(); i++) {
			int current = allAssigns.at(i);
			STMT_LIST affectedAssigns = getAffectsSecond(current, true);
			for(int j=0; j<affectedAssigns.size(); j++) {
				st1s_p->push_back(current);
				st2s_p->push_back(affectedAssigns.at(j));
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
			vector<int> temp = getAffectsFirst(currentEleSt2s,true);
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
			vector<int> temp = getAffectsSecond(currentEleSt1s, true);
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
				if (isAffects(st1s_p->at(index), st2s_p->at(index))) {
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

BOOLEAN DesignExtractor::affects( STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg)
	{
	case 0:
		return affects00(st1s_ptr, st2s_ptr);
	case 1:
		return affects01(st1s_ptr, st2s_ptr);
	case 2:
		return affects10(st1s_ptr, st2s_ptr);
	case 3:
		return affects11(st1s_ptr, st2s_ptr);
	default:
		return false;
	}
}

BOOLEAN DesignExtractor::isAffectsStar(STMT stmt1, STMT stmt2)
{
	STMT_LIST visited; //the visited stmt list
	BOOLEAN result = false; //result initialised to false

	isAffectsStarHelper(stmt1, stmt2, &visited, &result);
	return result;
}

void DesignExtractor::isAffectsStarHelper(STMT stmt1, STMT stmt2, STMT_LIST* visited_p, BOOLEAN* result_p)
{
	STMT_LIST directAffectedLst = getAffectsSecond(stmt1, true);

	if (directAffectedLst.size() == 0) { // no more successor, base case, stop and return
		return;
	}else {
		int directAffectedLstSize = directAffectedLst.size();
		for (int i = 0; i < directAffectedLstSize; i++) { // loop through each direct successor
			int currentSecond = directAffectedLst.at(i);
			if(indexOf((*visited_p), currentSecond) >= 0){
				//current successor has been visited, do not visit it again
			} else {
				visited_p->push_back(currentSecond); //add this successor into visited list
				if(currentSecond == stmt2) { //match found, set result to be true and stop
					(*result_p) = true;
					return;
				}
				isAffectsStarHelper(currentSecond,stmt2, visited_p, result_p); // let this successor check all its direct/indirect successors
				if((*result_p) == true)
					return;
			}
		}
	}
}

STMT_LIST DesignExtractor::getAffectsStarFirst(STMT stmt2, BOOLEAN exhaustive)
{
	STMT_LIST resultLst; //the result list as well as visited stmt list

	if(!exhaustive) { //stop once one result is found
		STMT_LIST directAffectsLst = getAffectsFirst(stmt2, false);
		int size = directAffectsLst.size();
		if(size == 0) { //no direct Affects result, which means no Affects* result
			return resultLst;
		} else { //Affects result found, which also satisfy Affects*, since search is non-exhaustive, add one result and return
			resultLst.push_back(directAffectsLst.at(0));
			return resultLst;
		}
	} else { //find all results
		getAffectsStarFirstHelper(stmt2, &resultLst);
		return resultLst;
	}
}

void DesignExtractor::getAffectsStarFirstHelper(STMT stmt2, STMT_LIST* resultLst_p)
{
	STMT_LIST directAffectLst = getAffectsFirst(stmt2, true);

	if (directAffectLst.size() == 0) { // no more predecessor, base case, stop and return
		return;
	}else {
		int directAffectLstSize = directAffectLst.size();
		for (int i = 0; i < directAffectLstSize; i++) { // loop through each direct predecessor
			int currentFirst = directAffectLst.at(i);
			if(indexOf((*resultLst_p), currentFirst) >= 0){
				//current predecessor is in the result list, which means it has been visited, do not visit it again
			} else {
				resultLst_p->push_back(currentFirst); //add this predecessor into result list
				getAffectsStarFirstHelper(currentFirst, resultLst_p); // let this predecessor find all its direct/indirect predecessors
			}
		}
	}
}

STMT_LIST DesignExtractor::getAffectsStarSecond(STMT stmt1, BOOLEAN exhaustive)
{
	STMT_LIST resultLst; //the result list as well as visited stmt list

	if(!exhaustive) { //stop once one result is found
		STMT_LIST directAffectedLst = getAffectsSecond(stmt1, false);
		int size = directAffectedLst.size();
		if(size == 0) { //no direct Affects result, which means no Affects* result
			return resultLst;
		} else { //Affects result found, which also satisfy Affects*, since search is non-exhaustive, add one result and return
			resultLst.push_back(directAffectedLst.at(0));
			return resultLst;
		}
	} else { //find all results
		getAffectsStarSecondHelper(stmt1, &resultLst);
		return resultLst;
	}
}

void DesignExtractor::getAffectsStarSecondHelper(STMT stmt1, STMT_LIST* resultLst_p)
{
	STMT_LIST directAffectedLst = getAffectsSecond(stmt1, true);

	if (directAffectedLst.size() == 0) { // no more successor, base case, stop and return
		return;
	}else {
		int directAffectedLstSize = directAffectedLst.size();
		for (int i = 0; i < directAffectedLstSize; i++) { // loop through each direct successor
			int currentSecond = directAffectedLst.at(i);
			if(indexOf((*resultLst_p), currentSecond) >= 0){
				//current successor is in the result list, which means it has been visited, do not visit it again
			} else {
				resultLst_p->push_back(currentSecond); //add this successor into result list
				getAffectsStarSecondHelper(currentSecond, resultLst_p); // let this successor find all its direct/indirect successors
			}
		}
	}
}

BOOLEAN DesignExtractor::affectsStar( STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	switch (arg)
	{
	case 0:
		return affectsStar00(st1s_ptr, st2s_ptr);
	case 1:
		return affectsStar01(st1s_ptr, st2s_ptr);
	case 2:
		return affectsStar10(st1s_ptr, st2s_ptr);
	case 3:
		return affectsStar11(st1s_ptr, st2s_ptr);
	default:
		throw "arg is invalid.";
	}
}

BOOLEAN DesignExtractor::affectsStar00( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 1a
		STMT_LIST allAssign;
		getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(allAssign.at(i), false); //exhaustive is false, stops once one assign found
			if(affectedAssigns.size()>0) //one pair of a1, a2 satisfy Affects(a1,a2)
				return true;
		}
		return false;
	} else if(size1==0 && size2>0) { //case 1b
		for(int i=0; i<size2; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(st2s_p->at(i), false);
			if(affectAssigns.size()>0)
				return true;
		}
		return false;
	} else if(size1>0 && size2==0) { //case 1c
		for(int i=0; i<size1; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(st1s_p->at(i), false);
			if(affectedAssigns.size()>0)
				return true;
		}
		return false;
	} else { //case 1d
		for (int i = 0; i < size1; i++)
		{
			for (int j = 0; j < size2; j++)
			{	
				if(isAffectsStar(st1s_p->at(i),st2s_p->at(j))) return true;
			}
		}
		return false;
	}
}

BOOLEAN DesignExtractor::affectsStar01( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 2a
		STMT_LIST allAssign;
		getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectAssigns.size()>0)
				st2s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 2c
		int index = 0;
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectAssigns = getAffectsFirst(st2s_p->at(index), false);
			if(affectAssigns.size()==0) {
				st2s_p->erase(st2s_p->begin() + index);
				// index remain the same
			}else {
				// do not remove the element
				index++;
			}
		}
	} else if(size1>0 && size2==0) { //case 2b
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedAssigns = getAffectsStarSecond(st1s_p->at(i), true);
			for (int j = 0; j < affectedAssigns.size(); j++) {
				st2s_p->push_back(affectedAssigns.at(j));
			}
		}
	} else { //case 2d
		int index = 0;
		for (int i = 0; i < size2; i++) {
			BOOLEAN noMatch = true;
			for (int j = 0; j < size1; j++) {
				if (isAffectsStar(st1s_p->at(j), st2s_p->at(index))) {
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

BOOLEAN DesignExtractor::affectsStar10( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if(size1==0 && size2==0) { //case 3a
		STMT_LIST allAssign;
		getAllAssignment(&allAssign);
		int size = allAssign.size();
		for(int i=0; i<size; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(allAssign.at(i), false); //exhaustive is false, check for existence
			if(affectedAssigns.size()>0)
				st1s_p->push_back(allAssign.at(i));
		}
	} else if(size1==0 && size2>0) { //case 3b
		for (int i = 0; i < size2; i++) {
			STMT_LIST affectAssigns = getAffectsStarFirst(st2s_p->at(i), true);
			for (int j = 0; j < affectAssigns.size(); j++) {
				st1s_p->push_back(affectAssigns.at(j));
			}
		}	
	} else if(size1>0 && size2==0) { //case 3c
		int index = 0;
		for (int i = 0; i < size1; i++) {
			STMT_LIST affectedAssigns = getAffectsSecond(st1s_p->at(index), false);
			if(affectedAssigns.size()==0) {
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
				if (isAffectsStar(st1s_p->at(index), st2s_p->at(j))) {
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

BOOLEAN DesignExtractor::affectsStar11( STMT_LIST* st1s_p, STMT_LIST* st2s_p)
{
	int size1 = st1s_p->size();
	int size2 = st2s_p->size();

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		STMT_LIST allAssigns;
		getAllAssignment(&allAssigns);
		for(int i=0; i<allAssigns.size(); i++) {
			int current = allAssigns.at(i);
			STMT_LIST affectedAssigns = getAffectsStarSecond(current, true);
			for(int j=0; j<affectedAssigns.size(); j++) {
				st1s_p->push_back(current);
				st2s_p->push_back(affectedAssigns.at(j));
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
			vector<int> temp = getAffectsStarFirst(currentEleSt2s,true);
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
			vector<int> temp = getAffectsStarSecond(currentEleSt1s, true);
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
				if (isAffectsStar(st1s_p->at(index), st2s_p->at(index))) {
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

BOOLEAN DesignExtractor::isContains( int indexA, int indexB, ASTNodeType argA, ASTNodeType argB)
{
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	for (int i = 0; i < ast->size(); i++)
	{
		if (ast->at(i)->getNodeType() == argA && ast->at(i)->getNodeValue() == indexA)
		{
			a = ast->at(i);
			b = a->getChildren();
			while(b != NULL)
			{
				if (b->getNodeType() == argB && b->getNodeValue() == indexB)
					return true;
				b = b->getSibling();
			}
		}
	}
	
	return false;
}

STMT_LIST DesignExtractor::getContainsFirst( int indexB, ASTNodeType argA, ASTNodeType argB )
{
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	vector<int> result;
	ASTNode * a;
	ASTNode * b;
	for (int i = 0; i < ast->size(); i++)
	{
		if (ast->at(i)->getNodeType() == argB && ast->at(i)->getNodeValue() == indexB)
		{
			b = ast->at(i);
			break;
		}
	}
	a = b->getParentNode();
	if (b->getNodeType() == argB)
		result.push_back(b->getNodeValue());

	return result;
}

STMT_LIST DesignExtractor::getContainsSecond( int indexA, ASTNodeType argA, ASTNodeType argB )
{
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	vector<int> result;
	ASTNode * a;
	ASTNode * b;
	for (int i = 0; i < ast->size(); i++)
	{
		if (ast->at(i)->getNodeType() == argA && ast->at(i)->getNodeValue() == indexA)
		{
			a = ast->at(i);
			break;
		}
	}
	b = a->getChildren();
	while(b != NULL)
	{
		if (b->getNodeType() == argB)
			result.push_back(b->getNodeValue());
		b = b->getSibling();
	}
	return result;
}


BOOLEAN DesignExtractor::contains( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB, int arg )
{
	vector<int> temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	switch (arg) {
	case 0:
		if (argA == AST_STMT)
		{
			if (contains_00(indexAs,&temp1,AST_ASSIGNMENT, argB)) return true;
			if (contains_00(indexAs,&temp2,AST_WHILE_LOOP, argB)) return true;
			if (contains_00(indexAs,&temp3,AST_IF_BRANCH, argB)) return true;
			if (contains_00(indexAs,&temp4,AST_CALL, argB)) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			if (contains_00(&temp1,indexBs, argA, AST_ASSIGNMENT)) return true;
			if (contains_00(&temp2,indexBs, argA, AST_WHILE_LOOP)) return true;
			if (contains_00(&temp3,indexBs, argA, AST_IF_BRANCH)) return true;
			if (contains_00(&temp4,indexBs, argA, AST_CALL)) return true;
			return false;
		}
		return(contains_00(indexAs, indexBs, argA, argB));

	case 1:
		if (argA == AST_STMT)
		{
			contains_01(indexAs,&temp1,AST_ASSIGNMENT, argB);
			contains_01(indexAs,&temp2,AST_WHILE_LOOP, argB);
			contains_01(indexAs,&temp3,AST_IF_BRANCH, argB);
			contains_01(indexAs,&temp4,AST_CALL, argB);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexBs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexBs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexBs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexBs->push_back(temp4.at(i));
			}
			if (indexBs->size() > 0) return true;
				return false;
		}
		if (argB == AST_STMT)
		{
			contains_01(indexAs,&temp1, argA, AST_ASSIGNMENT);
			contains_01(indexAs,&temp2, argA, AST_WHILE_LOOP);
			contains_01(indexAs,&temp3, argA, AST_IF_BRANCH);
			contains_01(indexAs,&temp4, argA, AST_CALL);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexBs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexBs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexBs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexBs->push_back(temp4.at(i));
			}
			if (indexBs->size() > 0) return true;
				return false;
		}
		return(contains_01(indexAs, indexBs, argA, argB));

	case 2:
		if (argA == AST_STMT)
		{
			contains_10(&temp1,indexBs,AST_ASSIGNMENT, argB);
			contains_10(&temp2,indexBs,AST_WHILE_LOOP, argB);
			contains_10(&temp3,indexBs,AST_IF_BRANCH, argB);
			contains_10(&temp4,indexBs,AST_CALL, argB);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexAs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexAs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexAs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexAs->push_back(temp4.at(i));
			}
			if (indexAs->size() > 0) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			contains_10(&temp1,indexBs, argA, AST_ASSIGNMENT);
			contains_10(&temp2,indexBs, argA, AST_WHILE_LOOP);
			contains_10(&temp3,indexBs, argA, AST_IF_BRANCH);
			contains_10(&temp4,indexBs, argA, AST_CALL);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexAs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexAs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexAs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexAs->push_back(temp4.at(i));
			}
			if (indexAs->size() > 0) return true;
			return false;
		}
		return(contains_10(indexAs, indexBs, argA, argB));

	case 3:
		if (argA == AST_STMT)
		{
			contains_11(&temp5,&temp1,AST_ASSIGNMENT, argB);
			contains_11(&temp6,&temp2,AST_WHILE_LOOP, argB);
			contains_11(&temp7,&temp3,AST_IF_BRANCH, argB);
			contains_11(&temp8,&temp4,AST_CALL, argB);
			for (int i = 0; i < temp5.size(); i++)
			{
				indexAs->push_back(temp5.at(i));
			}
			for (int i = 0; i < temp6.size(); i++)
			{
				indexAs->push_back(temp6.at(i));
			}
			for (int i = 0; i < temp7.size(); i++)
			{
				indexAs->push_back(temp7.at(i));
			}
			for (int i = 0; i < temp8.size(); i++)
			{
				indexAs->push_back(temp8.at(i));
			}
			for (int i = 0; i < temp1.size(); i++)
			{
				indexBs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexBs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexBs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexBs->push_back(temp4.at(i));
			}
			if (indexBs->size() > 0) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			contains_11(&temp1,&temp5, argA, AST_ASSIGNMENT);
			contains_11(&temp2,&temp6, argA, AST_WHILE_LOOP);
			contains_11(&temp3,&temp7, argA, AST_IF_BRANCH);
			contains_11(&temp4,&temp8, argA, AST_CALL);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexAs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexAs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexAs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexAs->push_back(temp4.at(i));
			}
			for (int i = 0; i < temp5.size(); i++)
			{
				indexBs->push_back(temp5.at(i));
			}
			for (int i = 0; i < temp6.size(); i++)
			{
				indexBs->push_back(temp6.at(i));
			}
			for (int i = 0; i < temp7.size(); i++)
			{
				indexBs->push_back(temp7.at(i));
			}
			for (int i = 0; i < temp8.size(); i++)
			{
				indexBs->push_back(temp8.at(i));
			}
			if (indexAs->size() > 0) return true;
			return false;
		}
		return(contains_11(indexAs, indexBs, argA, argB));

	default:
		return false;
	}
}


BOOLEAN DesignExtractor::contains_00(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB)
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	if ((size1 == 0) && (size2 == 0)) { //case 1a
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >= 0)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs, ast->at(i)->getNodeValue()) >=0)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs, ast->at(i)->getNodeValue())>=0)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >= 0)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}
	return false;
}


BOOLEAN DesignExtractor::contains_01(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB)
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(temp, b->getNodeValue()) >= 0 )
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b, st1s_p has no duplicate
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs,ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
		
	}else {  //size1!=0 && size2!=0, case 2d
		 /*
		  * VectorA (a1, a2, ..., am) VectorB (b1, b2, ..., bn)
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
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs, ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(temp, b->getNodeValue()) >= 0)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}
}


BOOLEAN DesignExtractor::contains_10(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB)
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b, st2s_p has no duplicate
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >=0 )
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(temp, ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
		 /*
		  * VectorA (a1, a2, ..., am) VectorB (b1, b2, ..., bn)
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
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(temp, ast->at(i)->getNodeValue()))
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >= 0)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}

	if (indexAs->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}


BOOLEAN DesignExtractor::contains_11(vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB)
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;
	int j = 0;

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(temp, b->getNodeValue()) >= 0)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(temp, ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		j = 0;
		while (j < indexAs->size())
		{
			if (!isContains(indexAs->at(j), indexBs->at(j), argA, argB))
			{
				indexAs->erase(indexAs->begin()+j);
				indexBs->erase(indexBs->begin()+j);
			}
			else
			{
				j ++;
			}
		}
	}
	if (indexAs->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

BOOLEAN DesignExtractor::isSibling( int indexA, int indexB, ASTNodeType argA, ASTNodeType argB )
{
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	ASTNode * parent;
	for (int i = 0; i < ast->size(); i++)
	{
		if (ast->at(i)->getNodeType() == argA && ast->at(i)->getNodeValue() == indexA)
		{
			a = ast->at(i);
			parent = a->getParentNode();
			b = parent->getChildren();
			while(b != NULL)
			{
				if (b->getNodeType() == argB && b->getNodeValue() == indexB && indexOf(*ast, b) != i)
					return true;
				b = b->getSibling();
			}
		}
	}

	return false;
}

BOOLEAN DesignExtractor::sibling( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB, int arg )
{
	switch (arg) {
	case 0:
		return(sibling_00(indexAs, indexBs, argA, argB));

	case 1:
		return(sibling_01(indexAs, indexBs, argA, argB));

	case 2:
		return(sibling_10(indexAs, indexBs, argA, argB));

	case 3:
		return(sibling_11(indexAs, indexBs, argA, argB));

	default:
		return false;
	}
}

BOOLEAN DesignExtractor::sibling_00( vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * parent;
	ASTNode * a;
	ASTNode * b;
	if ((size1 == 0) && (size2 == 0)) { //case 1a
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				parent = a->getParentNode();
				b =parent->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b) != i)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >= 0 && indexOf(*ast, b) != i)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(*indexAs, a->getNodeValue()) >=0)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b) != i)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(*indexAs, a->getNodeValue())>=0)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >= 0 && indexOf(*ast, b) != i)
					{
						return true;
					}
					b = b->getSibling();
				}
			}
		}
		return false;
	}
	return false;
}

BOOLEAN DesignExtractor::sibling_01( vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b)!=i)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(temp, b->getNodeValue()) >= 0 && indexOf(*ast, b)!=i)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b, st1s_p has no duplicate
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(*indexAs,a->getNodeValue()) >= 0)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b)!=i)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
		
	}else {  //size1!=0 && size2!=0, case 2d
		 /*
		  * VectorA (a1, a2, ..., am) VectorB (b1, b2, ..., bn)
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
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(*indexAs, a->getNodeValue()) >= 0)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(temp, b->getNodeValue()) >= 0 && indexOf(*ast, b)!=i)
					{
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}
}

BOOLEAN DesignExtractor::sibling_10( vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;

	if ((size1 == 0) && (size2 == 0)) { //case 3a
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b)!=i)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 3b, st2s_p has no duplicate
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >=0 && indexOf(*ast, b)!=0)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 3c
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(temp, a->getNodeValue()) >= 0)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b)!=i)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else {  //size1!=0 && size2!=0, case 3d
		 /*
		  * VectorA (a1, a2, ..., am) VectorB (b1, b2, ..., bn)
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
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(temp, a->getNodeValue()))
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*indexBs, b->getNodeValue()) >= 0 && indexOf(*ast, b)!=0)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}

	if (indexAs->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

BOOLEAN DesignExtractor::sibling_11( vector<int>* indexAs, vector<int> *indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;
	int j = 0;

	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b)!=i)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(temp, b->getNodeValue()) >= 0 && indexOf(*ast, b)!=i)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			a = ast->at(i);
			if (a->getNodeType() == argA && indexOf(temp, a->getNodeValue()) >= 0)
			{
				b = a->getParentNode()->getChildren();
				while (b != NULL)
				{
					if (b->getNodeType() == argB && indexOf(*ast, b)!=i)
					{
						indexAs->push_back(ast->at(i)->getNodeValue());
						indexBs->push_back(b->getNodeValue());
					}
					b = b->getSibling();
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		j = 0;
		while (j < indexAs->size())
		{
			if (!isSibling(indexAs->at(j), indexBs->at(j), argA, argB))
			{
				indexAs->erase(indexAs->begin()+j);
				indexBs->erase(indexBs->begin()+j);
			}
			else
			{
				j ++;
			}
		}
	}
	if (indexAs->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

BOOLEAN DesignExtractor::containsStar( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB, int arg )
{
	vector<int> temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	switch (arg) {
	case 0:
		if (argA == AST_STMT)
		{
			if(containsStar_00(indexAs,&temp1,AST_ASSIGNMENT, argB)) return true;
			if(containsStar_00(indexAs,&temp2,AST_WHILE_LOOP, argB)) return true;
			if(containsStar_00(indexAs,&temp3,AST_IF_BRANCH, argB)) return true;
			if(containsStar_00(indexAs,&temp4,AST_CALL, argB)) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			if(containsStar_00(&temp1,indexBs, argA, AST_ASSIGNMENT)) return true;
			if(containsStar_00(&temp2,indexBs, argA, AST_WHILE_LOOP)) return true;
			if(containsStar_00(&temp3,indexBs, argA, AST_IF_BRANCH)) return true;
			if(containsStar_00(&temp4,indexBs, argA, AST_CALL)) return true;
			return false;
		}
		return(containsStar_00(indexAs, indexBs, argA, argB));

	case 1:
		if (argA == AST_STMT)
		{
			containsStar_01(indexAs,&temp1,AST_ASSIGNMENT, argB);
			containsStar_01(indexAs,&temp2,AST_WHILE_LOOP, argB);
			containsStar_01(indexAs,&temp3,AST_IF_BRANCH, argB);
			containsStar_01(indexAs,&temp4,AST_CALL, argB);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexBs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexBs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexBs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexBs->push_back(temp4.at(i));
			}
			if (indexBs->size() > 0) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			containsStar_01(indexAs,&temp1, argA, AST_ASSIGNMENT);
			containsStar_01(indexAs,&temp2, argA, AST_WHILE_LOOP);
			containsStar_01(indexAs,&temp3, argA, AST_IF_BRANCH);
			containsStar_01(indexAs,&temp4, argA, AST_CALL);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexBs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexBs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexBs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexBs->push_back(temp4.at(i));
			}
			if (indexBs->size() > 0) return true;
			return false;
		}
		return(containsStar_01(indexAs, indexBs, argA, argB));

	case 2:
		if (argA == AST_STMT)
		{
			containsStar_10(&temp1,indexBs,AST_ASSIGNMENT, argB);
			containsStar_10(&temp2,indexBs,AST_WHILE_LOOP, argB);
			containsStar_10(&temp3,indexBs,AST_IF_BRANCH, argB);
			containsStar_10(&temp4,indexBs,AST_CALL, argB);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexAs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexAs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexAs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexAs->push_back(temp4.at(i));
			}
			if (indexAs->size() > 0) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			containsStar_10(&temp1,indexBs, argA, AST_ASSIGNMENT);
			containsStar_10(&temp2,indexBs, argA, AST_WHILE_LOOP);
			containsStar_10(&temp3,indexBs, argA, AST_IF_BRANCH);
			containsStar_10(&temp4,indexBs, argA, AST_CALL);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexAs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexAs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexAs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexAs->push_back(temp4.at(i));
			}
			if (indexAs->size() > 0) return true;
			return false;
		}
		return(containsStar_10(indexAs, indexBs, argA, argB));

	case 3:
		if (argA == AST_STMT)
		{
			containsStar_11(&temp5,&temp1,AST_ASSIGNMENT, argB);
			containsStar_11(&temp6,&temp2,AST_WHILE_LOOP, argB);
			containsStar_11(&temp7,&temp3,AST_IF_BRANCH, argB);
			containsStar_11(&temp8,&temp4,AST_CALL, argB);
			for (int i = 0; i < temp5.size(); i++)
			{
				indexAs->push_back(temp5.at(i));
			}
			for (int i = 0; i < temp6.size(); i++)
			{
				indexAs->push_back(temp6.at(i));
			}
			for (int i = 0; i < temp7.size(); i++)
			{
				indexAs->push_back(temp7.at(i));
			}
			for (int i = 0; i < temp8.size(); i++)
			{
				indexAs->push_back(temp8.at(i));
			}
			for (int i = 0; i < temp1.size(); i++)
			{
				indexBs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexBs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexBs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexBs->push_back(temp4.at(i));
			}
			if (indexBs->size() > 0) return true;
			return false;
		}
		if (argB == AST_STMT)
		{
			containsStar_11(&temp1,&temp5, argA, AST_ASSIGNMENT);
			containsStar_11(&temp2,&temp6, argA, AST_WHILE_LOOP);
			containsStar_11(&temp3,&temp7, argA, AST_IF_BRANCH);
			containsStar_11(&temp4,&temp8, argA, AST_CALL);
			for (int i = 0; i < temp1.size(); i++)
			{
				indexAs->push_back(temp1.at(i));
			}
			for (int i = 0; i < temp2.size(); i++)
			{
				indexAs->push_back(temp2.at(i));
			}
			for (int i = 0; i < temp3.size(); i++)
			{
				indexAs->push_back(temp3.at(i));
			}
			for (int i = 0; i < temp4.size(); i++)
			{
				indexAs->push_back(temp4.at(i));
			}
			for (int i = 0; i < temp5.size(); i++)
			{
				indexBs->push_back(temp5.at(i));
			}
			for (int i = 0; i < temp6.size(); i++)
			{
				indexBs->push_back(temp6.at(i));
			}
			for (int i = 0; i < temp7.size(); i++)
			{
				indexBs->push_back(temp7.at(i));
			}
			for (int i = 0; i < temp8.size(); i++)
			{
				indexBs->push_back(temp8.at(i));
			}
			if (indexAs->size() > 0) return true;
			return false;
		}
		return(containsStar_11(indexAs, indexBs, argA, argB));

	default:
		return false;
	}
}

BOOLEAN DesignExtractor::containsStar_00( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	bool flag;
	if ((size1 == 0) && (size2 == 0)) { //case 1a
		flag = false;
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1aRecursive(b, argB, &flag);
				if(flag == true) return true;
			}
		}
		return false;
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		flag = false;
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1bRecursive(b, argB, *indexBs, &flag);
				if(flag == true) return true;
			}
		}
		return false;
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		flag = false;
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs, ast->at(i)->getNodeValue()) >=0)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1aRecursive(b, argB, &flag);
				if(flag == true) return true;
			}
		}
		return false;
	}else {  //size1!=0 && size2!=0, case 1d
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs, ast->at(i)->getNodeValue())>=0)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1bRecursive(b, argB, *indexBs, &flag);
				if(flag == true) return true;
			}
		}
		return false;
	}
	return false;
}

BOOLEAN DesignExtractor::containsStar_01( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;

	if ((size1 == 0) && (size2 == 0)) { //case 2a
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				containsStarCase2aRecursive(b, argB, indexBs);
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 2c
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				containsStarCase2bRecursive(b, argB, temp, indexBs);
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 2b, st1s_p has no duplicate
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs,ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				containsStarCase2aRecursive(b, argB, indexBs);
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}else {  //size1!=0 && size2!=0, case 2d
		 /*
		  * VectorA (a1, a2, ..., am) VectorB (b1, b2, ..., bn)
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
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(*indexAs, ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				containsStarCase2bRecursive(b, argB, temp, indexBs);
			}
		}
		if (indexBs->size() > 0)
			return true;
		return false;
	}
}

BOOLEAN DesignExtractor::containsStar_10( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	bool flag;
	vector<int> temp;
	if ((size1 == 0) && (size2 == 0)) { //case 1a
		flag = false;
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1aRecursive(b, argB, &flag);
				if(flag == true) indexAs->push_back(ast->at(i)->getNodeValue());
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 1b
		flag = false;
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1bRecursive(b, argB, *indexBs, &flag);
				if(flag == true) indexAs->push_back(ast->at(i)->getNodeValue());
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 1c
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(temp, ast->at(i)->getNodeValue()) >=0)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1aRecursive(b, argB, &flag);
				if(flag == true) indexAs->push_back(ast->at(i)->getNodeValue());
			}
		}
	}else {  //size1!=0 && size2!=0, case 1d
		flag = false;
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			if (ast->at(i)->getNodeType() == argA && indexOf(temp, ast->at(i)->getNodeValue())>=0)
			{
				flag = false;
				b = ast->at(i)->getChildren();
				containsStarCase1bRecursive(b, argB, *indexBs, &flag);
				if(flag == true) indexAs->push_back(ast->at(i)->getNodeValue());
			}
		}
	}
	if (indexAs->size() > 0)
		return true;
	return false;
}

BOOLEAN DesignExtractor::containsStar_11( vector<int>* indexAs, vector<int>* indexBs, ASTNodeType argA, ASTNodeType argB )
{
	int size1 = indexAs->size();
	int size2 = indexBs->size();
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	ASTNode * a;
	ASTNode * b;
	vector<int> temp;
	int j = 0;
	int count;
	if ((size1 == 0) && (size2 == 0)) { //case 4a
		//throw "arg is 11, but both vectors are empty.";
		for (int i = 0; i < ast->size(); i++)
		{
			count = 0;
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				containsStarCase4aRecursive(b, argB, indexBs, &count);
				for(int n = 0; n < count; n++) 
				{
						indexAs->push_back(ast->at(i)->getNodeValue());
				}
			}
		}
	}else if (size1 == 0) {  //size1==0 && size2!=0, case 4b
		temp = *indexBs;
		indexBs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			count = 0;
			if (ast->at(i)->getNodeType() == argA)
			{
				b = ast->at(i)->getChildren();
				containsStarCase4bRecursive(b, argB, temp, indexBs, &count);
				for(int n = 0; n < count; n++) 
				{
					indexAs->push_back(ast->at(i)->getNodeValue());
				}
			}
		}
	}else if (size2 == 0) {  //size2==0 && size1 !=0, case 4c
		temp = *indexAs;
		indexAs->clear();
		for (int i = 0; i < ast->size(); i++)
		{
			count = 0;
			if (ast->at(i)->getNodeType() == argA && indexOf(temp, ast->at(i)->getNodeValue()) >= 0)
			{
				b = ast->at(i)->getChildren();
				containsStarCase4aRecursive(b, argB, indexBs, &count);
				for(int n = 0; n < count; n++) 
				{
					indexAs->push_back(ast->at(i)->getNodeValue());
				}
			}
		}
	}else {           //size1!=0 && size2!=0, case 4d
		j = 0;
		while (j < indexAs->size())
		{
			if (!isContainsStar(indexAs->at(j), indexBs->at(j), argA, argB))
			{
				indexAs->erase(indexAs->begin()+j);
				indexBs->erase(indexBs->begin()+j);
			}
			else
			{
				j ++;
			}
		}
	}
	if (indexAs->size() > 0) {
		return(true);
	}else {
		return(false);
	}
}

void DesignExtractor::containsStarCase1aRecursive( ASTNode* root, ASTNodeType type, BOOLEAN* result )
{
	ASTNode* sib, * child;
	if (root == NULL)
		return;
	if (root->getNodeType() == type) 
	{
			*result = true;
			return;
	}
	child = root->getChildren();
	if (child != NULL) containsStarCase1aRecursive(child, type, result);
	if (*result == true) 
		return;
	sib = root->getSibling();
	if (sib != NULL) containsStarCase1aRecursive(sib, type, result);
	if (*result == true)
		return;
}

void DesignExtractor::containsStarCase1bRecursive( ASTNode* root, ASTNodeType type, vector<int> val, BOOLEAN* result )
{
	ASTNode* sib, * child;
	if (root == NULL)
		return;
	if (root->getNodeType() == type && indexOf(val, root->getNodeValue()) >= 0) 
	{
		*result = true;
		return;
	}
	child = root->getChildren();
	if (child != NULL) containsStarCase1bRecursive(child, type, val, result);
	if (*result == true) 
		return;
	sib = root->getSibling();
	if (sib != NULL) containsStarCase1bRecursive(sib, type,val, result);
	if (*result == true)
		return;
}

BOOLEAN DesignExtractor::isContainsStar( int indexA, int indexB, ASTNodeType argA, ASTNodeType argB )
{
	vector<ASTNode*>* ast = _pkb->getAST()->getAllASTNodes();
	BOOLEAN flag = false;
	ASTNode* b;
	vector<int> temp;
	temp.push_back(indexB);
	for (int i = 0; i < ast->size(); i++)
	{
		if (ast->at(i)->getNodeType() == argA && ast->at(i)->getNodeValue() == indexA)
		{
			b = ast->at(i)->getChildren();
			containsStarCase1bRecursive(b, argB, temp, &flag);
			if(flag == true) return true;
		}
	}
	return false;
}

void DesignExtractor::containsStarCase2aRecursive( ASTNode* root, ASTNodeType type, vector<int>* indexs)
{
	ASTNode* sib, * child;
	if (root == NULL) return;
	if (root->getNodeType() == type) 
	{
		indexs->push_back(root->getNodeValue());
	}
	child = root->getChildren();
	if (child != NULL) containsStarCase2aRecursive(child, type, indexs);
	sib = root->getSibling();
	if (sib != NULL) containsStarCase2aRecursive(sib, type, indexs);
}

void DesignExtractor::containsStarCase2bRecursive( ASTNode* root, ASTNodeType type, vector<int> vals, vector<int>* indexs )
{
	ASTNode* sib, * child;
	if (root == NULL) return;
	if (root->getNodeType() == type && indexOf(vals, root->getNodeValue()) >= 0) 
	{
		indexs->push_back(root->getNodeValue());
	}
	child = root->getChildren();
	if (child != NULL) containsStarCase2bRecursive(child, type, vals, indexs);
	sib = root->getSibling();
	if (sib != NULL) containsStarCase2bRecursive(sib, type,vals, indexs);
}

void DesignExtractor::containsStarCase4aRecursive( ASTNode* root, ASTNodeType type, vector<int>* indexs, int* result )
{
	ASTNode* sib, * child;
	if (root == NULL) return;
	if (root->getNodeType() == type) 
	{
		*result++;
		indexs->push_back(root->getNodeValue());
	}
	child = root->getChildren();
	if (child != NULL) containsStarCase4aRecursive(child, type, indexs, result);
	sib = root->getSibling();
	if (sib != NULL) containsStarCase4aRecursive(sib, type, indexs, result);
}

void DesignExtractor::containsStarCase4bRecursive( ASTNode* root, ASTNodeType type, vector<int> vals, vector<int>* indexs, int* result )
{
	ASTNode* sib, * child;
	if (root == NULL) return;
	if (root->getNodeType() == type && indexOf(vals, root->getNodeValue()) >= 0) 
	{
		*result ++;
		indexs->push_back(root->getNodeValue());
	}
	child = root->getChildren();
	if (child != NULL) containsStarCase4bRecursive(child, type, vals,indexs, result);
	sib = root->getSibling();
	if (sib != NULL) containsStarCase4bRecursive(sib, type,vals,indexs, result);
}

void DesignExtractor::__connectAssignmentAndCall( stack<IfPreprocessingPhase> &phaseStack, vector<statement>::iterator &it, stack<statement> &scope, CFGNode* nextNode, bool bipEnabled, CFGNode* thisNode, CFG* cfg )
{
	if(bipEnabled && it->type == STMT_CALL)
	{
		assert(it->type == STMT_CALL);
		PROC_INDEX calleeIndex = _pkb->getProcIndex(it->extraName);
		STMT calleeStart = _pkb->getProcStart(calleeIndex);
		STMT calleeEnd = _pkb->getProcEnd(calleeIndex);
		CFGNode* calleeStartNode = cfg->getCFGNodeByStmtNumber(calleeStart);
		CFGNode* calleeEndNode = cfg->getFollowingCFGNodeByCurrentStmtNumber(calleeEnd);
		thisNode->connectTo(calleeStartNode);
		thisNode->setBipType(CFG_BIP_IN);
		if(nextNode->getProcIndex() == thisNode->getProcIndex())
		{
			if(calleeEndNode != NULL)
			calleeEndNode->connectTo(nextNode);
		}

		return ;
	}

	if(__getParsingPhase(phaseStack) == PREPROCESS_NORMAL_BLOCK || 
		(__getParsingPhase(phaseStack) == PREPROCESS_THEN && (it->stmtNumber < scope.top().midOfTheScope)) ||
		(__getParsingPhase(phaseStack) == PREPROCESS_WHILE && (it->stmtNumber < scope.top().endOfTheScope) && nextNode->getCFGType() != CFG_DUMMY) ||
		(__getParsingPhase(phaseStack) == PREPROCESS_ELSE && scope.size() <= 1))
	{
		if(!bipEnabled || it->type == STMT_ASSIGNMENT)
		{
			__smartConnectThisCFGToNext(nextNode, thisNode);
		}
	}
	else if(__getParsingPhase(phaseStack) == PREPROCESS_ELSE && scope.size() > 1)
	{
		//Get Grand_Parent Node
		statement parent = scope.top();
		scope.pop();
		statement grand_parent = scope.top();
		scope.push(parent);
		assert(grand_parent.type == STMT_IF || grand_parent.type == STMT_WHILE);

		//Get Grand_Parent State
		IfPreprocessingPhase parent_state = phaseStack.top();
		phaseStack.pop();
		IfPreprocessingPhase grand_parent_state = phaseStack.top();
		phaseStack.push(parent_state);

		if(grand_parent_state == PREPROCESS_THEN)
		{
			if(it->stmtNumber < grand_parent.midOfTheScope)
			{
				__smartConnectThisCFGToNext(nextNode, thisNode);
			}
		}
		else if(grand_parent_state == PREPROCESS_ELSE || grand_parent_state == PREPROCESS_WHILE)
		{
			if(it->stmtNumber < grand_parent.endOfTheScope)
			{
				__smartConnectThisCFGToNext(nextNode, thisNode);
			}
		}
		else
		{
			throw("Error while smart-connecting nested IF CFG.");
		}
	}
}

// The following codes are old codes, which might be used when new codes are wrong
/*
STMT_LIST DesignExtractor::getAffectsFirst(STMT stmt2, BOOLEAN exhaustive)
{ 
	//if exhaustive is true, the function finds all assignments that affect stmt2.
	//if exhaustive is false, the function stops once one assignment is found.

	vector<CFGNode*> visitedNodes;
	STMT_LIST stmt2s;
	stmt2s.push_back(stmt2);
	VAR_INDEX_LIST usedVars;
	_pkb->uses(&stmt2s, &usedVars, 1); //get variables used by stmt2

	CFGNode* endNode = _pkb->getCFG()->getCFGNodeByStmtNumber(stmt2); //get the CFGNode that contains stmt2
	STMT start = endNode->getStartStatement(); //the start statement of this CFGNode

	STMT_LIST result;

	for(int i=0; i<usedVars.size(); i++) { //for each variable used by stmt2, find all statements that Affects this var
		VAR_INDEX var = usedVars.at(i); //choose one variable used by stmt2
		VAR_INDEX_LIST vars; vars.push_back(var); //need a list to query Modifies relation
		bool carryOn = true; //indicate whether to proceed to previous CFGNode
		
		for(int k=stmt2-1; k>=start; k--) { //statements 'start' to 'stmt2-1' are either assign or call
			STMT_LIST temp1; 
			temp1.push_back(k);
			if(_pkb->modifies(&temp1, &vars, 0)) { //means statement k modifies variable var
				if(_pkb->getPreprocessedProgram()->at(k).type == STMT_ASSIGNMENT) {
					result.push_back(k);
					if(!exhaustive) {
						return result;
					}	
				}
				carryOn = false;
				break;
			}
		}

		if(carryOn) { //should proceed to previous CFGNode
			STMT_LIST temp = getAffectsFirstHelper(endNode, &visitedNodes, vars, exhaustive);
			for(int j=0; j<temp.size(); j++) { //combine results for all variables
				result.push_back(temp.at(j));
				if(!exhaustive)
					return result;
			}
			visitedNodes.clear();
		}
	}
 
	return result;
}

STMT_LIST DesignExtractor::getAffectsFirstHelper(CFGNode* node2, vector<CFGNode*>* visitedNodes, VAR_INDEX_LIST usedVar, BOOLEAN exhaustive)
{
	STMT_LIST resultLst;
	vector<CFGNode*> prev_nodes;
	//STMT_LIST prev_stmts;
	//STMT_LIST stmt2s; 
	//stmt2s.push_back(node2->getStartStatement());

	//_pkb->next(&prev_stmts, &stmt2s, 2);
	prev_nodes = node2->getPrevEdges();
	
	if (prev_nodes.size() == 0) { // no more predecessor, base case, stop and return
		return(resultLst);
	}else {
		int prev_nodes_size = prev_nodes.size();
		//for(int i=0; i<prev_nodes_size; i++) {
			//prev_nodes.push_back(_pkb->getCFG()->getCFGNodeByStmtNumber(prev_stmts.at(i)));
		//}
		for (int i = 0; i < prev_nodes_size; i++) { // loop through each previous CFGNode
			CFGNode* currentNode = prev_nodes.at(i);
			if(indexOf((*visitedNodes), currentNode) >= 0){
				//current predecessor has been visited, do not visit it again
			} else {
				visitedNodes->push_back(currentNode); //mark current predecessor as visited to avoid re-visit

				bool carryOn = true; //indicate whether to proceed to previous CFGNode

				if(currentNode->getCFGType() == CFG_IF_STATEMENT || currentNode->getCFGType() == CFG_WHILE_STATEMENT) {
					//this node is if or while, go to previous CFGNode directly
				} else {
					int end1 = currentNode->getEndStatement();
					int start1 = currentNode->getStartStatement();
					for(int k=end1; k>=start1; k--) {
						STMT_LIST temp1; 
						temp1.push_back(k);
						if(_pkb->modifies(&temp1, &usedVar, 0)) { //means statement k modifies variable var
							if(_pkb->getPreprocessedProgram()->at(k).type == STMT_ASSIGNMENT) {
								resultLst.push_back(k);
								if(!exhaustive)
									return resultLst;
							}
							carryOn = false;
							break;
						}
					}
				}

				if(carryOn) { //should proceed to previous CFGNode
					STMT_LIST temp = getAffectsFirstHelper(currentNode, visitedNodes, usedVar, exhaustive);
					for(int j=0; j<temp.size(); j++) { //combine results for all variables
						resultLst.push_back(temp.at(j));
						if(!exhaustive)
							return resultLst;
					}
				}
			}
		}
		return(resultLst);
	}
}
*/

/*
STMT_LIST DesignExtractor::getAffectsSecond(STMT stmt1, BOOLEAN exhaustive)
{
	//if exhaustive is true, the function finds all assignments affected by stmt1.
	//if exhaustive is false, the function stops once one assignment is found.

	vector<CFGNode*> visitedNodes;
	STMT_LIST stmt1s;
	stmt1s.push_back(stmt1);
	VAR_INDEX_LIST modifiedVar;
	_pkb->modifies(&stmt1s, &modifiedVar, 1); //get the variable modified by stmt1

	CFGNode* startNode = _pkb->getCFG()->getCFGNodeByStmtNumber(stmt1); //get the CFGNode that contains stmt1
	STMT end = startNode->getEndStatement(); //the end statement of this CFGNode

	STMT_LIST result;

	//one and only one variable can be modified by an assign statement, so size of modifiedVar must be 1
	//only need to check Affects for this single variable 'var' inside modifiedVar

	bool carryOn = true; //indicate whether to proceed to next CFGNode
		
	for(int k=stmt1+1; k<=end; k++) { //statements 'stmt+1' to 'end' are either assign or call
		STMT_LIST temp1; 
		temp1.push_back(k);

		if(_pkb->getPreprocessedProgram()->at(k).type == STMT_ASSIGNMENT && _pkb->uses(&temp1, &modifiedVar, 0) ) {
			result.push_back(k);
			if(!exhaustive) {
				return result;
			}	
		}
		if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken, should not move to nextNode
			carryOn = false;
			break;
		}
	}

	if(carryOn) { //should proceed to next CFGNode
		STMT_LIST temp = getAffectsSecondHelper(startNode, &visitedNodes, modifiedVar, exhaustive);
		for(int j=0; j<temp.size(); j++) {
			result.push_back(temp.at(j));
			if(!exhaustive)
				return result;
		}
	}
 
	return result;
}

STMT_LIST DesignExtractor::getAffectsSecondHelper(CFGNode* node1, vector<CFGNode*>* visitedNodes, VAR_INDEX_LIST modifiedVar, BOOLEAN exhaustive)
{
	STMT_LIST resultLst;
	vector<CFGNode*> next_nodes;
	//STMT_LIST next_stmts;
	//STMT_LIST stmt1s; 
	//stmt1s.push_back(node1->getEndStatement());

	//_pkb->next(&stmt1s, &next_stmts, 1);
	next_nodes = node1->getNextEdges();
	
	if (next_nodes.size() == 0) { // no more successor, base case, stop and return
		return(resultLst);
	}else {
		int next_nodes_size = next_nodes.size();
		//for(int i=0; i<next_nodes_size; i++) {
			//next_nodes.push_back(_pkb->getCFG()->getCFGNodeByStmtNumber(next_stmts.at(i)));
		//}
		for (int i = 0; i < next_nodes_size; i++) { // loop through each next CFGNode
			CFGNode* currentNode = next_nodes.at(i);
			if(indexOf((*visitedNodes), currentNode) >= 0){
				//current successor has been visited, do not visit it again
			} else {
				visitedNodes->push_back(currentNode); //mark current successor as visited to avoid re-visit

				bool carryOn = true; //indicate whether to proceed to next CFGNode

				if(currentNode->getCFGType() == CFG_IF_STATEMENT || currentNode->getCFGType() == CFG_WHILE_STATEMENT) {
					//this node is if or while, go to next CFGNode directly
				} else {
					int start1 = currentNode->getStartStatement();
					int end1 = currentNode->getEndStatement();
					for(int k=start1; k<=end1; k++) {
						STMT_LIST temp1; 
						temp1.push_back(k);

						if(_pkb->getPreprocessedProgram()->at(k).type == STMT_ASSIGNMENT && _pkb->uses(&temp1, &modifiedVar, 0) ) {
							resultLst.push_back(k);
							if(!exhaustive)
								return resultLst;
						}
						if(_pkb->modifies(&temp1, &modifiedVar, 0)) { //means statement k modifies variable var, path is broken, should not move to nextNode
							carryOn = false;
							break;
						}
					}
				}

				if(carryOn) { //should proceed to next CFGNode
					STMT_LIST temp = getAffectsSecondHelper(currentNode, visitedNodes, modifiedVar, exhaustive);
					for(int j=0; j<temp.size(); j++) { //combine results for all variables
						resultLst.push_back(temp.at(j));
						if(!exhaustive)
							return resultLst;
					}
				}
			}
		}
		return(resultLst);
	}
}
*/