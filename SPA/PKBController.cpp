/**
 * \file	PKBController.cpp
 * \class	PKB
 * \brief	Interface between PKB and Parser, PKB and Query Processor
 */

#include "stdafx.h"
#include <iostream>
#include "PKBController.h"

using namespace std;

/**
 * \fn	PKBController *PKBController::createInstance()
 *
 * \brief	Create only once the PKBController object.
 * 			If PKBController object exists, return the object.
 *			If PKBController object does not exist, create new object and return.
 *
 * \author	Yue Cong
 *
 * \param [out]	obj	The PKBController object
 */

PKBController *PKBController::createInstance()
{
	static PKBController *obj;

	if (obj != NULL) {
		return(obj);
	}else {
		obj = new PKBController();
		obj->init();
		return(obj);
	}
}


//ast api
/**
 * \fn	ASTNode *PKBController::astGetRoot()
 *
 * \brief	Return the root node of AST
 *			For Design Extractor and Parser to access AST
 *
 * \author	Yue Cong
 *
 * \param [out]	ast	ASTNode object
 */
ASTNode *PKBController::astGetRoot()
{
	return(_ast->getRoot());
}


//follows api

/**
 * \fn	void PKBController::addFollows(STMT stmt1, STMT stmt2)
 *
 * \brief	For Parser to add relation Follows(stmt1,stmt2) to Follows table			
 *
 * \author	Yue Cong
 *
 */
void PKBController::addFollows(STMT stmt1, STMT stmt2)
{
	_follows->addFollows(stmt1, stmt2);
}

/**
 * \fn	BOOLEAN PKBController::follows(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
 *
 * \brief	Propagate the call to follows table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::follows(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	return(_follows->follows(st1s_ptr, st2s_ptr, arg));
}

/**
 * \fn	BOOLEAN PKBController::followsStar(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
 *
 * \brief	Propagate the call to follows table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::followsStar(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	if ((arg == 3) && (st1s_ptr->size() == 0) && (st2s_ptr->size() == 0)) {
		if (_follows->followsStar(st1s_ptr, st2s_ptr, 2)) {
			return(_follows->followsStar(st1s_ptr, st2s_ptr, 3));
		}else {
			return(false);
		}
	}else {
		return(_follows->followsStar(st1s_ptr, st2s_ptr, arg));
	}
}


//parent api
/**
 * \fn	void PKBController::addParent(STMT stmt1, STMT stmt2)
 *
 * \brief	Propagate the call to Parent table		
 *
 * \author	Yue Cong
 *
 */
void PKBController::addParent(STMT stmt1, STMT stmt2)
{
	_parent->addParent(stmt1, stmt2);
}

/**
 * \fn	BOOLEAN PKBController::parent(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
 *
 * \brief	Propagate the call to Parent table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::parent(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	return(_parent->parent(st1s_ptr, st2s_ptr, arg));
}

/**
 * \fn	PKBController::parentStar(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
 *
 * \brief	Propagate the call to Parent table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::parentStar(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	if ((arg == 3) && (st1s_ptr->size() == 0) && (st2s_ptr->size() == 0)) {
		if (_parent->parentStar(st1s_ptr, st2s_ptr, 2)) {
			return(_parent->parentStar(st1s_ptr, st2s_ptr, 3));
		}else {
			return(false);
		}
	}else {
		return(_parent->parentStar(st1s_ptr, st2s_ptr, arg));
	}
}


//modifies api
/**
 * \fn	PKBController::addModifies(STMT stmt, VAR_INDEX var)
 *
 * \brief	Propagate the call to Modifies table		
 *
 * \author	Yue Cong
 *
 */
void PKBController::addModifies(STMT stmt, VAR_INDEX var)
{
	_modifies->addModifies(stmt, var);
}

