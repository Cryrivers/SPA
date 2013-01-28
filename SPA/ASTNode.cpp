#include "stdafx.h"
#include "ASTNode.h"
#include <assert.h>
#include <string>

ASTNode::ASTNode(void)
{
	_stmtNumber = 0;
	_children = NULL;
	_siblings = NULL;
	hashCodeComputed = false;
}


ASTNode::~ASTNode(void)
{
	delete _siblings;
	delete _children;
}


ASTNode *ASTNode::createNode(ASTNodeType t, int value)
{
	ASTNode *node = new ASTNode();

	node->setNodeType(t);
	node->setNodeValue(value);
	switch (t) {
	case AST_UNKNOWN:
		node->_hashcode = "uk";
		break;

	case AST_CALL:
		node->_hashcode = "ca";
		break;

	case AST_ELSE_BRANCH:
		node->_hashcode = "el";
		break;

	case AST_PROGRAM:
		node->_hashcode = "pg";
		break;

	case AST_PROCEDURE:
		node->_hashcode = "pc";
		break;

	case AST_STATEMENT_LIST:
		break;

	case AST_VARIABLE:
		node->_hashcode = "v";
		node->_hashcode.append(to_string(static_cast<long long>(value)));
		break;

	case AST_CONSTANT:
		node->_hashcode = "c";
		node->_hashcode.append(to_string(static_cast<long long>(value)));
		break;

	case AST_PLUS:
		node->_hashcode = "pl";
		break;

	case AST_MINUS:
		node->_hashcode = "mi";
		break;

	case AST_MULTIPLY:
		node->_hashcode = "mp";
		break;

	case AST_ASSIGNMENT:
		node->_hashcode = "as";
		break;

	case AST_WHILE_LOOP:
		node->_hashcode = "wl";
		node->_hashcode.append(to_string(static_cast<long long>(value)));
		break;

	case AST_IF_BRANCH:
		node->_hashcode = "ib";
		node->_hashcode.append(to_string(static_cast<long long>(value)));
		break;

	case AST_ANY:
		node->_hashcode = "_";
		break;
	}
	return(node);
}


ASTNodeType ASTNode::getNodeType()
{
	return(_nodeType);
}


void ASTNode::setNodeType(ASTNodeType t)
{
	_nodeType = t;
}


int ASTNode::getNodeValue()
{
	return(_nodeValue);
}


void ASTNode::setNodeValue(int v)
{
	_nodeValue = v;
}


ASTNode *ASTNode::getChildren(int index /*= 0*/)
{
	return(_children);
}


void ASTNode::addChild(ASTNode *node)
{
	if (_children == NULL) {
		_children = node;
	}else {
		ASTNode *i;
		i = _children;
		while (i->getSibling() != NULL) {
			i = i->getSibling();
		}
		i->addSibling(node);
	}
}


ASTNode *ASTNode::createChild(ASTNodeType t, int value)
{
	ASTNode *i = ASTNode::createNode(t, value);

	this->addChild(i);
	return(i);
}


ASTNode *ASTNode::getSibling()
{
	return(_siblings);
}


void ASTNode::addSibling(ASTNode *node)
{
	ASTNode *i;

	i = this;
	while (i->getSibling() != NULL) {
		i = i->getSibling();
	}
	i->_siblings = node;
}


ASTNode *ASTNode::createSibling(ASTNodeType t, int value)
{
	ASTNode *i = ASTNode::createNode(t, value);

	this->addSibling(i);
	return(i);
}


int ASTNode::getStmtNumber()
{
	return(_stmtNumber);
}


void ASTNode::setStmtNumber(int number)
{
	_stmtNumber = number;
}


std::string ASTNode::getHashcode()
{
	return(_hashcode);
}


string ASTNode::appendHashcode(string hashCode)
{
	_hashcode.append(hashCode);
	return(_hashcode);
}

void ASTNode::markHashcodeComputed()
{
	hashCodeComputed = true;
}
bool ASTNode::isHashcodeComputed()
{
	return hashCodeComputed;
}