/**
 * \file	ASTNode.cpp
 * \class	ASTNode
 * \brief	Implements the basic element of AST which is AST Node.
 */

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

/**
 * \fn	ASTNode *ASTNode::createNode(ASTNodeType t, int value)
 *
 * \brief	Creates a new ASTNode with specific node type and value.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	t	 	The type of this ASTNode.
 * \param	value	The custom value for this ASTNode. (Can be ProcIndex, VarIndex or NULL)
 *
 * \return	The new ASTNode.
 */

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
		//Unique Proc Index
		node->_hashcode.append(to_string(static_cast<long long>(value)));
		break;

	case AST_STATEMENT_LIST:
		break;

	case AST_VARIABLE:
		node->_hashcode = "v";
		//Unique Variable Index
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

/**
 * \fn	ASTNodeType ASTNode::getNodeType()
 *
 * \brief	Gets the type of current ASTNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	The node type.
 */

ASTNodeType ASTNode::getNodeType()
{
	return(_nodeType);
}

/**
 * \fn	void ASTNode::setNodeType(ASTNodeType t)
 *
 * \brief	Sets the type of ASTNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	t	The ASTNodeType to process.
 */

void ASTNode::setNodeType(ASTNodeType t)
{
	_nodeType = t;
}

/**
 * \fn	int ASTNode::getNodeValue()
 *
 * \brief	Gets node value.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	The node value.
 */

int ASTNode::getNodeValue()
{
	return(_nodeValue);
}

/**
 * \fn	void ASTNode::setNodeValue(int v)
 *
 * \brief	Sets ASTNode's custom value.
 * 			The custom value can be ProcIndex, VarIndex, constant, etc.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	v	The value.
 */

void ASTNode::setNodeValue(int v)
{
	_nodeValue = v;
}

/**
 * \fn	ASTNode *ASTNode::getChildren(int index )
 *
 * \brief	Gets the first child of this ASTNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	null if it has no child, else the first child of this ASTNode.
 */

ASTNode *ASTNode::getChildren()
{
	return(_children);
}

/**
 * \fn	void ASTNode::addChild(ASTNode *node)
 *
 * \brief	Adds a existing ASTNode to this one as a child ASTNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param [in]	node	the ASTNode.
 */

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

/**
 * \fn	ASTNode *ASTNode::createChild(ASTNodeType t, int value)
 *
 * \brief	Creates a child ASTNode of this one.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	t	 	The ASTNodeType to process.
 * \param	value	The value.
 *
 * \return	The new ASTNode as a child of this ASTNode.
 */

ASTNode *ASTNode::createChild(ASTNodeType t, int value)
{
	ASTNode *i = ASTNode::createNode(t, value);

	this->addChild(i);
	return(i);
}

/**
 * \fn	ASTNode *ASTNode::getSibling()
 *
 * \brief	Gets this ASTNode's next sibling.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	null if it has no sibling, else its next sibling.
 */

ASTNode *ASTNode::getSibling()
{
	return(_siblings);
}

/**
 * \fn	void ASTNode::addSibling(ASTNode *node)
 *
 * \brief	Add a existing ASTNode to this one as its sibling.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param [in]	node	the ASTNode
 */

void ASTNode::addSibling(ASTNode *node)
{
	ASTNode *i;

	i = this;
	while (i->getSibling() != NULL) {
		i = i->getSibling();
	}
	i->_siblings = node;
}

/**
 * \fn	ASTNode *ASTNode::createSibling(ASTNodeType t, int value)
 *
 * \brief	Creates a new ASTNode which is the sibling of this ASTNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	t	 	The ASTNodeType to process.
 * \param	value	The value.
 *
 * \return	The new sibling.
 */

ASTNode *ASTNode::createSibling(ASTNodeType t, int value)
{
	ASTNode *i = ASTNode::createNode(t, value);

	this->addSibling(i);
	return(i);
}

/**
 * \fn	int ASTNode::getStmtNumber()
 *
 * \brief	Gets this ASTNode's Statement Number.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	The Statement Number of this ASTNode.
 */

int ASTNode::getStmtNumber()
{
	return(_stmtNumber);
}

/**
 * \fn	void ASTNode::setStmtNumber(int number)
 *
 * \brief	Sets Statement Number to ASTNode.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	number	The Statement Number.
 */

void ASTNode::setStmtNumber(int number)
{
	_stmtNumber = number;
}

/**
 * \fn	std::string ASTNode::getHashcode()
 *
 * \brief	Calculates the Hashcode for this object.
 * 			Hashcode is a string containing all details of a node, 
 * 			its children and its siblings.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	The Hashcode.
 */

std::string ASTNode::getHashcode()
{
	return(_hashcode);
}

/**
 * \fn	string ASTNode::appendHashcode(string hashCode)
 *
 * \brief	Appends a Hashcode.
 * 
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \param	hashCode	The Hashcode.
 *
 * \return	the new Hashcode
 */

string ASTNode::appendHashcode(string hashCode)
{
	_hashcode.append(hashCode);
	return(_hashcode);
}

/**
 * \fn	void ASTNode::markHashcodeComputed()
 *
 * \brief	Mark this ASTNode's Hashcode computed.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 */

void ASTNode::markHashcodeComputed()
{
	hashCodeComputed = true;
}

/**
 * \fn	bool ASTNode::isHashcodeComputed()
 *
 * \brief	Query if this ASTNode's Hashcode is computed.
 *
 * \author	Wang Zhongliang
 * \date	2013/1/29
 *
 * \return	True if Hashcode computed, False if not.
 */

bool ASTNode::isHashcodeComputed()
{
	return hashCodeComputed;
}