/**
 * \fn	BOOLEAN PKBController::modifies(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
 *
 * \brief	Propagate the call to Modifies table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::modifies(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	return(_modifies->modifies(sts_ptr, vs_ptr, arg));
}

/**
 * \fn	STMT_LIST PKBController::getAllModifiesFirst()
 *
 * \brief	Propagate the call to Modifies table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllModifiesFirst()
{
	return(_modifies->getAllModifiesFirst());
}

/**
 * \fn	VAR_INDEX_LIST PKBController::getAllModifiesSecond()
 *
 * \brief	Propagate the call to Modifies table		
 *
 * \author	Yue Cong
 *
 */
VAR_INDEX_LIST PKBController::getAllModifiesSecond()
{
	return(_modifies->getAllModifiesSecond());
}

/**
 * \fn	void PKBController::addModifiesP(PROC_INDEX proc, VAR_INDEX var)
 *
 * \brief	Propagate the call to ModifiesP table		
 *
 * \author	Yue Cong
 *
 */
void PKBController::addModifiesP(PROC_INDEX proc, VAR_INDEX var)
{
	return _modifiesP->addModifiesP(proc, var);
}

/**
 * \fn	BOOLEAN PKBController::modifiesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr,int arg)
 *
 * \brief	Propagate the call to ModifiesP table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::modifiesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr,int arg)
{
	return _modifiesP->modifiesP(ps_ptr, vs_ptr, arg);
}

/**
 * \fn	STMT_LIST PKBController::getAllModifiesPFirst()
 *
 * \brief	Propagate the call to ModifiesP table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllModifiesPFirst()
{
	return _modifiesP->getAllModifiesPFirst();
}

/**
 * \fn	VAR_INDEX_LIST PKBController::getAllModifiesPSecond()
 *
 * \brief	Propagate the call to ModifiesP table		
 *
 * \author	Yue Cong
 *
 */
VAR_INDEX_LIST PKBController::getAllModifiesPSecond()
{
	return _modifiesP->getAllModifiesPSecond();
}


//uses api
/**
 * \fn	void PKBController::addUses(STMT stmt, VAR_INDEX var)
 *
 * \brief	Propagate the call to Uses table		
 *
 * \author	Yue Cong
 *
 */
void PKBController::addUses(STMT stmt, VAR_INDEX var)
{
	_uses->addUses(stmt, var);
}

/**
 * \fn	BOOLEAN PKBController::uses(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
 *
 * \brief	Propagate the call to Uses table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::uses(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	return(_uses->uses(sts_ptr, vs_ptr, arg));
}

/**
 * \fn	STMT_LIST PKBController::getAllUsesFirst()
 *
 * \brief	Propagate the call to Uses table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllUsesFirst()
{
	return(_uses->getAllUsesFirst());
}

/**
 * \fn	STMT_LIST PKBController::getAllUsesSecond()
 *
 * \brief	Propagate the call to Uses table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllUsesSecond()
{
	return(_uses->getAllUsesSecond());
}

/**
 * \fn	void PKBController::addUsesP(PROC_INDEX proc, VAR_INDEX var)
 *
 * \brief	Propagate the call to UsesP table		
 *
 * \author	Yue Cong
 *
 */
void PKBController::addUsesP(PROC_INDEX proc, VAR_INDEX var)
{
	return _usesP->addUsesP(proc, var);
}

/**
 * \fn	BOOLEAN PKBController::usesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr,int arg)
 *
 * \brief	Propagate the call to UsesP table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::usesP(PROC_INDEX_LIST* ps_ptr, VAR_INDEX_LIST* vs_ptr,int arg)
{
	return _usesP->usesP(ps_ptr, vs_ptr, arg);
}

/**
 * \fn	STMT_LIST PKBController::getAllUsesPFirst()
 *
 * \brief	Propagate the call to UsesP table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllUsesPFirst()
{
	return _usesP->getAllUsesPFirst();
}

/**
 * \fn	STMT_LIST PKBController::getAllUsesPSecond()
 *
 * \brief	Propagate the call to UsesP table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllUsesPSecond()
{
	return _usesP->getAllUsesPSecond();
}

/**
 * \fn	void PKBController::addCalls(STMT stmt, PROC_INDEX caller, PROC_INDEX callee)
 *
 * \brief	Propagate the call to Calls table		
 *
 * \author	Yue Cong
 *
 */
