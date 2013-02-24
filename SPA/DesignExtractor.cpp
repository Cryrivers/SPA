/**
 * \file	DesignExtractor.cpp
 * \class	Design Extractor
 * \brief	Handles Pattern, With and getAll* queries and complete Modifies and Uses table
 */

#include "stdafx.h"
#include "DesignExtractor.h"
#include "SPAType.h"
#include "ASTNode.h"
#include <string>

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
		if (_pkb->getPreprocessedProgram()->at(i).stmtNumber != 0) {
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
	if (arg == 0)
	{
		if(b->size() == 0)
		{

		}
		else if (b->size() == 1)
		{

		}
	}
	else if (arg == 1)
	{
		if (a->size() == 0 && b->size() == 0)
		{
		}
		else if (a->size() > 0 && b->size() == 0)
		{
		}
		else if (a->size() == 0 && b->size() > 0)
		{
		}
		else if (a->size() > 0 && b->size() > 0)
		{
		}
	}
	else if (arg == 2)
	{
		if (a->size() == 0 && b->size() == 0)
		{
		}
		else if (a->size() == 0 && b->size() ==1)
		{
		}
		else if (a->size() > 0 && b->size() == 0)
		{
		}
		else if (a->size() > 0 && b->size() == 1)
		{
		}
	}
	else if (arg == 3)
	{
		if (a->size() == 0 && b->size() == 0)
		{
		}
		else if (a->size() == 0 && b->size() > 0)
		{
		}
		else if (a->size() > 0 && b->size() == 0)
		{
		}
		else if (a->size() > 0 && b->size() > 0)
		{
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
	int i = 0, flag2 = 0;
	if (arg == 0)
	{
		//case 1a
		if(b->size() == 0)
		{
			getAllIf(temp);
			if (temp->size() == 0)
			{
				delete(temp);
				return  false;
			}
			delete(temp);
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
		//case 2b
		else if (a->size() > 0 && b->size() == 0)
		{
		}
		else if (a->size() == 0 && b->size() > 0)
		{
		}
		else if (a->size() > 0 && b->size() > 0)
		{
		}
	}
	else if (arg == 2)
	{
		if (a->size() == 0 && b->size() == 0)
		{
		}
		else if (a->size() == 0 && b->size() ==1)
		{
		}
		else if (a->size() > 0 && b->size() == 0)
		{
		}
		else if (a->size() > 0 && b->size() == 1)
		{
		}
	}
	else if (arg == 3)
	{
		if (a->size() == 0 && b->size() == 0)
		{
		}
		else if (a->size() == 0 && b->size() > 0)
		{
		}
		else if (a->size() > 0 && b->size() == 0)
		{
		}
		else if (a->size() > 0 && b->size() > 0)
		{
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
					b->erase(a->begin() + i);
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
}
