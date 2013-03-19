/**
 * \file	Parser.cpp
 * \class	Parser
 * \brief	Implements SIMPLE parser class.
 */

#include "stdafx.h"
#include <iostream>
#include <string>
#include <regex>
#include "Parser.h"
#include <assert.h>

#define LEFT_BRACKET     0
#define RIGHT_BRACKET    1
#define SEMICOLON        2
#define NO_STATEMENT_NUMBER -1
#define NO_SPECIFIC_PROC_INDEX -1

using namespace std;

/**
 * \fn	Parser::Parser(AST *ast)
 *
 * \brief	Constructor of Parser. Developers need to specify an AST object.
 * 			All the parsing result will directly output to the AST object.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param [out]	ast	The AST object which stores the parsing result.
 */

Parser::Parser(AST *ast)
{
	_ast = ast;
	_parentStack.push(ast->getRoot());
	_pkb = PKBController::createInstance();
	preprocProgram = _pkb->getPreprocessedProgram();
	sameLevelAtNext = true;
	previousASTNode = ASTNode::createNode(AST_ANY,0);
	previousASTNode->setStmtNumber(0);
}


Parser::~Parser(void)
{
}


void Parser::_parseLine()
{
	CFGNode* currentCFGNode = NULL;
	statement s;
	for (vector<statement>::iterator it = preprocProgram->begin(); it != preprocProgram->end(); ++it) {
		ASTNode* currentASTNode;
		switch (it->type) {

		case STMT_CALL:
			assert(currentCFGNode != NULL);

			if(CFG_BIP_ENABLED)
			{
				if(currentCFGNode->isValidCFGNode())
					_pkb->getCFG()->addToCFG(currentCFGNode);
				else
					delete currentCFGNode;
				currentCFGNode = new CFGNode();
				currentCFGNode->setStartStatement(it->stmtNumber);
				currentCFGNode->setEndStatement(it->stmtNumber);
				currentCFGNode->setProcIndex(it->procIndex);
				currentCFGNode->setCFGType(CFG_BIP_CALL_STATEMENT);
				_pkb->getCFG()->addToCFG(currentCFGNode);
				currentCFGNode = new CFGNode();
			}
			else
			{
				if(!currentCFGNode->isValidCFGNode())
				{
					currentCFGNode->setProcIndex(it->procIndex);
					currentCFGNode->setStartStatement(it->stmtNumber);
				}
				currentCFGNode->setEndStatement(it->stmtNumber);
			}

			currentASTNode = _buildCallAST(&*it);
			__addFollowsTable(currentASTNode);
			previousASTNode =  currentASTNode;
			
			break;

		case STMT_IF:
			assert(currentCFGNode != NULL);
			if(currentCFGNode->isValidCFGNode())
				_pkb->getCFG()->addToCFG(currentCFGNode);
			else
				delete currentCFGNode;
			currentCFGNode = new CFGNode();
			currentCFGNode->setStartStatement(it->stmtNumber);
			currentCFGNode->setEndStatement(it->stmtNumber);
			currentCFGNode->setProcIndex(it->procIndex);
			currentCFGNode->setCFGType(CFG_IF_STATEMENT);
			_pkb->getCFG()->addToCFG(currentCFGNode);
			currentCFGNode = NULL;

			_buildIfAST(&*it);
			break;

		case STMT_ELSE:
			_buildElseAST(&*it);
			break;

		case STMT_PROCEDURE:
			if(currentCFGNode != NULL) delete currentCFGNode;
			_pkb->addProc(it->extraName,it->stmtNumber,it->endOfTheScope);
			_buildProcedureAST(&*it);
			break;

		case STMT_ASSIGNMENT:
			assert(currentCFGNode != NULL);
			if(!currentCFGNode->isValidCFGNode())
			{
				currentCFGNode->setProcIndex(it->procIndex);
				currentCFGNode->setStartStatement(it->stmtNumber);
			}
			currentCFGNode->setEndStatement(it->stmtNumber);

			currentASTNode = _buildAssignmentAST(&*it);
			__addFollowsTable(currentASTNode);

			previousASTNode = currentASTNode;
			break;

		case STMT_WHILE:
			assert(currentCFGNode != NULL);
			if(currentCFGNode->isValidCFGNode())
				_pkb->getCFG()->addToCFG(currentCFGNode);
			else
				delete currentCFGNode;
			currentCFGNode = new CFGNode();
			currentCFGNode->setStartStatement(it->stmtNumber);
			currentCFGNode->setEndStatement(it->stmtNumber);
			currentCFGNode->setProcIndex(it->procIndex);
			currentCFGNode->setCFGType(CFG_WHILE_STATEMENT);
			_pkb->getCFG()->addToCFG(currentCFGNode);
			currentCFGNode = NULL;

			currentASTNode = _buildWhileLoopAST(&*it);
			if(sameLevelAtNext)
			{
				if(previousASTNode->getStmtNumber()>0)
					_pkb->addFollows(previousASTNode->getStmtNumber(), currentASTNode->getStmtNumber());
			}
			else
			{
				sameLevelAtNext = true;
			}
			previousASTNode = currentASTNode;
			break; 

		case STMT_OPEN_BRACKET:
			//New CFG Node & set start of the CFG
			currentCFGNode = new CFGNode();
			currentCFGNode->setStartStatement(it->stmtNumber);
			currentCFGNode->setProcIndex(it->procIndex);
			currentCFGNode->setCFGType(CFG_NORMAL_BLOCK);

			_parentStack.push(_newParent);
			_parentStackNoStmtLst.push(_newParentNoStmtLst);
			sameLevelAtNext = false;
			break;

		case STMT_CLOSE_BRACKET_END_OF_PROC:
		case STMT_CLOSE_BRACKET_END_OF_THEN:
		case STMT_CLOSE_BRACKET_END_OF_ELSE:
		case STMT_CLOSE_BRACKET_END_OF_WHILE:


			assert(currentCFGNode != NULL);
			
			if(currentCFGNode->isValidCFGNode())
				_pkb->getCFG()->addToCFG(currentCFGNode);
			else
				delete currentCFGNode;
			
			//Will leak 1 object
			currentCFGNode = new CFGNode();

			previousASTNode = _parentStackNoStmtLst.top();	
			_parentStack.pop();
			_parentStackNoStmtLst.pop();
			sameLevelAtNext = true;
			break;

		default:
			cout << "Cannot build AST." << endl;
		}
	}
}