void PKBController::addCalls(STMT stmt, PROC_INDEX caller, PROC_INDEX callee)
{
	return (_calls->addCalls(stmt,caller,callee));
}

/**
 * \fn	BOOLEAN PKBController::calls(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg)
 *
 * \brief	Propagate the call to Calls table		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::calls(PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg)
{
	return _calls->calls(callers_ptr,callees_ptr,arg);
}

/**
 * \fn	PROC_INDEX PKBController::getCallee(STMT stmt)
 *
 * \brief	Propagate the call to Calls table		
 *
 * \author	Yue Cong
 *
 */
PROC_INDEX PKBController::getCallee(STMT stmt)
{
	return _calls->getCallee(stmt);
}
/**
 * \fn	STMT_LIST PKBController::getAllCallers()
 *
 * \brief	Propagate the call to Calls table		
 *
 * \author	Yue Cong
 *
 */
STMT_LIST PKBController::getAllCallers()
{
	return _calls->getAllCallers();
}

/**
 * \fn	PROC_INDEX_LIST PKBController::getAllCallsSecond()
 *
 * \brief	Propagate the call to Calls table		
 *
 * \author	Yue Cong
 *
 */
PROC_INDEX_LIST PKBController::getAllCallees()
{
	return _calls->getAllCallees();
}

//vartable api
/**
 * \fn	VAR_INDEX PKBController::addVar(STRING varName)
 *
 * \brief	Propagate the call to Var table		
 *
 * \author	Yue Cong
 *
 */
VAR_INDEX PKBController::addVar(STRING varName)
{
	return(_varTable->addVar(varName));
}

/**
 * \fn	STRING PKBController::getVarName(VAR_INDEX ind)
 *
 * \brief	Propagate the call to Var table		
 *
 * \author	Yue Cong
 *
 */
STRING PKBController::getVarName(VAR_INDEX ind)
{
	return(_varTable->getVarName(ind));
}

/**
 * \fn	VAR_INDEX PKBController::getVarIndex(STRING varName)
 *
 * \brief	Propagate the call to Var table		
 *
 * \author	Yue Cong
 *
 */
VAR_INDEX PKBController::getVarIndex(STRING varName)
{
	return(_varTable->getVarIndex(varName));
}

/**
 * \fn	STRING_LIST PKBController::getAllVarName(VAR_INDEX_LIST inds)
 *
 * \brief	Propagate the call to Var table		
 *
 * \author	Yue Cong
 *
 */
STRING_LIST PKBController::getAllVarName(VAR_INDEX_LIST inds)
{
	return(_varTable->getAllVarName(inds));
}
/**
 * \fn	STRING_LIST PKBController::getAllVarName()
 *
 * \brief	Propagate the call to Var table		
 *
 * \author	Yue Cong
 *
 */
STRING_LIST PKBController::getAllVarName()
{
	return (_varTable->getAllVarName());
}

//proctable
/**
 * \fn	VAR_INDEX PKBController::addProc(STRING procName)
 *
 * \brief	Propagate the call to Proc table		
 *
 * \author	Yue Cong
 *
 */
VAR_INDEX PKBController::addProc(STRING procName,STMT start, STMT end)
{
	return(_procTable->addProc(procName,start,end));
}

/**
 * \fn	STRING PKBController::getProcName(VAR_INDEX ind)
 *
 * \brief	Propagate the call to Proc table		
 *
 * \author	Yue Cong
 *
 */
STRING PKBController::getProcName(VAR_INDEX ind)
{
	return(_procTable->getProcName(ind));
}

/**
 * \fn	VAR_INDEX PKBController::getProcIndex(STRING procName)
 *
 * \brief	Propagate the call to Proc table		
 *
 * \author	Yue Cong
 *
 */
VAR_INDEX PKBController::getProcIndex(STRING procName)
{
	return(_procTable->getProcIndex(procName));
}

/**
 * \fn	STRING_LIST PKBController::getAllProcName(VAR_INDEX_LIST inds)
 *
 * \brief	Propagate the call to Proc table		
 *
 * \author	Yue Cong
 *
 */
