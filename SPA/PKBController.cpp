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
VAR_INDEX PKBController::addProc(STRING procName)
{
	return(_procTable->addProc(procName));
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
BOOLEAN PKBController::pattern(vector<int> *a, vector<int> *b, string expr, int arg)
{
	return(_designExtractor->pattern(a, b, expr, arg));
}

/**
 * \fn	BOOLEAN PKBController::with(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg1,int arg2)
 *
 * \brief	Propagate the call to Design Extractor		
 *
 * \author	Yue Cong
 *
 */
BOOLEAN PKBController::with(STMT_LIST* st1s_ptr, STMT_LIST* st2s_ptr,int arg1,int arg2){
	return _designExtractor->with(st1s_ptr,st2s_ptr,arg1,arg2);
}

void PKBController::completePKB(){
	_designExtractor->addModifies();
	_designExtractor->addUses();
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
	delete(_preprocessedProgram);
	delete(_designExtractor);
	delete(_constTable);
	delete(_assignmentTable);
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
	_varTable = new VarTable();
	_procTable = new ProcTable();
	_constTable = new vector<int>();
	_assignmentTable = new vector<ASTNode *>();
	_ast = new AST();
	_designExtractor = new DesignExtractor();
	_preprocessedProgram = new vector<statement>();
}

void PKBController::setPreprocessedProgram( vector<statement>* p )
{
	_preprocessedProgram = p;
}