void Parser::_preprocessProgram(string program)
{
	string prog = program;

	int currentProcIndex = NO_SPECIFIC_PROC_INDEX;
	int currentStmtNumber = 0;
	StmtType currentLineSeparatorType;
	currentLineSeparatorType = STMT_NONE;
	int nearestSeparator = -1;

	do {
		size_t lbracketPos = prog.find_first_of("{");
		size_t rbracketPos = prog.find_first_of("}");
		size_t semicolonPos = prog.find_first_of(";");

		nearestSeparator = _findValidMinimumOfThree(lbracketPos, rbracketPos, semicolonPos);
		size_t nearestPos;

		if (nearestSeparator == LEFT_BRACKET) {
			nearestPos = lbracketPos;
		}else if (nearestSeparator == RIGHT_BRACKET) {
			nearestPos = rbracketPos;
		}else if (nearestSeparator == SEMICOLON) {
			nearestPos = semicolonPos;
		}else {
			break;
		}

		string thisStmt = prog.substr(0, nearestPos);
		prog.erase(0, nearestPos + 1);
		thisStmt.erase(thisStmt.find_last_not_of(" \n\r\t")+1);
		
		//Parse thisStmt
		if (thisStmt.size() > 0) {
			regex procRegex("\\s*procedure\\s*([a-zA-Z0-9]*)\\s*");
			regex whileRegex("\\s*while\\s*([a-zA-Z0-9]*)\\s*");
			regex ifRegex("\\s*if\\s*([a-zA-Z0-9]*)\\s*then\\s*");
			regex elseRegex("\\s*else\\s*");
			regex callRegex("\\s*call\\s*([a-zA-Z0-9]*)\\s*");
			regex assignRegex("\\s*([a-zA-Z0-9]+)\\s*=\\s*((\\(|[a-zA-Z0-9]+)+(\\s*[\\+|\\-|\\*|\\(]\\s*\\(*\\s*[a-zA-Z0-9]+\\s*[\\)|\\s]*)*)");
			smatch sm;

			statement s;
			
			if (regex_match(thisStmt, sm, procRegex)) {
				s.stmtLine = thisStmt;
				//Equals the statement number of the first line
				// of code in this procedure
				s.stmtNumber = currentStmtNumber+1;
				s.type = STMT_PROCEDURE;
				// sm[1] : the procedure name
				s.extraName = sm[1];
				//Hack: Assume the procIndex here. Add to ProcTable while generating AST
				currentProcIndex++;
				s.procIndex = currentProcIndex;
				//procedure is a block, so push it into the scope stack
				statementScope.push(preprocProgram->size());

			}else if (regex_match(thisStmt, sm, whileRegex)) {
				currentStmtNumber++;
				s.stmtLine = thisStmt;
				s.stmtNumber = currentStmtNumber;
				s.type = STMT_WHILE;
				s.extraCond = sm[1];
				assert(currentProcIndex>-1);
				s.procIndex = currentProcIndex;
				//while is a block, so push it into the scope stack
				statementScope.push(preprocProgram->size());

			}else if (regex_match(thisStmt, sm, callRegex)) {
				currentStmtNumber++;
				s.stmtLine = thisStmt;
				s.stmtNumber = currentStmtNumber;
				s.type = STMT_CALL;
				s.extraName = sm[1];
				assert(currentProcIndex>-1);
				s.procIndex = currentProcIndex;

			}else if (regex_match(thisStmt, sm, assignRegex)) {
				currentStmtNumber++;
				s.stmtLine = thisStmt;
				s.stmtNumber = currentStmtNumber;
				s.type = STMT_ASSIGNMENT;
				s.extraExpr = sm[2];
				s.extraVar = sm[1];
				assert(currentProcIndex>-1);
				s.procIndex = currentProcIndex;

			}else if (regex_match(thisStmt,sm,ifRegex)) {
				currentStmtNumber++;
				s.stmtLine = thisStmt;
				s.stmtNumber = currentStmtNumber;
				s.type = STMT_IF;
				s.extraCond = sm[1];
				assert(currentProcIndex>-1);
				s.procIndex = currentProcIndex;
				//if is a block, so push it into the scope stack
				//push twice for THEN and ELSE. Still hard to make CFG.
				statementScope.push(preprocProgram->size());
				statementScope.push(preprocProgram->size());
				preprocessingStatusStk.push(PREPROCESS_THEN);
			}else if (regex_match(thisStmt,sm,elseRegex)) {
				s.stmtLine = thisStmt;
				s.stmtNumber = NO_STATEMENT_NUMBER;
				s.type = STMT_ELSE;
				assert(currentProcIndex>-1);
				s.procIndex = currentProcIndex;

			}else {
				cout << "Parser cannot parse: " << thisStmt << endl;
			}
			preprocProgram->push_back(s);
		}

		if (nearestSeparator == LEFT_BRACKET) {
			statement lb;
			lb.type = STMT_OPEN_BRACKET;
			lb.stmtLine = "{";
			//Must be the first statement inside this block
			lb.stmtNumber = currentStmtNumber + 1;
			lb.procIndex = currentProcIndex;
			preprocProgram->push_back(lb);
		}else if (nearestSeparator == RIGHT_BRACKET) {
			statement rb;
			rb.stmtLine = "}";
			//Must be the last statement inside this block
			rb.stmtNumber = currentStmtNumber;
			rb.procIndex = currentProcIndex;
			//check the top scope
			assert(statementScope.size()>0);
			//define end of the scope
			rb.startOfTheScope = preprocProgram->at(statementScope.top()).stmtNumber;
			preprocProgram->at(statementScope.top()).endOfTheScope = currentStmtNumber;

			switch(preprocProgram->at(statementScope.top()).type)
			{
			case STMT_PROCEDURE:
				rb.type = STMT_CLOSE_BRACKET_END_OF_PROC;
				break;
			case STMT_IF:
				if(_preprocessingStatus() == PREPROCESS_THEN)
				{
					preprocProgram->at(statementScope.top()).midOfTheScope = currentStmtNumber;
					rb.type = STMT_CLOSE_BRACKET_END_OF_THEN;
					preprocessingStatusStk.top() = PREPROCESS_ELSE;
				}
				else if (_preprocessingStatus() == PREPROCESS_ELSE)
				{
					preprocProgram->at(statementScope.top()).endOfTheScope = currentStmtNumber;
					rb.type = STMT_CLOSE_BRACKET_END_OF_ELSE;
					preprocessingStatusStk.pop();
				}
				else
				{
					throw ("Error occurred while preprocessing IF Scope.");
				}
				break;
			case STMT_WHILE:
				rb.type = STMT_CLOSE_BRACKET_END_OF_WHILE;
				break;
			default:
				throw("Unknown Scope while preprocssing the program.");
			}
			statementScope.pop();
			//push back
			preprocProgram->push_back(rb);
		}
	} while (nearestSeparator != -1);
}