STRING_LIST PKBController::getAllProcName(VAR_INDEX_LIST inds)
{
	return(_procTable->getAllProcName(inds));
}
/**
 * \fn	STRING_LIST PKBController::getAllProcName()
 *
 * \brief	Propagate the call to Proc table		
 *
 * \author	Yue Cong
 *
 */
STRING_LIST PKBController::getAllProcName()
{
	return (_procTable->getAllProcName());
}

//other
/**
 * \fn	vector<statement> *PKBController::getPreprocessedProgram()
 *
 * \brief	Get the table of preprocessed program. Easier for preprocessor to travers
 *
 * \author	Yue Cong
 *
 */
vector<statement> *PKBController::getPreprocessedProgram()
{
	return(_preprocessedProgram);
}

/**
 * \fn	BOOLEAN PKBController::getAllStmt(STMT_LIST *result)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::getAllStmt(STMT_LIST *result)
{
	return(_designExtractor->getAllStmt(result));
}

/**
 * \fn	BOOLEAN PKBController::getAllAssignment(STMT_LIST *result)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::getAllAssignment(STMT_LIST *result)
{
	return(_designExtractor->getAllAssignment(result));
}

/**
 * \fn	BOOLEAN PKBController::getAllWhile(STMT_LIST *result)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::getAllWhile(STMT_LIST *result)
{
	return(_designExtractor->getAllWhile(result));
}

/**
 * \fn	BOOLEAN PKBController::getAllIf(STMT_LIST *result)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::getAllIf(STMT_LIST *result)
{
	return(_designExtractor->getAllIf(result));
}

/**
 * \fn	BOOLEAN PKBController::getAllCall(STMT_LIST *result)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::getAllCall(STMT_LIST *result)
{
	return(_designExtractor->getAllCall(result));
}

/**
 * \fn	BOOLEAN PKBController::getAllConstant(vector<int> *result)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::getAllConstant(vector<int> *result)
{
	return(_designExtractor->getAllConstant(result));
}

/**
 * \fn	BOOLEAN PKBController::pattern(vector<int> *a, vector<int> *b, string expr, int arg)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::pattern(vector<int> *a, vector<int> *b, string expr, PatternQueryType queryType, int arg)
{
	return(_designExtractor->pattern(a, b, expr, queryType,arg));
}

/**
 * \fn	BOOLEAN PKBController::with(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg1,int arg2)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::with(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg1,int arg2, int arg){
	switch(arg1){
	case WITH_VARNAME:
		switch(arg2){
		case WITH_CALLPROCNAME:
			return _var_call->parent(st1s_ptr, st2s_ptr, arg);
			break;
		case WITH_PROCNAME:
			return _proc_var->follows(st2s_ptr, st1s_ptr, arg);
			break;
		default:
			break;
		}
		break;
	case WITH_PROCNAME:
		switch (arg2)
		{
		case WITH_CALLPROCNAME:
			return _proc_call->parent(st1s_ptr, st2s_ptr, arg);
			break;
		case WITH_VARNAME:
			return _proc_var->follows(st1s_ptr,st2s_ptr,arg);
			break;
		default:
			break;
		}
		break;
	case WITH_CALLPROCNAME:
		switch (arg2)
		{
		case WITH_VARNAME:
			return _var_call->parent(st2s_ptr, st1s_ptr, arg);
			break;
		case WITH_PROCNAME:
			return _proc_call->parent(st2s_ptr, st1s_ptr, arg);
			break;
		case WITH_CALLPROCNAME:
			return _call_call->modifies(st1s_ptr, st2s_ptr, arg);
			break;
		default:
			break;
		}
		break;
	case WITH_STMTNUMBER:
		switch (arg2)
		{
		case WITH_VALUE:
			return _stmt_const->follows(st1s_ptr, st2s_ptr, arg);
			break;
		default:
			break;
		}
		break;
	case WITH_VALUE:
		switch (arg2)
		{
		case WITH_STMTNUMBER:
			return _stmt_const->follows(st2s_ptr, st1s_ptr, arg);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return false;
}

void PKBController::completePKB(){
	_designExtractor->connectCFG(_cfg, CFG_BIP_DISABLED);
	_designExtractor->connectCFG(_cfgbip, CFG_BIP_ENABLED);
	_designExtractor->addModifies();
	_designExtractor->addUses();
	_designExtractor->addNext();
	build_call_call();
	build_proc_call();
	build_proc_var();
	build_var_call();
	build_stmt_const();
}

PKBController::PKBController()
{
}


PKBController::~PKBController()
{
	delete(_follows);
	delete(_parent);
	delete(_modifies);
	delete(_uses);
	delete(_varTable);
	delete(_procTable);
	delete(_ast);
	delete(_cfg);
	delete(_cfgbip);
	delete(_preprocessedProgram);
	delete(_designExtractor);
	delete(_constTable);
	delete(_assignmentTable);
	delete(_whileLoopTable);
	delete(_ifBranchTable);
}


AST *PKBController::getAST()
{
	return(_ast);
}


void PKBController::addConstant(int value)
{
	_constTable->push_back(value);
}


vector<int> *PKBController::getConstantTable()
{
	return(_constTable);
}


void PKBController::addAssignmentNode(ASTNode *node)
{
	_assignmentTable->push_back(node);
}


vector<ASTNode *> *PKBController::getAssignmentTable()
{
	return(_assignmentTable);
}

void PKBController::init()
{
	_follows = new Follows();
	_parent = new Parent();
	_modifies = new Modifies();
	_modifiesP = new ModifiesP();
	_uses = new Uses();
	_usesP = new UsesP();
	_calls = new Calls();
	_next = new Next();
	_varTable = new VarTable();
	_procTable = new ProcTable();
	_constTable = new vector<int>();
	_assignmentTable = new vector<ASTNode *>();
	_whileLoopTable = new vector<ASTNode *>();
	_ifBranchTable = new vector<ASTNode *>();
	_ast = new AST();
	_cfg = new CFG();
	_cfgbip = new CFG();
	_designExtractor = new DesignExtractor();
	_preprocessedProgram = new vector<statement>();
	//with
	_proc_call = new Parent();
	_proc_var = new Follows();
	_call_call = new Modifies();
	_var_call = new Parent();
	_stmt_const = new Follows();
	//operators
	_plus = new Operators();
	_minus = new Operators();
	_multiply = new Operators();
}

void PKBController::setPreprocessedProgram( vector<statement>* p )
{
	_preprocessedProgram = p;
}

CFG* PKBController::getCFG()
{
	return _cfg;
}

CFG* PKBController::getCFGBip()
{
	return _cfgbip;
}

void PKBController::build_proc_var(){
	for (int i = 0; i < _procTable->size(); i++)
	{
		for (int j = 0; j < _varTable->size(); j++){
			if (_procTable->getProcName(i)==_varTable->getVarName(j))
			{
				_proc_var->addFollows(i,j);
			}
		}
	}
}

void PKBController::build_call_call(){
	vector<int>* _a = new vector<int>();
	getAllCall(_a);

	for (int i = 0; i < _a->size(); i++)
	{
		_call_call->addModifies(_a->at(i), _a->at(i));
		for (int j = i+1; j < _a->size(); j++){
			if(_calls->getCallee(_a->at(i)) == _calls->getCallee(_a->at(j))){
				_call_call->addModifies(_a->at(i), _a->at(j));
			}
		}
	}
	delete(_a);
}

void PKBController::build_proc_call(){
	vector<int>* _a = new vector<int>();
	getAllCall(_a);

	for(int i = 0; i < _procTable->size(); i++){
		for (int j = 0; j < _a->size(); j++)
		{
			if(i == _calls->getCallee(_a->at(j))){
				_proc_call->addParent(i, _a->at(j));
			}
		}
	}
	delete(_a);
}

void PKBController::build_var_call(){
	vector<int>* _a = new vector<int>();
	getAllCall(_a);

	for (int i = 0; i < _varTable->size(); i++)
	{
		for (int j = 0; j < _a->size(); j++)
		{
			if(_varTable->getVarName(i) == _procTable->getProcName(getCallee(_a->at(j)))){
				_var_call->addParent(i, _a->at(j));
			}
		}
	}
	delete(_a);
}

void PKBController::build_stmt_const(){
	vector<int> *stmtlist = new vector<int>();
	getAllStmt(stmtlist);
	for (int i = 0; i < _constTable->size(); i++)
	{
		if (_constTable->at(i)<stmtlist->size()&&_constTable->at(i)>0)
		{
			_stmt_const->addFollows(_constTable->at(i), _constTable->at(i));
		}
	}
	delete(stmtlist);
}

/**
 * \fn	void PKBController::addWhileLoopNode( ASTNode* node )
 *
 * \brief	Adds a while loop node.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \param [in]	node	If non-null, the node.
 */

