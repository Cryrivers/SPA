#include "stdafx.h"
#include "DesignExtractor.h"
#include "SPAType.h"
#include "ASTNode.h"
#include <string>

DesignExtractor::DesignExtractor(void)
{
	_pkb = PKBController::createInstance();
	_ast = new AST();
}


DesignExtractor::~DesignExtractor(void)
{
	delete(_ast);
}


void DesignExtractor::addModifies()
{
	STMT_LIST stmts = _pkb->getAllModifiesFirst();
	VAR_INDEX_LIST vars = _pkb->getAllModifiesSecond();
	STMT_LIST      *parents = new STMT_LIST();
	STMT_LIST      *child = new STMT_LIST();

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


void DesignExtractor::addUses()
{
	int j = 0;
	STMT_LIST stmts = _pkb->getAllUsesFirst();
	VAR_INDEX_LIST vars = _pkb->getAllUsesSecond();
	STMT_LIST      *parents = new STMT_LIST();
	STMT_LIST      *child = new STMT_LIST();

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


BOOLEAN DesignExtractor::pattern(vector<int> *a, vector<int> *b, string expr, int arg)
{
	ASTNode *tree;
	vector<ASTNode *> *assign = _pkb->getAssignmentTable();

	vector<int> *temp = new vector<int>();
	int flag = 0, i = 0, flag2 = 0;
	if (expr!="" && expr[0] == '"') {
		expr = expr.substr(1, expr.size()-2);
		flag = 1;
	}
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