inline int Parser::_findValidMinimumOfThree(size_t lb, size_t rb, size_t sc)
{
	size_t minimumValue = 32767;
	int minimumIndex = -1;

	if ((lb < minimumValue) && (lb != string::npos)) {
		minimumValue = lb;
		minimumIndex = LEFT_BRACKET;
	}
	if ((rb < minimumValue) && (rb != string::npos)) {
		minimumValue = rb;
		minimumIndex = RIGHT_BRACKET;
	}
	if ((sc < minimumValue) && (sc != string::npos)) {
		minimumValue = sc;
		minimumIndex = SEMICOLON;
	}
	return(minimumIndex);
}


ASTNode *Parser::_buildAssignmentAST(statement *s)
{
	ASTNode *node = ASTNode::createNode(AST_ASSIGNMENT, NULL);

	node->setStmtNumber(s->stmtNumber);
	//0 is the index in varTable
	node->createChild(AST_VARIABLE, _pkb->addVar(s->extraVar));

	//add modifies
	_pkb->addModifies(s->stmtNumber, _pkb->addVar(s->extraVar));
	_pkb->addModifiesP(s->procIndex, _pkb->addVar(s->extraVar));

	//***************
	//build expression tree. infix to postfix
	string postfixExpr = "";
	string infixExpr = s->extraExpr;
	//remove all whitespaces
	regex removeWhitespace("\\s+");
	string rmvWS("");
	//add #var# and %constant% pattern to infix expression
	regex hashAllVariables("[a-zA-Z][a-zA-Z0-9]*"); //with #
	string varFmt("#$&#");
	string constFmt("$1%$2%");
	regex hashAllConstants("([-\\+\\*\\s*])([0-9]+)"); //with %

	infixExpr = regex_replace(infixExpr, removeWhitespace, rmvWS);
	infixExpr = regex_replace(infixExpr, hashAllVariables, varFmt);
	infixExpr = regex_replace(infixExpr, hashAllConstants, constFmt);
	
	//If an expression starts with a constant, the constant won't be hashed. So go find it.
	//Future to do with negative numbers?
	int ci = 0;

	for(ci=0;ci<infixExpr.size();ci++)
		if (!isdigit(infixExpr.at(ci)))
			break;

	if(ci != 0)
	{
		infixExpr.insert(0,"%");
		infixExpr.insert(ci+1,"%");
	}

	//iterate
	stack<char> operatorStack;
	for (std::string::iterator it = infixExpr.begin(); it != infixExpr.end(); ++it) {
		char c = *it;
		if ((c != '(') && (c != ')') && (c != '+') && (c != '-') && (c != '*')) {
			postfixExpr.append(string(1, c));
		}else if (c == '(') {
			operatorStack.push(c);
		}else if (c == ')') {
			char s;
			while ((s = operatorStack.top()) != '(') {
				operatorStack.pop();
				postfixExpr.append(string(1, s));
			}
		}else {
			int exprPrec = _operatorPrecedence(c);
			int stackPrec;
			if (operatorStack.size() == 0) {
				stackPrec = 0;
			}else {
				stackPrec = _operatorPrecedence(operatorStack.top());
			}
			if (exprPrec > stackPrec) {
				operatorStack.push(c);
			}else {
				while (stackPrec >= exprPrec) {
					if (operatorStack.size() == 0) {
						break;
					}
					char oldTop = operatorStack.top();
					operatorStack.pop();
					postfixExpr.append(string(1, oldTop));
					stackPrec = (operatorStack.size() == 0) ? 0 : _operatorPrecedence(operatorStack.top());
				}
				operatorStack.push(c);
			}
		}
	}
	while (operatorStack.size() > 0) {
		if(operatorStack.top()!='(')
			postfixExpr.append(string(1, operatorStack.top()));
		operatorStack.pop();
	}

	//cout << "\nPOSTFIX:\n" << postfixExpr << endl;

	//Pass 2. go through postFixExpr and build AST Tree.
	stack<ASTNode *> variableStack;

	int concatState = 0; // 0 - CONCAT_NONE, 1 - CONCAT_DIGIT, 2 - CONCAT_VARIABLE
	string postfixValue = "";

	for (string::iterator it = postfixExpr.begin(); it != postfixExpr.end(); ++it) {
		char c = *it;
		if ((c == '+') || (c == '-') || (c == '*')) {
			ASTNode *rightOperand = variableStack.top();
			variableStack.pop();
			ASTNode *leftOperand = variableStack.top();
			variableStack.pop();
			ASTNode *operatorNode;
			switch (c) {
			case '+':
				operatorNode = ASTNode::createNode(AST_PLUS, NULL);
				break;

			case '-':
				operatorNode = ASTNode::createNode(AST_MINUS, NULL);
				break;

			case '*':
				operatorNode = ASTNode::createNode(AST_MULTIPLY, NULL);
				break;

			default:
				operatorNode = ASTNode::createNode(AST_UNKNOWN, NULL);
			}
			operatorNode->addChild(leftOperand);
			operatorNode->addChild(rightOperand);
			variableStack.push(operatorNode);
		}else if (c == '%') {
			if (concatState == 0) {
				concatState = 1;
			}else {
				int parsedInteger = atoi(postfixValue.c_str());
				ASTNode *constantNode = ASTNode::createNode(AST_CONSTANT, parsedInteger);
				_pkb->addConstant(parsedInteger);
				variableStack.push(constantNode);
				concatState = 0;
				postfixValue.clear();
			}
		}else if (c == '#') {
			if (concatState == 0) {
				concatState = 2;
			}else {
				//0 refer to value in varTable
				ASTNode *variableNode = ASTNode::createNode(AST_VARIABLE, PKBController::createInstance()->addVar(postfixValue));
				_pkb->addUses(s->stmtNumber,_pkb->addVar(postfixValue));
				_pkb->addUsesP(s->procIndex,_pkb->getVarIndex(postfixValue));
				variableStack.push(variableNode);
				concatState = 0;
				postfixValue.clear();
			}
		}else {
			postfixValue.append(string(1, c));
		}
	}
	//Update ParentTable
	if(_parentStackNoStmtLst.top()->getStmtNumber()>0) _pkb->addParent(_parentStackNoStmtLst.top()->getStmtNumber(), s->stmtNumber);
	node->addChild(variableStack.top());
	_parentStack.top()->addChild(node);
	_pkb->addAssignmentNode(node);
	return(node);
}