void PKBController::addWhileLoopNode( ASTNode* node )
{
	_whileLoopTable->push_back(node);
}

/**
 * \fn	void PKBController::addIfBranchNode( ASTNode* node )
 *
 * \brief	Adds if branch node.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \param [in,out]	node	If non-null, the node.
 */

void PKBController::addIfBranchNode( ASTNode* node )
{
	_ifBranchTable->push_back(node);
}

/**
 * \fn	vector<ASTNode*>* PKBController::getIfBranchTable()
 *
 * \brief	Gets if branch table.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \return	null if it fails, else if branch table.
 */

vector<ASTNode*>* PKBController::getIfBranchTable()
{
	return _ifBranchTable;
}

/**
 * \fn	vector<ASTNode*>* PKBController::getWhileTable()
 *
 * \brief	Gets while table.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \return	null if it fails, else the while table.
 */

vector<ASTNode*>* PKBController::getWhileTable()
{
	return _whileLoopTable;
}

void PKBController::addNext( STMT stmt1, STMT stmt2, CFGBipType bipType)
{
	_next->addNext(stmt1, stmt2, bipType);
}

BOOLEAN PKBController::next( STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg )
{
	return _next->next(st1s_ptr, st2s_ptr, arg);
}

BOOLEAN PKBController::nextStar( STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg )
{
	if ((arg == 3) && (st1s_ptr->size() == 0) && (st2s_ptr->size() == 0)) {
		if (_next->nextStar(st1s_ptr, st2s_ptr, 2)) {
			return(_next->nextStar(st1s_ptr, st2s_ptr, 3));
		}else {
			return(false);
		}
	}else {
		return(_next->nextStar(st1s_ptr, st2s_ptr, arg));
	}
}

