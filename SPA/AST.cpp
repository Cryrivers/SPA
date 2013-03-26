#include "stdafx.h"
#include "AST.h"
#include <assert.h>

using namespace std;

AST::AST(void)
{
	this->_root = ASTNode::createNode(AST_PROGRAM, NULL);
	this->setASTNodeIndex(_root);
}


AST::~AST(void)
{
}

/**
 * \fn	ASTNode *AST::getRoot()
 *
 * \brief	Gets the root ASTNode of this AST.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	null if it fails, else the root ASTNode object.
 */

ASTNode *AST::getRoot()
{
	return(this->_root);
}


vector<int> *AST::containSubtree(ASTNode *tree)
{
	_calcHashcode(tree->getChildren()->getSibling());
	vector<int>* result  = new vector<int>();
	vector<ASTNode*>* assignTable = PKBController::createInstance()->getAssignmentTable();
	for(std::vector<ASTNode*>::iterator it = assignTable->begin(); it != assignTable->end();++it)
	{
		_calcHashcode((*it)->getChildren()->getSibling());
		ASTNode* lhs = (*it)->getChildren()->getSibling();
		ASTNode* rhs = tree->getChildren()->getSibling();
		//Compare the expression tree in Assignment Node.
		size_t loc;
		loc = lhs->getHashcode().find(rhs->getHashcode());
		if(loc != string::npos)
		{
			if(tree->getChildren()->getNodeType() == AST_ANY || (tree->getChildren()->getNodeValue() == (*it)->getChildren()->getNodeValue()))
			{
				result->push_back((*it)->getStmtNumber());
			}
		}
	}
	return(result);
}

/**
 * \fn	vector<int> *AST::containTree(ASTNode *tree)
 *
 * \brief	Query if AST Contains the specific tree.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param [in]	tree	If non-null, the tree.
 *
 * \return	vector of integer containing the statement numbers of the statments that contains the specific tree.
 */

vector<int> *AST::containTree(ASTNode *tree)
{
	_calcHashcode(tree->getChildren()->getSibling());
	vector<int>* result  = new vector<int>();
	vector<ASTNode*>* assignTable = PKBController::createInstance()->getAssignmentTable();
	for(std::vector<ASTNode*>::iterator it = assignTable->begin(); it != assignTable->end();++it)
	{
		PKBController* _pkb = PKBController::createInstance();
		//Compare the expression tree in Assignment Node.
		_calcHashcode((*it)->getChildren()->getSibling());
		ASTNode* lhs = (*it)->getChildren()->getSibling();
		ASTNode* rhs = tree->getChildren()->getSibling();
		if(lhs->getHashcode() == rhs->getHashcode())
		{
			if(tree->getChildren()->getNodeType() == AST_ANY || (tree->getChildren()->getNodeValue() == (*it)->getChildren()->getNodeValue()))
			{
				result->push_back((*it)->getStmtNumber());
			}
		}
	}
	return(result);
}


ASTNode *AST::buildSubtree(string expression)
{
	string postfixExpr = "";
	string infixExpr = expression;
	//remove all whitespaces
	regex removeWhitespace("\\s+");
	string rmvWS("");
	//add #var# and %constant% pattern to infix expression
	regex hashAllVariables("[a-zA-Z][a-zA-Z0-9]*"); //with #
	string varFmt("#$&#");
	string constFmt("$1%$2%");
	regex hashAllConstants("([-\\(\\+\\*\\s*])([0-9]+)"); //with %

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

	/*cout << "\nPOSTFIX:\n" << postfixExpr << endl;*/

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
				ASTNode *constantNode = ASTNode::createNode(AST_CONSTANT, atoi(postfixValue.c_str()));
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
				variableStack.push(variableNode);
				concatState = 0;
				postfixValue.clear();
			}
		}else {
			postfixValue.append(string(1, c));
		}
	}

	return(variableStack.top());
}


inline int AST::_operatorPrecedence(char op)
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


ASTNode *AST::buildAssignmentSubtree(int varIndex, string expression)
{
	ASTNode *node = ASTNode::createNode(AST_ASSIGNMENT, NULL);

	node->setStmtNumber(0);
	//-1 = UNDERSCORE
	if (varIndex != -1) {
		node->createChild(AST_VARIABLE, varIndex);
	}else {
		node->createChild(AST_ANY, NULL);
	}

	node->addChild(buildSubtree(expression));
	return(node);
}

/**
 * \fn	void AST::calculateHashcode()
 *
 * \brief	Cacalculates every ASTNode's Hashcode in this AST.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 */

void AST::calculateHashcode()
{
	_calcHashcode(_root);
}


string AST::_calcHashcode(ASTNode *node)
{
	if(node->isHashcodeComputed()) return (node->getHashcode());

	if ((node->getChildren() == NULL) && (node->getSibling() == NULL)) {
		node->markHashcodeComputed();
		return(node->getHashcode());
	}
	if (node->getChildren() != NULL) {
		node->appendHashcode(_calcHashcode(node->getChildren()));
	}
	if (node->getSibling() != NULL) {
		node->appendHashcode(_calcHashcode(node->getSibling()));
	}

	node->markHashcodeComputed();
	return(node->getHashcode());
}

bool AST::_containTree( ASTNode* container, ASTNode* object )
{
	if((container->getChildren() == NULL) && (container->getSibling() == NULL))
	{
		return (container->getHashcode() == object->getHashcode());
	}
	
	bool c = false;

	if (container->getChildren() != NULL) {
		c = _containTree(container->getChildren(),object);
		if(c) return true;
	}
	
	if (container->getSibling() != NULL) {
		c = _containTree(container->getSibling(),object);
		if(c) return true;
	}

	return false;
}

/**
 * \fn	vector<int>* AST::matchWhileLoop( VAR_INDEX ind )
 *
 * \brief	Match while loop.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \param	ind	The ind.
 *
 * \return	null if it fails, else.
 */

vector<int>* AST::matchWhileLoop( VAR_INDEX ind )
{
	vector<int>* result  = new vector<int>();
	vector<ASTNode*>* whileTable = PKBController::createInstance()->getWhileTable();
	for(std::vector<ASTNode*>::iterator it = whileTable->begin(); it != whileTable->end();++it)
		if((*it)->getNodeValue() == ind)
			result->push_back((*it)->getStmtNumber());
	return(result);
}

/**
 * \fn	vector<int>* AST::matchIfBranch( VAR_INDEX ind )
 *
 * \brief	Match if branch.
 *
 * \author	Wang Zhongliang
 * \date	2013/2/19
 *
 * \param	ind	The ind.
 *
 * \return	null if it fails, else.
 */

vector<int>* AST::matchIfBranch( VAR_INDEX ind )
{
	vector<int>* result  = new vector<int>();
	vector<ASTNode*>* ifTable = PKBController::createInstance()->getIfBranchTable();
	for(std::vector<ASTNode*>::iterator it = ifTable->begin(); it != ifTable->end();++it)
		if((*it)->getNodeValue() == ind)
			result->push_back((*it)->getStmtNumber());
	return(result);
}

void AST::setASTNodeIndex( ASTNode* node )
{
	assert(node->getNodeType() != AST_ANY);
	assert(node->getNodeType() != AST_UNKNOWN);
	_astIndex.push_back(node);
	node->setId(_astIndex.size() - 1);
}