ASTNode* Parser::_buildIfAST(statement* s)
{
	ASTNode *node = ASTNode::createNode(AST_IF_BRANCH, _pkb->addVar(s->extraCond));

	node->setStmtNumber(s->stmtNumber);
	_parentStack.top()->addChild(node);

	node->createChild(AST_VARIABLE, _pkb->addVar(s->extraCond));
	ASTNode *stmtLstNode = node->createChild(AST_STATEMENT_LIST, NULL);
	_newParent = stmtLstNode;
	_newParentNoStmtLst = node;
	if(_parentStackNoStmtLst.top()->getStmtNumber()>0) _pkb->addParent(_parentStackNoStmtLst.top()->getStmtNumber(), s->stmtNumber);
	_pkb->addUses(s->stmtNumber,_pkb->addVar(s->extraCond));
	_pkb->addUsesP(s->procIndex,_pkb->addVar(s->extraCond));
	_pkb->addIfBranchNode(node);
	return(node);
}

ASTNode* Parser::_buildElseAST(statement* s)
{
	ASTNode *stmtLstNode = previousASTNode->createChild(AST_STATEMENT_LIST, NULL);
	_newParent = stmtLstNode;
	_newParentNoStmtLst = previousASTNode;
	return(previousASTNode);
}

ASTNode* Parser::_buildCallAST( statement* s )
{
	//NodeValue is Callee's procIndex, no idea if callers procindex needed.
	ASTNode *node = ASTNode::createNode(AST_CALL,_findAssumedProcIndexByName(s->extraName));
	node->setStmtNumber(s->stmtNumber);
	_parentStack.top()->addChild(node);

	node->createChild(AST_PROCEDURE, _findAssumedProcIndexByName(s->extraName));
	_pkb->addCalls(s->stmtNumber,s->procIndex,_findAssumedProcIndexByName(s->extraName));
	//Update ParentTable
	if(_parentStackNoStmtLst.top()->getStmtNumber()>0) _pkb->addParent(_parentStackNoStmtLst.top()->getStmtNumber(), s->stmtNumber);
	return(node);
}

