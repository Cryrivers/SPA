#include "stdafx.h"
#include <iostream>
#include "PKBController.h"

using namespace std;

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
ASTNode *PKBController::astGetRoot()
{
	return(_ast->getRoot());
}


//follows api
void PKBController::addFollows(STMT stmt1, STMT stmt2)
{
	_follows->addFollows(stmt1, stmt2);
}


BOOLEAN PKBController::follows(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	return(_follows->follows(st1s_ptr, st2s_ptr, arg));
}


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
void PKBController::addParent(STMT stmt1, STMT stmt2)
{
	_parent->addParent(stmt1, stmt2);
}


BOOLEAN PKBController::parent(STMT_LIST *st1s_ptr, STMT_LIST *st2s_ptr, int arg)
{
	return(_parent->parent(st1s_ptr, st2s_ptr, arg));
}


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
void PKBController::addModifies(STMT stmt, VAR_INDEX var)
{
	_modifies->addModifies(stmt, var);
}


BOOLEAN PKBController::modifies(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	return(_modifies->modifies(sts_ptr, vs_ptr, arg));
}


STMT_LIST PKBController::getAllModifiesFirst()
{
	return(_modifies->getAllModifiesFirst());
}


VAR_INDEX_LIST PKBController::getAllModifiesSecond()
{
	return(_modifies->getAllModifiesSecond());
}


//uses api
void PKBController::addUses(STMT stmt, VAR_INDEX var)
{
	_uses->addUses(stmt, var);
}


BOOLEAN PKBController::uses(STMT_LIST *sts_ptr, VAR_INDEX_LIST *vs_ptr, int arg)
{
	return(_uses->uses(sts_ptr, vs_ptr, arg));
}


STMT_LIST PKBController::getAllUsesFirst()
{
	return(_uses->getAllUsesFirst());
}


STMT_LIST PKBController::getAllUsesSecond()
{
	return(_uses->getAllUsesSecond());
}


//vartable api
VAR_INDEX PKBController::addVar(STRING varName)
{
	return(_varTable->addVar(varName));
}


STRING PKBController::getVarName(VAR_INDEX ind)
{
	return(_varTable->getVarName(ind));
}


VAR_INDEX PKBController::getVarIndex(STRING varName)
{
	return(_varTable->getVarIndex(varName));
}


STRING_LIST PKBController::getAllVarName(VAR_INDEX_LIST inds)
{
	return(_varTable->getAllVarName(inds));
}

STRING_LIST PKBController::getAllVarName()
{
	return (_varTable->getAllVarName());
}

vector<statement> *PKBController::getPreprocessedProgram()
{
	return(_preprocessedProgram);
}


BOOLEAN PKBController::getAllStmt(STMT_LIST *result)
{
	return(_designExtractor->getAllStmt(result));
}


BOOLEAN PKBController::getAllAssignment(STMT_LIST *result)
{
	return(_designExtractor->getAllAssignment(result));
}


BOOLEAN PKBController::getAllWhile(STMT_LIST *result)
{
	return(_designExtractor->getAllWhile(result));
}


BOOLEAN PKBController::getAllConstant(vector<int> *result)
{
	return(_designExtractor->getAllConstant(result));
}


BOOLEAN PKBController::pattern(vector<int> *a, vector<int> *b, string expr, int arg)
{
	return(_designExtractor->pattern(a, b, expr, arg));
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
	_uses = new Uses();
	_varTable = new VarTable();
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