STMT_LIST PKBController::getAllNextFirst()
{
	return _next->getAllNextFirst();
}

STMT_LIST PKBController::getAllNextSecond()
{
	return _next->getAllNextSecond();
}

BOOLEAN PKBController::affects( STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	return _designExtractor->affects(st1s_ptr, st2s_ptr, arg);
}

BOOLEAN PKBController::affectsStar( STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr, int arg)
{
	return _designExtractor->affectsStar(st1s_ptr, st2s_ptr, arg);
}

BOOLEAN PKBController::callsStar( PROC_INDEX_LIST* callers_ptr, PROC_INDEX_LIST* callees_ptr, int arg )
{
	return _calls->callsStar(callers_ptr, callees_ptr, arg);
}

STMT PKBController::getProcEnd( PROC_INDEX ind )
{
	return _procTable->getProcEnd(ind);
}

STMT PKBController::getProcStart( PROC_INDEX ind )
{
	return _procTable->getProcStart(ind);
}

void PKBController::addPlusOperator( int ASTNodeId )
{
	_plus->addOperatorByASTNodeId(ASTNodeId);
}

void PKBController::addMinusOperator( int ASTNodeId )
{
	_minus->addOperatorByASTNodeId(ASTNodeId);
}

void PKBController::addMultiplyOperator( int ASTNodeId )
{
	_multiply->addOperatorByASTNodeId(ASTNodeId);
}