ASTNode *Parser::_buildWhileLoopAST(statement *s)
{
	ASTNode *node = ASTNode::createNode(AST_WHILE_LOOP, _pkb->addVar(s->extraCond));

	node->setStmtNumber(s->stmtNumber);
	_parentStack.top()->addChild(node);
	//0 is the index in varTable
	node->createChild(AST_VARIABLE, _pkb->addVar(s->extraCond));
	ASTNode *stmtLstNode = node->createChild(AST_STATEMENT_LIST, NULL);
	_newParent = stmtLstNode;
	_newParentNoStmtLst = node;
	if(_parentStackNoStmtLst.top()->getStmtNumber()>0) _pkb->addParent(_parentStackNoStmtLst.top()->getStmtNumber(), s->stmtNumber);
	_pkb->addUses(s->stmtNumber,_pkb->addVar(s->extraCond));
	_pkb->addUsesP(s->procIndex,_pkb->addVar(s->extraCond));
	_pkb->addWhileLoopNode(node);
	return(node);
}


ASTNode *Parser::_buildProcedureAST(statement *s)
{
	//0 is the index in procTable
	ASTNode *node = ASTNode::createNode(AST_PROCEDURE, s->procIndex);

	_parentStack.top()->addChild(node);
	ASTNode *stmtLstNode = node->createChild(AST_STATEMENT_LIST, NULL);
	_newParent = stmtLstNode;
	_newParentNoStmtLst = node;
	return(node);
}

inline int Parser::_operatorPrecedence(char op)
{
	switch (op) {

	case '*':
		return(4);

	case '+':
		return(2);

	case '-':
		return(2);

	default:
		return(0);
	}
}

/**
 * \fn	void Parser::parse( string program )
 *
 * \brief	Parse a program, build Abstract Syntax Tree of the program.
 * 			Parser also finds Parent, Uses, Modifies and Calls relation in the program and 
 * 			add them to respective tables.
 * 			
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	program	Input program.
 */

void Parser::parse( string program )
{
	this->_preprocessProgram(program);
	this->_parseLine();
	PKBController::createInstance()->completePKB();
}

PROC_INDEX Parser::_findAssumedProcIndexByName( string name )
{
	for(vector<statement>::iterator it = _pkb->getPreprocessedProgram()->begin(); it != _pkb->getPreprocessedProgram()->end(); ++it )
		if(it->type == STMT_PROCEDURE && it->extraName == name)
			return it->procIndex;

	throw("No Specific Procedure Name Found");
	return -1;
}

IfPreprocessingPhase Parser::_preprocessingStatus()
{
	if(preprocessingStatusStk.size()>0)
		return preprocessingStatusStk.top();
	else
		return PREPROCESS_NORMAL_BLOCK;
}

void Parser::__addFollowsTable( ASTNode* currentASTNode )
{
	if(sameLevelAtNext)
	{
		if(previousASTNode->getStmtNumber()>0)
			_pkb->addFollows(previousASTNode->getStmtNumber(), currentASTNode->getStmtNumber());
	}
	else
	{
		sameLevelAtNext = true;
	}
}
